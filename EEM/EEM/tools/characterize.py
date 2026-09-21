#!/usr/bin/env python3
"""
characterize.py -- one-time precompute of multiplier LUTs for a benchmark.

For each approximate multiplier unit and each COEFFICIENT the benchmark uses,
tabulate unit(coeff, x) for every x in the input range, and persist it. This is
the PMF-INDEPENDENT part (the paper's pre-representation feature library): it
depends only on the unit and the coefficient, never on any config or input
distribution. The PMF-weighting (turning these outputs into Dr and output PMFs)
stays in model.build and is done per config.

Adders are NOT precomputed here: both operands vary over a wide domain, so there
is no small complete table -- their LUT fills in lazily at runtime (see
unit_lut.lut_eval). This mirrors the paper (LUT for multipliers, bit-analysis
for adders).

Usage:
    EEM_LUT=1 python3 tools/characterize.py fir
"""
import os, sys, importlib.util
import numpy as np

_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(_ROOT, "core"))

os.environ["EEM_LUT"] = "1"   # ensure the LUT layer is active for persistence
import unit_lut


# multiplier units the FIR configs use (extend as needed)
MUL_UNITS = ["mul16u_5FA", "mul16u_8VH", "mul16u_AQ1", "mul16u_DAE",
             "mul16u_F6B", "mul16u_GPF", "mul16u_HGP", "mul16u_CK3"]


def _load_spec(benchmark):
    p = os.path.join(_ROOT, "benchmarks", benchmark, "spec.py")
    sys.path.insert(0, os.path.join(_ROOT, "benchmarks", benchmark))
    s = importlib.util.spec_from_file_location(f"{benchmark}_spec", p)
    m = importlib.util.module_from_spec(s); s.loader.exec_module(m)
    return m


def _input_range(benchmark):
    """Distinct input values actually present in the benchmark's sample.txt --
    tabulate exactly those (no need to cover unused values)."""
    sp = os.path.join(_ROOT, "benchmarks", benchmark, "data", "sample.txt")
    x = np.array([int(l) for l in open(sp) if l.strip()], dtype=np.int64)
    return np.unique(x)


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: EEM_LUT=1 python3 tools/characterize.py <benchmark>")
    benchmark = sys.argv[1]
    spec = _load_spec(benchmark)
    coeffs = sorted(set(spec.COEFFS))
    x_vals = _input_range(benchmark)
    print(f"benchmark={benchmark}  coeffs={coeffs}  distinct inputs={len(x_vals)}")

    total = 0
    for unit in MUL_UNITS:
        try:
            for c in coeffs:
                unit_lut.precompute_mul(unit, c, x_vals)
                total += len(x_vals)
        except Exception as e:
            print(f"  skip {unit}: {e}")
            continue
        print(f"  tabulated {unit} for coeffs {coeffs}")
    unit_lut.save_all()
    print(f"done: {total:,} (unit,coeff,x) entries written to core/lut_cache/")


if __name__ == "__main__":
    main()
