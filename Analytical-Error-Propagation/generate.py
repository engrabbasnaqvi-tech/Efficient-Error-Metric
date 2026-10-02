"""
Random configurations and their simulated ground truth.

    python3 generate.py <benchmark> [configs] [samples]
    python3 generate.py --fixed <file> [samples]

Draws random configurations until each RMS band between LOW and HIGH percent
holds one, simulates them with sim/bin and writes data/ground_truth/<benchmark>.json.
With --fixed, simulates the configurations listed in <file> instead and writes
data/ground_truth/<file name>.json.
"""
import json
import math
import sys
import time
from pathlib import Path

import numpy as np

from model.data import DATA, load_benchmark, read_json
from model.propagate import compute_wire_max
from sim.simulate import EXACT, UNITS, sample_inputs, simulate

LOW, HIGH = 0.5, 10.0
MAX_DRAWS = 2000
SEED_CONFIGS, SEED_INPUTS = 7, 42


def random_config(dfg, rng):
    density = rng.uniform(0.1, 0.9)
    return {
        node.id: int(rng.choice(UNITS[node.op])) if rng.random() < density else EXACT[node.op]
        for node in dfg.nodes
    }


def ground_truth(dfg, codes, inputs, m_out):
    start = time.perf_counter()
    exact, approx = simulate(dfg, codes, inputs)
    seconds = time.perf_counter() - start
    error = (approx - exact).astype(np.float64)
    mse = float((error ** 2).mean())
    return {
        "codes": codes,
        "me": float(error.mean()),
        "mse": mse,
        "rms_pct": math.sqrt(mse) / m_out * 100,
        "seconds": seconds,
    }


def random_configs(dfg, inputs, m_out, count):
    edges = np.geomspace(LOW, HIGH, count + 1)
    rng = np.random.default_rng(SEED_CONFIGS)
    slots = [None] * count
    for _ in range(MAX_DRAWS):
        if all(slots):
            break
        truth = ground_truth(dfg, random_config(dfg, rng), inputs, m_out)
        band = int(np.searchsorted(edges, truth["rms_pct"])) - 1
        if 0 <= band < count and slots[band] is None:
            slots[band] = truth
    if not all(slots):
        sys.exit(f"found {sum(map(bool, slots))} of {count} configurations in {MAX_DRAWS} draws")
    return [{"name": f"C{i + 1}", **truth} for i, truth in enumerate(slots)]


def fixed_configs(dfg, inputs, m_out, listed):
    return [{"name": name, **ground_truth(dfg, codes, inputs, m_out)} for name, codes in listed.items()]


def main():
    args = sys.argv[1:]
    fixed = None
    if "--fixed" in args:
        i = args.index("--fixed")
        fixed = Path(args[i + 1])
        del args[i:i + 2]
    elif not args:
        sys.exit(__doc__)

    if fixed:
        spec = read_json(fixed)
        benchmark, name = spec["benchmark"], fixed.stem
        n = int(args[0]) if args else 200_000
    else:
        benchmark = name = args[0]
        count = int(args[1]) if len(args) > 1 else 3
        n = int(args[2]) if len(args) > 2 else 200_000

    dfg = load_benchmark(benchmark)
    m_out = compute_wire_max(dfg)[dfg.output]
    inputs = sample_inputs(dfg, n, SEED_INPUTS)
    if fixed:
        configs = fixed_configs(dfg, inputs, m_out, spec["configs"])
    else:
        configs = random_configs(dfg, inputs, m_out, count)

    out = DATA / "ground_truth" / f"{name}.json"
    out.parent.mkdir(exist_ok=True)
    out.write_text(json.dumps({"benchmark": benchmark, "samples": n, "seed": SEED_INPUTS,
                               "m_out": m_out, "configs": configs}, indent=1))
    for c in configs:
        print(f"{name} {c['name']}: RMS {c['rms_pct']:.3f} %")
    print(f"written to {out.relative_to(DATA.parent)}")


if __name__ == "__main__":
    main()
