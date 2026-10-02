import subprocess
from pathlib import Path

import numpy as np

BIN = Path(__file__).resolve().parent / "bin"
EXACT = {"add": 0, "mul": 11}
UNITS = {"add": list(range(2, 11)), "mul": list(range(13, 22))}


def run_unit(op, code, a, b):
    if code == EXACT[op]:
        return a + b if op == "add" else a * b
    if op == "add":
        argv = [BIN / "inexact_adder_batch_16u", str(code)]
    else:
        argv = [BIN / f"inexact_mul_batch_{code}"]
    pairs = np.column_stack((a, b)).astype(np.uint64).tobytes()
    out = subprocess.run(argv, input=pairs, capture_output=True, check=True).stdout
    return np.frombuffer(out, dtype=np.uint64).astype(np.int64)


def sample_inputs(dfg, n, seed):
    rng = np.random.default_rng(seed)
    values = {}
    for name in dfg.inputs:
        lo, hi = (int(v) for v in dfg.input_ranges[name])
        values[name] = np.full(n, lo, dtype=np.int64) if lo == hi else rng.integers(lo, hi + 1, n, dtype=np.int64)
    return values


def simulate(dfg, codes, inputs):
    exact, approx = dict(inputs), dict(inputs)
    for node in dfg.nodes:
        a, b = node.inputs
        exact[node.output] = run_unit(node.op, EXACT[node.op], exact[a], exact[b])
        approx[node.output] = run_unit(node.op, codes.get(node.id, EXACT[node.op]), approx[a], approx[b])
    return exact[dfg.output], approx[dfg.output]
