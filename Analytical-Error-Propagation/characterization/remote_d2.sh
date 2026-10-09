#!/usr/bin/env bash
# Build the depth-1 and depth-2 tables of a benchmark on a remote machine, in a tmux session.
#
#     characterization/remote_d2.sh <host> <benchmark> [workers]
#
# The code is synced to <host>:~/aep_d2/repo, the job runs in tmux session d2_<benchmark>
# and logs to ~/aep_d2/logs/<benchmark>.log. Fetch the tables afterwards with
#     rsync -a <host>:aep_d2/repo/data/node_metrics/<benchmark>* data/node_metrics/
set -euo pipefail

host=${1:?host}
bench=${2:?benchmark}
workers=${3:-}
cd "$(dirname "$0")/.."

ssh -o BatchMode=yes "$host" "tmux has-session -t d2_$bench 2>/dev/null" && {
    echo "tmux session d2_$bench already runs on $host"; exit 1; }

rsync -a --prune-empty-dirs \
    --include='model/***' --include='characterization/***' \
    --include='sim/' --include='sim/__init__.py' --include='sim/simulate.py' --include='sim/build.sh' --include='sim/src/***' \
    --include='data/' --include='data/benchmarks/***' --include='data/*.json' \
    --exclude='__pycache__' --exclude='*' \
    ./ "$host:aep_d2/repo/"

ssh -o BatchMode=yes "$host" bash -s -- "$bench" "$workers" <<'REMOTE'
set -euo pipefail
bench=$1
workers=${2:-$(( $(nproc) - 2 ))}
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
cat > "jobs/$bench.sh" <<JOB
#!/usr/bin/env bash
set -euo pipefail
cd ~/aep_d2/repo
py=~/aep_d2/venv/bin/python3
{
    echo "== \$(date '+%F %T') $bench on \$(hostname), $workers workers"
    \$py -B -m characterization.characterize $bench
    \$py -B -m characterization.characterize_deep $bench --workers $workers --check 10
    \$py -B -m characterization.characterize_deep $bench --workers $workers
    echo "== \$(date '+%F %T') $bench done"
} 2>&1 | tee -a ~/aep_d2/logs/$bench.log
JOB
chmod +x "jobs/$bench.sh"
tmux new-session -d -s "d2_$bench" "bash ~/aep_d2/jobs/$bench.sh; echo; echo 'job ended, press enter'; read"
echo "started tmux session d2_$bench on $(hostname) with $workers workers"
REMOTE
