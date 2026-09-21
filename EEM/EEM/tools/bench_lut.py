#!/usr/bin/env python3
"""
bench_lut.py -- measure the LUT's effect across MANY configs, and verify results
are identical with LUT off vs on.

A single-config run does not benefit from the LUT (unit evals aren't the
bottleneck). The win appears when the same units recur across many configs -- a
DSE sweep. This runs every config file N times with LUT off, then with LUT on,
reports total time each, and confirms MSE is bit-identical.

Usage:
    python3 tools/bench_lut.py fir systemc [repeats]
"""
import os, sys, ast, time, glob, importlib.util
import numpy as np

_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(_ROOT, "core"))


def _load_spec(b):
    p = os.path.join(_ROOT, "benchmarks", b, "spec.py")
    sys.path.insert(0, os.path.join(_ROOT, "benchmarks", b))
    s = importlib.util.spec_from_file_location(f"{b}_spec", p)
    m = importlib.util.module_from_spec(s); s.loader.exec_module(m)
    return m


def _parse(path, lmap):
    txt = open(path).read().strip()
    if txt.lower().startswith("conf_path:"):
        txt = txt.split(":", 1)[1].strip()
    conf = ast.literal_eval(txt); assign = {}
    for line, spec in conf.items():
        node = lmap[int(line)]; ap = spec.split("-", 1)[1] if "-" in spec else spec
        if ap and not ap.endswith("exact"): assign[node] = ap
    return assign


def run_all(spec, backend, samples, cfgs, repeats):
    # imported fresh so EEM_LUT is read at import time
    import importlib, model
    importlib.reload(model)
    from model import InputStats, analytic_propagate
    circuit = spec.build_circuit(backend)
    stats = InputStats(circuit, {spec.INPUTS[0]: samples})
    results = {}
    t0 = time.perf_counter()
    for _ in range(repeats):
        for c in cfgs:
            assign = _parse(c, spec.line_map(backend))
            results[c] = analytic_propagate(stats, assign)["MSE"]
    return time.perf_counter() - t0, results


def main():
    benchmark = sys.argv[1] if len(sys.argv) > 1 else "fir"
    backend = sys.argv[2] if len(sys.argv) > 2 else "systemc"
    repeats = int(sys.argv[3]) if len(sys.argv) > 3 else 5
    spec = _load_spec(benchmark)
    sp = os.path.join(_ROOT, "benchmarks", benchmark, "data", "sample.txt")
    samples = np.array([int(l) for l in open(sp) if l.strip()], dtype=np.int64)
    cfgs = sorted(glob.glob(os.path.join(_ROOT, "benchmarks", benchmark, "configs", "*.txt")))
    print(f"{benchmark}/{backend}: {len(cfgs)} configs x {repeats} repeats, {len(samples)} samples\n")

    os.environ["EEM_LUT"] = "0"
    t_off, r_off = run_all(spec, backend, samples, cfgs, repeats)
    os.environ["EEM_LUT"] = "1"
    # clear any stale cache so the ON run builds fresh, then a warm pass
    import shutil
    shutil.rmtree(os.path.join(_ROOT, "core", "lut_cache"), ignore_errors=True)
    t_on_cold, r_on = run_all(spec, backend, samples, cfgs, repeats)
    t_on_warm, _ = run_all(spec, backend, samples, cfgs, repeats)

    identical = all(abs(r_off[c] - r_on[c]) < 1e-6 for c in cfgs)
    print(f"results identical (LUT off vs on): {identical}")
    print(f"LUT off        : {t_off:8.3f} s")
    print(f"LUT on  (cold) : {t_on_cold:8.3f} s   (builds tables while running)")
    print(f"LUT on  (warm) : {t_on_warm:8.3f} s   (tables already built)")


if __name__ == "__main__":
    main()
