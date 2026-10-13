#!/usr/bin/env bash
# Build the depth-1 and depth-2 tables of one or more benchmarks on a remote machine, one after
# another, in a tmux session.
#
#     characterization/remote_d2.sh <host> <benchmark> [<benchmark> ...]
#     WORKERS=64 characterization/remote_d2.sh <host> <benchmark> ...
#
# The code is synced to <host>:~/aep_d2/repo, the job runs in tmux session d2_<first benchmark>
# and logs to ~/aep_d2/logs/<benchmark>.log. If a benchmark fails, the ones after it are not started.
# Fetch the tables afterwards with
#     rsync -a <host>:aep_d2/repo/data/node_metrics/d1/<benchmark>*.json data/node_metrics/d1/
#     rsync -a <host>:aep_d2/repo/data/node_metrics/d2/<benchmark>_d2 data/node_metrics/d2/
set -euo pipefail

host=${1:?host}
shift
[ $# -gt 0 ] || { echo "usage: $0 <host> <benchmark> [<benchmark> ...]"; exit 1; }
benches=("$@")
session=d2_${benches[0]}
workers=${WORKERS:-auto}  # never empty: ssh drops empty arguments
cd "$(dirname "$0")/.."

ssh -o BatchMode=yes "$host" "tmux has-session -t $session 2>/dev/null" && {
    echo "tmux session $session already runs on $host"; exit 1; }
ssh -o BatchMode=yes "$host" "mkdir -p ~/aep_d2/repo"

rsync -a --prune-empty-dirs \
    --include='model/***' --include='characterization/***' \
    --include='sim/' --include='sim/__init__.py' --include='sim/simulate.py' --include='sim/build.sh' --include='sim/src/***' \
    --include='data/' --include='data/benchmarks/***' --include='data/*.json' \
    --exclude='__pycache__' --exclude='*' \
    ./ "$host:aep_d2/repo/"

ssh -o BatchMode=yes "$host" bash -s -- "$session" "$workers" "${benches[@]}" <<'REMOTE'
set -euo pipefail
session=$1
workers=$2
shift 2
[ "$workers" = auto ] && workers=$(( $(nproc) - 2 ))
case $workers in ''|*[!0-9]*) echo "workers must be a number, got '$workers'"; exit 1 ;; esac
cd ~/aep_d2
if [ ! -x venv/bin/python3 ] || ! venv/bin/python3 -c "import numpy, pydantic" 2>/dev/null; then
    rm -rf venv
    python3 -m venv --system-site-packages --without-pip venv
    curl -sS -o get-pip.py https://bootstrap.pypa.io/get-pip.py
    venv/bin/python3 get-pip.py -q
    venv/bin/python3 -m pip install -q pydantic
fi
sh repo/sim/build.sh >/dev/null
mkdir -p logs jobs
for bench in "$@"; do
    [ -f "repo/data/benchmarks/$bench.json" ] || ls repo/data/benchmarks/*/"$bench.json" >/dev/null 2>&1 \
        || { echo "no benchmark $bench"; exit 1; }
done
cat > "jobs/$session.sh" <<JOB
#!/usr/bin/env bash
set -euo pipefail
cd ~/aep_d2/repo
py=~/aep_d2/venv/bin/python3
for bench in $*; do
    {
        echo "== \$(date '+%F %T') \$bench on \$(hostname), $workers workers"
        \$py -B -m characterization.characterize \$bench
        \$py -B -m characterization.characterize_deep \$bench --workers $workers --check 10
        \$py -B -m characterization.characterize_deep \$bench --workers $workers
        echo "== \$(date '+%F %T') \$bench done"
    } 2>&1 | tee -a ~/aep_d2/logs/\$bench.log
done
echo "== \$(date '+%F %T') all done: $*"
JOB
chmod +x "jobs/$session.sh"
tmux new-session -d -s "$session" "bash ~/aep_d2/jobs/$session.sh; echo; echo 'job ended, press enter'; read"
echo "started tmux session $session on $(hostname) with $workers workers: $*"
REMOTE
