"""
Node and feeder tables: each unit's own error measured on the operands that reach every node,
and, for adders, behind every pair of units that can feed it.

    python3 -m characterization.characterize <benchmark> [samples]

Writes data/node_metrics/<benchmark>.json and data/node_metrics/<benchmark>_feeder.json.
"""
import json
import sys
from concurrent.futures import ThreadPoolExecutor

import numpy as np

from model.data import DATA, load_benchmark
from sim.simulate import EXACT, UNITS, run_unit, sample_inputs

SEED = 1


def exact_values(dfg, inputs):
    values = dict(inputs)
    for node in dfg.nodes:
        a, b = node.inputs
        values[node.output] = run_unit(node.op, EXACT[node.op], values[a], values[b])
    return values


def moments(error, damage=None):
    error = error.astype(np.float64)
    if damage is None:
        return {"me": float(error.mean()), "mse": float((error ** 2).mean())}
    damage = damage.astype(np.float64)
    return [float(error.mean()), float((error ** 2).mean()), float((damage * error).mean()), float(damage.mean())]


def node_table(dfg, exact):
    table = {}
    for node in dfg.nodes:
        a, b = (exact[w] for w in node.inputs)
        table[node.id] = {
            str(code): moments(run_unit(node.op, code, a, b) - exact[node.output]) for code in UNITS[node.op]
        }
    return table


def feeder_values(dfg, exact, wire):
    """Value of `wire` for every unit its producer can take, with all same-op nodes upstream using that unit."""
    producer = {node.output: node for node in dfg.nodes}
    if wire not in producer:
        return {"-": exact[wire]}
    op = producer[wire].op

    def value(w, code, memo):
        node = producer.get(w)
        if node is None:
            return exact[w]
        if w not in memo:
            a, b = (value(x, code, memo) for x in node.inputs)
            memo[w] = run_unit(node.op, code if node.op == op else EXACT[node.op], a, b)
        return memo[w]

    return {str(code): value(wire, code, {}) for code in [EXACT[op]] + UNITS[op]}


def feeder_table(dfg, exact):
    adders = [node for node in dfg.nodes if node.op == "add"]
    wires = {w for node in adders for w in node.inputs}
    feeds = dict(zip(wires, ThreadPoolExecutor().map(lambda w: feeder_values(dfg, exact, w), wires)))

    def entries(node):
        wire_a, wire_b = node.inputs
        out = {}
        for fa, a in feeds[wire_a].items():
            for fb, b in feeds[wire_b].items():
                damage = (a - exact[wire_a]) + (b - exact[wire_b])
                for code in UNITS["add"]:
                    out[f"{code}|{fa}|{fb}"] = moments(run_unit("add", code, a, b) - (a + b), damage)
        return out

    return dict(zip((n.id for n in adders), ThreadPoolExecutor().map(entries, adders)))


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    benchmark = sys.argv[1]
    n = int(sys.argv[2]) if len(sys.argv) > 2 else 200_000

    dfg = load_benchmark(benchmark)
    exact = exact_values(dfg, sample_inputs(dfg, n, SEED))
    out_dir = DATA / "node_metrics"
    out_dir.mkdir(exist_ok=True)
    (out_dir / f"{benchmark}.json").write_text(json.dumps(node_table(dfg, exact)))
    (out_dir / f"{benchmark}_feeder.json").write_text(json.dumps(feeder_table(dfg, exact)))
    print(f"{benchmark}: node tables written to {out_dir.relative_to(DATA.parent)}")


if __name__ == "__main__":
    main()
