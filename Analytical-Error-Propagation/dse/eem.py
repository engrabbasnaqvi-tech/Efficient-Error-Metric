import importlib.util
import math
import os
import sys
from pathlib import Path

import numpy as np

from sim.simulate import EXACT, SEED_INPUTS, UNITS, run_unit, sample_inputs

EEM_ROOT = Path(os.environ.get("EEM_PATH", Path(__file__).resolve().parents[2] / "EEM" / "EEM"))


def load_eem():
    # loaded unchanged, under another name (model.py clashes with our model package), without writing bytecode
    core = EEM_ROOT / "core"
    if not (core / "model.py").exists():
        raise FileNotFoundError(f"EEM not found at {EEM_ROOT} (set EEM_PATH)")
    if str(core) not in sys.path:
        sys.path.append(str(core))
    write_bytecode = sys.dont_write_bytecode
    sys.dont_write_bytecode = True
    try:
        spec = importlib.util.spec_from_file_location("eem_model", core / "model.py")
        model = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(model)
        import component_lib
        from circuit import Circuit
    finally:
        sys.dont_write_bytecode = write_bytecode
    return model, Circuit, component_lib


def compare_units(component_lib, lib, pairs=4096):
    # how each EEM unit relates to ours: "same", "swapped" (EEM unit(b, a) == ours(a, b)) or "different"
    rng = np.random.default_rng(0)
    a, b = rng.integers(0, 65536, pairs), rng.integers(0, 65536, pairs)
    relation = {}
    for op in ("add", "mul"):
        for code in UNITS[op]:
            ours = run_unit(op, code, a, b)
            if np.array_equal(component_lib.call_unit(op, lib[code].name, a, b), ours):
                relation[code] = "same"
            elif np.array_equal(component_lib.call_unit(op, lib[code].name, b, a), ours):
                relation[code] = "swapped"
            else:
                relation[code] = "different"
    return relation


def to_circuit(dfg, Circuit, swapped=frozenset()):
    # nodes in `swapped` get their operands in reverse order
    circuit = Circuit(dfg.output)
    constants = {w: int(lo) for w, (lo, hi) in dfg.input_ranges.items() if lo == hi}
    for w in dfg.inputs:
        if w not in constants:
            circuit.input(w)
    producer = {}

    def operand(w):
        if w in constants:
            return circuit.const(constants[w])
        if w in producer:
            return ("node", producer[w])
        return circuit.tap(w, 0)

    for node in dfg.nodes:
        a, b = (operand(w) for w in node.inputs)
        circuit.add(node.id, node.op, *((b, a) if node.id in swapped else (a, b)))
        producer[node.output] = node.id
    circuit.set_output(producer[dfg.output])
    return circuit


def eem_error(dfg, samples, m_out, lib):
    # codes -> RMS % from the EEM model, on the same inputs as the simulation
    model, Circuit, component_lib = load_eem()
    relation = compare_units(component_lib, lib)
    inputs = sample_inputs(dfg, samples, SEED_INPUTS)
    stats = {}

    def rms(codes):
        swapped = frozenset(nid for nid, code in codes.items() if relation.get(code) == "swapped")
        if swapped not in stats:
            circuit = to_circuit(dfg, Circuit, swapped)
            stats[swapped] = model.InputStats(circuit, {w: inputs[w] for w in circuit.inputs})
        assign = {nid: lib[code].name for nid, code in codes.items() if code != EXACT[lib[code].op]}
        return math.sqrt(max(model.analytic_propagate(stats[swapped], assign)["MSE"], 0.0)) / m_out * 100

    rms.different = sorted(lib[code].name for code, r in relation.items() if r == "different")
    return rms
