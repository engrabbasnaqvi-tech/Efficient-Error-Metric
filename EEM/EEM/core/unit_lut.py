"""
Unit-output LUT (paper's "pre-representation feature library").

Caches the APPROXIMATE OUTPUT of each unit for each input pair -- NOT Dr, and
NOT any PMF-weighted quantity. From the cached output the model derives both:
  * Dr  = approx_out - exact_out
  * the output PMF (distribution of approx_out)
so caching the output (rather than Dr) is what lets the LUT serve both needs.

Why output and not Dr: Dr at a node depends on the arriving distribution (config-
dependent), so it can't be cached per node. The unit's OUTPUT for a given (a,b)
is pure (config-independent), so it can. The PMF-weighting stays in model.build.

lut_eval(op, unit, a, b) returns exactly what component_lib.call_unit returns.
On a cache miss it computes via call_unit, stores, and (optionally) persists.
So results are bit-identical to the non-LUT path; only recomputation is avoided.

Multipliers: when one operand is constant (the FIR case), the table is 1-D over
the varying operand -- fully precomputable by characterize.py. Adders: both
operands vary over a large domain, so the cache is a lazy memo that fills in as
pairs are seen (mirrors the paper using a LUT for muls, bit-analysis for adds).

Enable with EEM_LUT=1 (default off, since a single-config run does not benefit;
the win is across many configs / a DSE sweep -- see tools/bench_lut.py).
"""
import os
import numpy as np
from component_lib import call_unit

_LUT_ON = os.environ.get("EEM_LUT", "0") == "1"
_HERE = os.path.dirname(os.path.abspath(__file__))
_LUT_DIR = os.path.join(_HERE, "lut_cache")

# in-memory: (op, unit) -> [sorted packed keys (int64), outputs (int64)]
_LUT = {}
_DIRTY = set()


def _pack(a, b):
    return (a.astype(np.int64) << np.int64(32)) | (b.astype(np.int64) & np.int64(0xFFFFFFFF))


def _path(op, unit):
    return os.path.join(_LUT_DIR, f"{op}__{unit}.npz")


def _load(op, unit):
    key = (op, unit)
    if key in _LUT:
        return _LUT[key]
    keys = np.empty(0, dtype=np.int64); outs = np.empty(0, dtype=np.int64)
    p = _path(op, unit)
    if os.path.exists(p):
        z = np.load(p); keys, outs = z["keys"], z["out"]
    _LUT[key] = [keys, outs]
    return _LUT[key]


def save_all():
    """Persist any tables modified this session."""
    if not _DIRTY:
        return
    os.makedirs(_LUT_DIR, exist_ok=True)
    for (op, unit) in list(_DIRTY):
        keys, outs = _LUT[(op, unit)]
        if len(keys):
            np.savez(_path(op, unit), keys=keys, out=outs)
    _DIRTY.clear()


def _merge(op, unit, new_keys, new_outs):
    entry = _LUT[(op, unit)]
    keys, outs = entry
    if len(keys):
        allk = np.concatenate([keys, new_keys]); allo = np.concatenate([outs, new_outs])
    else:
        allk, allo = new_keys, new_outs
    order = np.argsort(allk, kind="stable")
    allk, allo = allk[order], allo[order]
    keep = np.concatenate([[True], allk[1:] != allk[:-1]])
    entry[0], entry[1] = allk[keep], allo[keep]
    _DIRTY.add((op, unit))


def lut_eval(op, unit, a, b):
    """Identical to call_unit(op,unit,a,b); memoized. Misses computed once."""
    if not _LUT_ON:
        return call_unit(op, unit, a, b)
    a = np.ascontiguousarray(a).astype(np.int64)
    b = np.ascontiguousarray(b).astype(np.int64)
    q = _pack(a, b)
    keys, outs = _load(op, unit)

    out = np.empty(len(a), dtype=np.int64)
    if len(keys):
        pos = np.clip(np.searchsorted(keys, q), 0, len(keys) - 1)
        hit = keys[pos] == q
        out[hit] = outs[pos[hit]]
    else:
        hit = np.zeros(len(a), dtype=bool)

    if not hit.all():
        miss = ~hit
        computed = call_unit(op, unit, a[miss], b[miss])
        out[miss] = computed
        uk, ui = np.unique(q[miss], return_index=True)
        _merge(op, unit, uk, computed[ui])
    return out


def precompute_mul(unit, coeff, x_values):
    """Fully tabulate a multiplier with a CONSTANT operand: unit(coeff, x) for
    every x in x_values. This is the complete, PMF-independent precompute
    characterize.py performs for the FIR's constant-coefficient multipliers.
    Note the operand order matches how the benchmark calls the unit; the FIR
    (SystemC) uses mul(coeff, sample), so a=coeff, b=x."""
    a = np.full(len(x_values), coeff, dtype=np.int64)
    b = np.asarray(x_values, dtype=np.int64)
    outs = call_unit("mul", unit, a, b)
    _load("mul", unit)
    _merge("mul", unit, _pack(a, b), outs)
