#!/usr/bin/env python3
"""
Analytic-only runner: estimate a DeepApprox config's error with the paper model.

    python3 run.py <benchmark> <backend> <config_file> [sample_file]
    python3 run.py fir systemc benchmarks/fir/configs/sc_cfg2.txt
    python3 run.py fir systemc benchmarks/fir/configs/sc_cfg2.txt --validate   # also run sim

Prints ME, MSE, SNR, PSNR from the analytic model. With --validate it also runs
the Monte Carlo ground truth (validate.py) and shows inaccuracy vs it.
"""
import os, sys, ast, math, importlib.util, time

_ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(_ROOT, "core"))

import numpy as np
from model import InputStats, analytic_propagate


def _load_spec(benchmark):
    p = os.path.join(_ROOT, "benchmarks", benchmark, "spec.py")
    if not os.path.exists(p):
        sys.exit(f"no spec.py for benchmark '{benchmark}' ({p})")
    sys.path.insert(0, os.path.join(_ROOT, "benchmarks", benchmark))
    s = importlib.util.spec_from_file_location(f"{benchmark}_spec", p)
    m = importlib.util.module_from_spec(s); s.loader.exec_module(m)
    return m


def _default_sample(benchmark):
    return os.path.join(_ROOT, "benchmarks", benchmark, "data", "sample.txt")


def _parse_config(path, lmap):
    txt = open(path).read().strip()
    if txt.lower().startswith("conf_path:"):
        txt = txt.split(":", 1)[1].strip()
    conf = ast.literal_eval(txt)
    assign = {}
    for line, spec in conf.items():
        node = lmap[int(line)]
        approx = spec.split("-", 1)[1] if "-" in spec else spec
        if approx and not approx.endswith("exact"):
            assign[node] = approx
    return assign


def _psnr(mse):
    return 100.0 if mse <= 0 else abs(20 * math.log10(255) - 10 * math.log10(mse))


def main():
    args = [a for a in sys.argv[1:] if a != "--validate"]
    do_validate = "--validate" in sys.argv
    if len(args) < 3:
        sys.exit(__doc__)
    benchmark, backend, cfg_path = args[0], args[1], args[2]
    sample_path = args[3] if len(args) > 3 else _default_sample(benchmark)

    spec = _load_spec(benchmark)
    if backend not in spec.BACKENDS:
        sys.exit(f"backend '{backend}' not in spec (have: {list(spec.BACKENDS)})")

    samples = np.array([int(l) for l in open(sample_path) if l.strip()], dtype=np.int64)
    circuit = spec.build_circuit(backend)
    stats = InputStats(circuit, {spec.INPUTS[0]: samples})
    assign = _parse_config(cfg_path, spec.line_map(backend))

    order = spec.node_order(backend)
    print(f"benchmark={benchmark}  backend={backend}  samples={len(samples)}")
    print("config:", {n: assign.get(n, "exact") for n in order})

    _t0 = time.perf_counter()
    ana = analytic_propagate(stats, assign)
    _elapsed = time.perf_counter() - _t0

    print("\nANALYTIC")
    print(f"   ME   = {ana['ME']:18,.2f}")
    print(f"   MSE  = {ana['MSE']:18,.2f}")
    print(f"   SNR  = {ana['SNR']:8.3f} dB")
    print(f"   PSNR = {_psnr(ana['MSE']):8.3f}")
    print(f"   time = {_elapsed*1000:8.2f} ms  ({_elapsed:.4f} s)")


if __name__ == "__main__":
    main()
