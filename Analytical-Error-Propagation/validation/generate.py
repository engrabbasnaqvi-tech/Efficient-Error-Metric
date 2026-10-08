"""
Random configurations and their simulated ground truth.

    python3 -m validation.generate <benchmark> [configs] [samples]
    python3 -m validation.generate --fixed <file or folder> [samples]
"""
import ast
import json
import math
import re
import sys
import time
from pathlib import Path

import numpy as np

from model.data import DATA, load_benchmark, read_json
from model.propagate import compute_wire_max
from sim.simulate import EXACT, SEED_INPUTS, UNITS, random_config, sample_inputs, simulate

LOW, HIGH = 0.5, 10.0
MAX_DRAWS = 2000
SEED_CONFIGS = 7

DEEPAPPROX_LINES = {
    **{63 + i: f"mult_{i}" for i in range(9)},
    **{85 + i: f"adder_{i}" for i in range(4)},
    **{91 + i: f"adder_{4 + i}" for i in range(4)},
}
EXACT_UNITS = {"mul16u_BMC"}


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


def deepapprox_configs(folder, dfg):
    codes_by_name = {e["name"]: int(code) for code, e in read_json(DATA / "final_library.json").items()}
    ops = {node.id: node.op for node in dfg.nodes}
    files = sorted(folder.glob("*.txt"), key=lambda f: [int(t) if t.isdigit() else t for t in re.split(r"(\d+)", f.stem)])
    if not files:
        sys.exit(f"no .txt configurations in {folder}")
    configs = {}
    for f in files:
        codes = {node_id: EXACT[op] for node_id, op in ops.items()}
        for line, unit in ast.literal_eval(f.read_text()).items():
            node_id, unit = DEEPAPPROX_LINES.get(line), unit.split("-")[-1]
            if node_id is None or unit not in codes_by_name:
                sys.exit(f"{f.name}: unknown line {line} or unit {unit}")
            code = EXACT[ops[node_id]] if unit in EXACT_UNITS else codes_by_name[unit]
            if code != EXACT[ops[node_id]] and code not in UNITS[ops[node_id]]:
                sys.exit(f"{f.name}: unit {unit} does not fit {node_id}")
            codes[node_id] = code
        configs[f.stem] = codes
    return configs


def main():
    args = sys.argv[1:]
    fixed = None
    if "--fixed" in args:
        i = args.index("--fixed")
        fixed = Path(args[i + 1])
        del args[i:i + 2]
    elif not args:
        sys.exit(__doc__)

    if fixed and fixed.is_dir():
        benchmark, name = "fir", f"fir_{fixed.name}"
        spec = {"configs": deepapprox_configs(fixed, load_benchmark(benchmark))}
        n = int(args[0]) if args else 200_000
    elif fixed:
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
    for c in configs if len(configs) <= 20 else []:
        print(f"{name} {c['name']}: RMS {c['rms_pct']:.3f} %")
    if len(configs) > 20:
        rms = sorted(c["rms_pct"] for c in configs)
        print(f"{name}: {len(configs)} configurations, RMS {rms[0]:.3f} to {rms[-1]:.3f} %, median {rms[len(rms) // 2]:.3f} %")
    print(f"written to {out.relative_to(DATA.parent)}")


if __name__ == "__main__":
    main()
