"""
Depth-2 feeder table: each adder's own error measured for every unit on the node,
every unit on its feeders and every unit one level further upstream.

    python3 characterize_deep.py <benchmark> [--workers N] [--samples N] [--check N]

Writes data/node_metrics/<benchmark>_d2/: meta.json and one <node>.npy per adder,
shape (own unit, history a, history b, 4) with [E[dr], E[dr^2], E[d*dr], E[d]].
Progress is kept in <node>_done.npy, so an interrupted run resumes where it stopped.
--check N computes N random entries and exits without building the table.
"""
import os

os.environ.setdefault("OMP_NUM_THREADS", "1")
os.environ.setdefault("OPENBLAS_NUM_THREADS", "1")
os.environ.setdefault("MKL_NUM_THREADS", "1")

import itertools
import json
import shutil
import sys
import time
from multiprocessing import Pool

import numpy as np

from model.data import DATA, load_benchmark
from sim.simulate import EXACT, UNITS, run_unit, sample_inputs

SEED = 1
BATCH = 10

STATE = {}


def codes_of(op):
    return [EXACT[op]] + UNITS[op]


def exact_values(dfg, inputs):
    values = dict(inputs)
    for node in dfg.nodes:
        a, b = node.inputs
        values[node.output] = run_unit(node.op, EXACT[node.op], values[a], values[b])
    return values


def cone_value(dfg, exact, wire, code):
    """Value of `wire` when every node of its producer's type in its cone uses `code`, other types exact."""
    producer = {node.output: node for node in dfg.nodes}
    op = producer[wire].op
    memo = {}

    def value(w):
        node = producer.get(w)
        if node is None:
            return exact[w]
        if w not in memo:
            memo[w] = run_unit(node.op, code if node.op == op else EXACT[node.op], value(node.inputs[0]), value(node.inputs[1]))
        return memo[w]

    return value(wire)


def port_layout(dfg, wire):
    """Dimensions of the depth-2 history of `wire`: (node id, codes) for the feeder and its non-primary inputs."""
    producer = {node.output: node for node in dfg.nodes}
    feeder = producer.get(wire)
    if feeder is None:
        return []
    dims = [(feeder.id, codes_of(feeder.op))]
    for x in feeder.inputs:
        q = producer.get(x)
        if q is not None:
            dims.append((q.id, codes_of(q.op)))
    return dims


def port_streams(dfg, exact, wire, path):
    """All depth-2 history values of `wire`, written to `path` as a (states, samples) int64 array."""
    producer = {node.output: node for node in dfg.nodes}
    feeder = producer[wire]
    inner = []
    for x in feeder.inputs:
        q = producer.get(x)
        inner.append({None: exact[x]} if q is None else {c: cone_value(dfg, exact, x, c) for c in codes_of(q.op)})
    states = list(itertools.product(codes_of(feeder.op), *[list(d) for d in inner]))
    tmp = f"{path}.tmp.npy"
    out = np.lib.format.open_memmap(tmp, mode="w+", dtype=np.int64, shape=(len(states), len(exact[wire])))
    for i, (fa, ga, gb) in enumerate(states):
        out[i] = run_unit(feeder.op, fa, inner[0][ga], inner[1][gb])
    out.flush()
    del out
    os.replace(tmp, path)
    return len(states)


def init_worker(meta_path, work_dir):
    meta = json.loads(open(meta_path).read())
    STATE["meta"], STATE["work"], STATE["out"] = meta, work_dir, os.path.dirname(os.path.abspath(meta_path))
    STATE["exact"] = dict(np.load(os.path.join(work_dir, "exact.npz")))
    STATE["streams"] = {}


def stream(wire):
    if wire not in STATE["streams"]:
        path = os.path.join(STATE["work"], f"stream_{wire}.npy")
        STATE["streams"][wire] = (np.load(path, mmap_mode="r") if os.path.exists(path)
                                  else STATE["exact"][wire][None, :])
    return STATE["streams"][wire]


def entries_for(node, ia, b_range=None):
    """Entries of one node for history ia of input a: array (own units, histories b, 4)."""
    wa, wb = node["inputs"]
    a_exact, b_exact = STATE["exact"][wa], STATE["exact"][wb]
    A = np.asarray(stream(wa)[ia])
    Bs = stream(wb)
    da = (A - a_exact).astype(np.float64)
    first, last = b_range or (0, Bs.shape[0])
    nb, n = last - first, A.shape[0]
    out = np.empty((len(node["own"]), nb, 4))
    for start in range(0, nb, BATCH):
        stop = min(start + BATCH, nb)
        B = np.asarray(Bs[first + start:first + stop])
        db = (B - b_exact).astype(np.float64)
        A_rep = np.broadcast_to(A, B.shape)
        for k, code in enumerate(node["own"]):
            own = (run_unit("add", code, A_rep.ravel(), B.ravel()) - (A_rep.ravel() + B.ravel())).reshape(B.shape).astype(np.float64)
            out[k, start:stop, 0] = own.sum(axis=1) / n
            out[k, start:stop, 1] = np.einsum("ij,ij->i", own, own) / n
            out[k, start:stop, 2] = (own @ da + np.einsum("ij,ij->i", own, db)) / n
            out[k, start:stop, 3] = (da.sum() + db.sum(axis=1)) / n
    return out


def run_task(task):
    node_id, ia = task
    node = STATE["meta"]["nodes"][node_id]
    values = entries_for(node, ia)
    table = np.load(os.path.join(STATE["out"], f"{node_id}.npy"), mmap_mode="r+")
    table[:, ia] = values
    table.flush()
    done = np.load(os.path.join(STATE["out"], f"{node_id}_done.npy"), mmap_mode="r+")
    done[ia] = True
    done.flush()
    return node_id, ia


def prepare(benchmark, n):
    dfg = load_benchmark(benchmark)
    out_dir = DATA / "node_metrics" / f"{benchmark}_d2"
    work_dir = out_dir / "work"
    work_dir.mkdir(parents=True, exist_ok=True)
    meta_path = out_dir / "meta.json"
    if meta_path.exists():
        return dfg, json.loads(meta_path.read_text()), meta_path, work_dir

    exact = exact_values(dfg, sample_inputs(dfg, n, SEED))
    np.savez(work_dir / "exact.npz", **exact)
    producer = {node.output: node for node in dfg.nodes}
    meta = {"benchmark": benchmark, "samples": n, "seed": SEED, "nodes": {}}
    for node in dfg.nodes:
        if node.op != "add":
            continue
        ports = []
        for wire in node.inputs:
            layout = port_layout(dfg, wire)
            if wire in producer and not (work_dir / f"stream_{wire}.npy").exists():
                port_streams(dfg, exact, wire, work_dir / f"stream_{wire}.npy")
            ports.append({"wire": wire, "dims": layout})
        size = [int(np.prod([len(c) for _, c in p["dims"]])) if p["dims"] else 1 for p in ports]
        meta["nodes"][node.id] = {"inputs": list(node.inputs), "own": UNITS["add"], "ports": ports}
        np.lib.format.open_memmap(out_dir / f"{node.id}.npy", mode="w+", dtype=np.float64,
                                  shape=(len(UNITS["add"]), size[0], size[1], 4)).flush()
        np.lib.format.open_memmap(out_dir / f"{node.id}_done.npy", mode="w+", dtype=bool, shape=(size[0],))[:] = False
    meta_path.write_text(json.dumps(meta, indent=1))
    return dfg, meta, meta_path, work_dir


def history_index(port, codes):
    index = 0
    for node_id, choices in port["dims"]:
        index = index * len(choices) + choices.index(codes[node_id])
    return index


def lookup(meta, tables, node_id, codes):
    """[E[dr], E[dr^2], E[d*dr], E[d]] of `node_id` under configuration `codes`."""
    node = meta["nodes"][node_id]
    ia, ib = (history_index(p, codes) for p in node["ports"])
    return tables[node_id][node["own"].index(codes[node_id]), ia, ib]


def log(msg):
    print(f"[{time.strftime('%Y-%m-%d %H:%M:%S')}] {msg}", flush=True)


def check(benchmark, n, count, meta, work_dir):
    """Compare random entries with a direct computation of the same quantity."""
    dfg = load_benchmark(benchmark)
    exact = dict(np.load(work_dir / "exact.npz"))
    producer = {node.output: node for node in dfg.nodes}
    init_worker(str(work_dir.parent / "meta.json"), str(work_dir))
    rng = np.random.default_rng(0)
    worst = 0.0
    for _ in range(count):
        node_id = str(rng.choice(sorted(meta["nodes"])))
        node = meta["nodes"][node_id]
        codes = {nd.id: int(rng.choice(codes_of(nd.op))) for nd in dfg.nodes}
        codes[node_id] = int(rng.choice(UNITS["add"]))
        ia, ib = (history_index(p, codes) for p in node["ports"])
        stored = entries_for(node, ia, (ib, ib + 1))[node["own"].index(codes[node_id]), 0]

        def direct(wire):
            feeder = producer.get(wire)
            if feeder is None:
                return exact[wire]
            ins = [cone_value(dfg, exact, x, codes[producer[x].id]) if x in producer else exact[x] for x in feeder.inputs]
            return run_unit(feeder.op, codes[feeder.id], *ins)

        a, b = (direct(w) for w in node["inputs"])
        own = (run_unit("add", codes[node_id], a, b) - (a + b)).astype(np.float64)
        d = ((a - exact[node["inputs"][0]]) + (b - exact[node["inputs"][1]])).astype(np.float64)
        ref = np.array([own.mean(), (own ** 2).mean(), (d * own).mean(), d.mean()])
        rel = np.max(np.abs(stored - ref) / np.maximum(np.abs(ref), 1e-9))
        worst = max(worst, rel)
        log(f"check {node_id} own={codes[node_id]} ia={ia} ib={ib}: max relative difference {rel:.2e}")
    log(f"check done: worst relative difference {worst:.2e}")
    return worst


def main():
    args = sys.argv[1:]
    if not args or args[0].startswith("--"):
        sys.exit(__doc__)
    benchmark = args[0]
    opt = lambda flag, default: int(args[args.index(flag) + 1]) if flag in args else default
    workers = opt("--workers", max(1, int(os.cpu_count() * 0.8)))
    n = opt("--samples", 200_000)

    log(f"{benchmark}: preparing histories ({n:,} samples)")
    dfg, meta, meta_path, work_dir = prepare(benchmark, n)
    out_dir = str(meta_path.parent)
    if "--check" in args:
        worst = check(benchmark, n, opt("--check", 20), meta, work_dir)
        sys.exit(0 if worst < 1e-9 else 1)

    tasks, total_entries = [], 0
    for node_id, node in meta["nodes"].items():
        done = np.load(os.path.join(out_dir, f"{node_id}_done.npy"))
        tasks += [(node_id, int(i)) for i in np.flatnonzero(~done)]
        shape = np.load(os.path.join(out_dir, f"{node_id}.npy"), mmap_mode="r").shape
        total_entries += int(np.prod(shape[:3]))
    tasks.sort(key=lambda t: np.load(os.path.join(out_dir, f"{t[0]}.npy"), mmap_mode="r").shape[2])
    log(f"{len(meta['nodes'])} adder nodes, {total_entries:,} entries, {len(tasks):,} tasks to do, {workers} workers")

    start = time.time()
    with Pool(workers, initializer=init_worker, initargs=(str(meta_path), str(work_dir))) as pool:
        for k, (node_id, ia) in enumerate(pool.imap_unordered(run_task, tasks), 1):
            if k % 10 == 0 or k == len(tasks):
                elapsed = time.time() - start
                eta = elapsed / k * (len(tasks) - k)
                log(f"{k:,}/{len(tasks):,} tasks   elapsed {elapsed / 3600:.2f} h   eta {eta / 3600:.2f} h")

    for node_id in meta["nodes"]:
        if not np.load(os.path.join(out_dir, f"{node_id}_done.npy")).all():
            sys.exit(f"{node_id}: incomplete")
    shutil.rmtree(work_dir)
    log(f"finished: table in {out_dir}")


if __name__ == "__main__":
    main()
