import math
from typing import Dict, Optional, Tuple
from .models import BenchmarkDFG, ComponentVariant

_ADDER_CANONICAL_RANGES = [2047, 4095, 8191, 16383, 32767, 65535]
_MUL_CANONICAL_RANGES = [255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535]


def scalar_rms(wire_state: Dict[str, float], m_out: float = 1.0) -> float:
    """RMS error as a percentage of the maximum output value."""
    return math.sqrt(max(wire_state["mse"], 0.0)) / max(m_out, 1.0) * 100.0


def compute_wire_max(dfg: BenchmarkDFG) -> Dict[str, float]:
    """Worst-case value of every wire."""
    wire_max: Dict[str, float] = {}
    for inp in dfg.inputs:
        lo, hi = dfg.input_ranges.get(inp, (0.0, 0.0))
        wire_max[inp] = max(abs(float(lo)), abs(float(hi)))
    for node in dfg.nodes:
        max_a = wire_max.get(node.inputs[0], 65535.0)
        max_b = wire_max.get(node.inputs[1], 65535.0)
        wire_max[node.output] = max_a * max_b if node.op == "mul" else max_a + max_b
    return wire_max


def _range_entry(range_lib, name, ranges, max_input):
    per_range = range_lib.get(name) if range_lib is not None else None
    if per_range is None:
        return None
    key = next((str(r) for r in ranges if r >= max_input), "65535")
    return per_range.get(key)


def _node_scalars(
    component_lib: Dict[int, ComponentVariant],
    code: int,
    adder_range_lib: Optional[Dict] = None,
    mul_range_lib: Optional[Dict] = None,
    max_input: float = 65535.0,
) -> Tuple[float, float]:
    """Own error (me, mse) of a unit from the range-bucketed libraries."""
    cv = component_lib.get(code)
    if cv is None:
        return 0.0, 0.0
    if cv.op == "add":
        entry = _range_entry(adder_range_lib, cv.name, _ADDER_CANONICAL_RANGES, max_input)
    else:
        entry = _range_entry(mul_range_lib, cv.name, _MUL_CANONICAL_RANGES, max_input)
    if entry is not None:
        return float(entry["me"]), float(entry["mse"])
    return cv.me, cv.error


def _input_wire_state(lo: float, hi: float) -> Dict[str, float]:
    """Uniform input over [lo, hi] without error."""
    ex = (lo + hi) / 2.0
    ex2 = (lo * lo + lo * hi + hi * hi) / 3.0
    return {"me": 0.0, "mse": 0.0, "ex": ex, "ex2": ex2}


def _multiplier_output_state(
    ws_a: Dict[str, float],
    ws_b: Dict[str, float],
    me_r: float,
    mse_r: float,
) -> Dict[str, float]:
    """
    Eq 13.
      E[c_exa]  = E[a_exa] · E[b_exa]
      E[c²_exa] = E[a²_exa] · E[b²_exa]
      E[Δc]     = E[Δa]·E[b_exa] + E[Δb]·E[a_exa] + E[Δr]
      E[Δc²]    = E[Δa²]·E[b²_exa] + E[Δb²]·E[a²_exa] + E[Δr²]
                + 2·E[Δa]·E[b_exa]·E[Δb]·E[a_exa]
                + 2·E[b_exa]·E[Δa]·E[Δr] + 2·E[a_exa]·E[Δb]·E[Δr]
    """
    me_a, mse_a, ex_a, ex2_a = ws_a["me"], ws_a["mse"], ws_a["ex"], ws_a["ex2"]
    me_b, mse_b, ex_b, ex2_b = ws_b["me"], ws_b["mse"], ws_b["ex"], ws_b["ex2"]
    me_c = me_a * ex_b + me_b * ex_a + me_r
    mse_c = (
        mse_a * ex2_b
        + mse_b * ex2_a
        + mse_r
        + 2.0 * me_a * ex_b * me_b * ex_a
        + 2.0 * ex_b * me_a * me_r
        + 2.0 * ex_a * me_b * me_r
    )
    return {"me": me_c, "mse": mse_c, "ex": ex_a * ex_b, "ex2": ex2_a * ex2_b}


def _adder_output_state(
    ws_a: Dict[str, float],
    ws_b: Dict[str, float],
    me_r: float,
    mse_r: float,
) -> Dict[str, float]:
    """
    Eq 12.
      E[c_exa]  = E[a_exa] + E[b_exa]
      E[c²_exa] = E[a²_exa] + 2·E[a_exa]·E[b_exa] + E[b²_exa]
      E[Δc]     = E[Δa] + E[Δb] + E[Δr]
      E[Δc²]    = E[Δa²] + E[Δb²] + E[Δr²] + 2·E[Δa]·E[Δb] + 2·E[Δa]·E[Δr] + 2·E[Δb]·E[Δr]
    """
    me_a, mse_a, ex_a, ex2_a = ws_a["me"], ws_a["mse"], ws_a["ex"], ws_a["ex2"]
    me_b, mse_b, ex_b, ex2_b = ws_b["me"], ws_b["mse"], ws_b["ex"], ws_b["ex2"]
    mse_c = mse_a + mse_b + mse_r + 2.0 * me_a * me_b + 2.0 * me_a * me_r + 2.0 * me_b * me_r
    return {
        "me": me_a + me_b + me_r,
        "mse": mse_c,
        "ex": ex_a + ex_b,
        "ex2": ex2_a + 2.0 * ex_a * ex_b + ex2_b,
    }


def _adder_output_state_feeder(
    ws_a: Dict[str, float],
    ws_b: Dict[str, float],
    entry: list,
) -> Dict[str, float]:
    """
    Eq 12 with the own error measured behind the feeding units:
    entry = [E[Δr], E[Δr²], E[d·Δr], E[d]], d = Δa + Δb.
    """
    me_r, mse_r, dxr_tab, me_d_tab = entry
    me_a, me_b = ws_a["me"], ws_b["me"]
    me_d = me_a + me_b
    dxr = dxr_tab + (me_d - me_d_tab) * me_r
    return {
        "me": me_d + me_r,
        "mse": ws_a["mse"] + ws_b["mse"] + mse_r + 2.0 * me_a * me_b + 2.0 * dxr,
        "ex": ws_a["ex"] + ws_b["ex"],
        "ex2": ws_a["ex2"] + 2.0 * ws_a["ex"] * ws_b["ex"] + ws_b["ex2"],
    }


def _node_output_state(op, ws_a, ws_b, me_r, mse_r):
    if op == "mul":
        return _multiplier_output_state(ws_a, ws_b, me_r, mse_r)
    return _adder_output_state(ws_a, ws_b, me_r, mse_r)


def propagate_circuit_scalar(
    dfg: BenchmarkDFG,
    codes: Dict[str, int],
    component_lib: Dict[int, ComponentVariant],
    adder_range_lib: Optional[Dict] = None,
    mul_range_lib: Optional[Dict] = None,
    wire_max: Optional[Dict[str, float]] = None,
    node_lib: Optional[Dict] = None,
    feeder_lib: Optional[Dict] = None,
) -> Dict[str, Dict[str, float]]:
    """
    Propagate {me, mse, ex, ex2} from the inputs to every wire.

    Own error of a node, in order of preference:
      feeder_lib[node]["code|feeder_a|feeder_b"]   adders, from characterize.py
      node_lib[node][code]                         from characterize.py
      range-bucketed libraries
    """
    if wire_max is None:
        wire_max = compute_wire_max(dfg)
    wire_states = {inp: _input_wire_state(*dfg.input_ranges.get(inp, (0.0, 0.0))) for inp in dfg.inputs}
    made_by = {inp: "-" for inp in dfg.inputs}
    for node in dfg.nodes:
        wire_a, wire_b = node.inputs
        if wire_a not in wire_states or wire_b not in wire_states:
            raise ValueError(f"node {node.id!r} reads a wire that is not yet defined")
        ws_a, ws_b = wire_states[wire_a], wire_states[wire_b]
        code = codes[node.id]
        made_by[node.output] = str(code)

        if feeder_lib and node.op == "add" and code != 0:
            entry = feeder_lib.get(node.id, {}).get(f"{code}|{made_by[wire_a]}|{made_by[wire_b]}")
            if entry is not None:
                wire_states[node.output] = _adder_output_state_feeder(ws_a, ws_b, entry)
                continue

        entry = node_lib.get(node.id, {}).get(str(code)) if node_lib else None
        if wire_max.get(node.output, 1.0) == 0:
            me_r, mse_r = 0.0, 0.0
        elif entry is not None:
            me_r, mse_r = entry["me"], entry["mse"]
        else:
            max_input = max(wire_max.get(wire_a, 65535.0), wire_max.get(wire_b, 65535.0))
            me_r, mse_r = _node_scalars(component_lib, code, adder_range_lib, mul_range_lib, max_input)
        wire_states[node.output] = _node_output_state(node.op, ws_a, ws_b, me_r, mse_r)
    return wire_states
