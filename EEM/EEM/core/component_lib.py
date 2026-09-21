"""
ctypes bridge to the approximate/exact units compiled into engine.so.
engine.so is located relative to THIS file (core/engine/engine.so), so the
runner works from any working directory.
"""
import os
import ctypes
import numpy as np

_HERE = os.path.dirname(os.path.abspath(__file__))
_SO = os.path.join(_HERE, "engine", "engine.so")
if not os.path.exists(_SO):
    raise FileNotFoundError(
        f"engine.so not found at {_SO}\n"
        f"Build it: g++ -O2 -shared -fPIC core/engine/engine.cpp "
        f"core/engine/components.cpp -o core/engine/engine.so")
_lib = ctypes.CDLL(_SO)

_lib.run_mul.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_uint16),
                         ctypes.POINTER(ctypes.c_uint16), ctypes.POINTER(ctypes.c_uint32),
                         ctypes.c_long]
_lib.run_add.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_uint64),
                         ctypes.POINTER(ctypes.c_uint64), ctypes.POINTER(ctypes.c_uint64),
                         ctypes.c_long]

def _call_unit_raw(op, unit, a, b):
    """Direct engine.so call. Always correct; no caching."""
    a = np.ascontiguousarray(a); b = np.ascontiguousarray(b)
    n = len(a)
    if op == "mul":
        a16 = a.astype(np.uint16); b16 = b.astype(np.uint16)
        out = np.empty(n, dtype=np.uint32)
        _lib.run_mul(unit.encode(),
                     a16.ctypes.data_as(ctypes.POINTER(ctypes.c_uint16)),
                     b16.ctypes.data_as(ctypes.POINTER(ctypes.c_uint16)),
                     out.ctypes.data_as(ctypes.POINTER(ctypes.c_uint32)), n)
        return out.astype(np.int64)
    else:
        a64 = a.astype(np.uint64); b64 = b.astype(np.uint64)
        out = np.empty(n, dtype=np.uint64)
        _lib.run_add(unit.encode(),
                     a64.ctypes.data_as(ctypes.POINTER(ctypes.c_uint64)),
                     b64.ctypes.data_as(ctypes.POINTER(ctypes.c_uint64)),
                     out.ctypes.data_as(ctypes.POINTER(ctypes.c_uint64)), n)
        return out.astype(np.int64)


# --------------------------------------------------------------------------
# Optional transparent cache.
#
# A unit's output is a pure function of (op, unit, a, b), so results can be
# memoized and reused across configs. The cache is keyed by the DISTINCT input
# pairs seen; on a repeat pair it returns the stored output instead of calling
# engine.so. This does NOT change any result — call_unit(op,unit,a,b) returns
# exactly what _call_unit_raw would. It only avoids recomputation.
#
# Enable/disable with EEM_CACHE=1/0 (default on). Persisted per-unit to
# <core>/cache/<op>_<unit>.npz so the engine.so work happens once, ever.
# --------------------------------------------------------------------------
# Default OFF: for the analytic path the unit is already only evaluated over its
# operand PMF (a small distinct-value set), so engine.so is not the bottleneck and
# memoization adds net overhead. The cache is kept, correct, and opt-in via
# EEM_CACHE=1 -- useful only when a workload re-evaluates the SAME large (a,b)
# sets many times (e.g. repeated full-sample SIM runs). Never changes results.
_CACHE_ON = os.environ.get("EEM_CACHE", "0") == "1"
_CACHE_DIR = os.path.join(_HERE, "cache")
_mem = {}   # (op, unit) -> dict{ packed_key(int64) : out(int64) }  as numpy sorted arrays

def _cache_path(op, unit):
    return os.path.join(_CACHE_DIR, f"{op}__{unit}.npz")

def _pack(a, b):
    # pack two <=32-bit non-negative ints into one int64 key, vectorized
    return (a.astype(np.int64) << np.int64(32)) | b.astype(np.int64)

def _load_unit_cache(op, unit):
    key = (op, unit)
    if key in _mem:
        return _mem[key]
    keys = np.empty(0, dtype=np.int64); outs = np.empty(0, dtype=np.int64)
    p = _cache_path(op, unit)
    if os.path.exists(p):
        z = np.load(p)
        keys, outs = z["keys"], z["out"]
    _mem[key] = [keys, outs]
    return _mem[key]

def _save_unit_cache(op, unit):
    if not _CACHE_ON:
        return
    keys, outs = _mem.get((op, unit), [None, None])
    if keys is None or len(keys) == 0:
        return
    os.makedirs(_CACHE_DIR, exist_ok=True)
    np.savez(_cache_path(op, unit), keys=keys, out=outs)

def call_unit(op, unit, a, b):
    """Transparent wrapper: identical output to _call_unit_raw, memoized.

    Vectorized: known (a,b) pairs are resolved by a single sorted-array lookup
    (searchsorted); only genuinely-new pairs hit engine.so. Guaranteed to return
    exactly _call_unit_raw's result. With EEM_CACHE=0 it is a pure passthrough,
    so behaviour is provably unchanged."""
    if not _CACHE_ON:
        return _call_unit_raw(op, unit, a, b)

    a = np.ascontiguousarray(a).astype(np.int64)
    b = np.ascontiguousarray(b).astype(np.int64)
    q = _pack(a, b)

    entry = _load_unit_cache(op, unit)
    keys, outs = entry

    out = np.empty(len(a), dtype=np.int64)
    if len(keys):
        pos = np.searchsorted(keys, q)
        pos_clip = np.clip(pos, 0, len(keys) - 1)
        hit = keys[pos_clip] == q
        out[hit] = outs[pos_clip[hit]]
    else:
        hit = np.zeros(len(a), dtype=bool)

    if not hit.all():
        miss = ~hit
        computed = _call_unit_raw(op, unit, a[miss], b[miss])
        out[miss] = computed
        # merge new distinct pairs into the sorted cache
        new_keys = q[miss]
        uniq_k, uidx = np.unique(new_keys, return_index=True)
        uniq_o = computed[uidx]
        if len(keys):
            allk = np.concatenate([keys, uniq_k]); allo = np.concatenate([outs, uniq_o])
        else:
            allk, allo = uniq_k, uniq_o
        order = np.argsort(allk, kind="stable")
        allk, allo = allk[order], allo[order]
        dedup = np.concatenate([[True], allk[1:] != allk[:-1]])
        entry[0] = allk[dedup]; entry[1] = allo[dedup]
        _save_unit_cache(op, unit)
    return out

def exact(op, a, b):
    if op == "mul": return (a.astype(np.int64) * b.astype(np.int64))
    return (a.astype(np.int64) + b.astype(np.int64))
