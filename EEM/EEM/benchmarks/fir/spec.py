"""
FIR benchmark spec — the ONLY per-benchmark code.

Defines, for each backend, the exact circuit the DeepApprox netlist computes:
its topology, coefficients, tap delays, multiplier operand order, and the
line-number -> node map used to decode a DeepApprox `conf_path`.

Two backends are supported because the SystemC and Verilog FIR are DIFFERENT
circuits computing the same filter:
  - SystemC : balanced adder TREE, mul(coeff, sample), delays [0..8]
  - Verilog : linear adder CASCADE, mul(sample, coeff), delays [0,0,1..7]

The model core (core/model.py, core/circuit.py) never needs to know any of this
— it just consumes the Circuit object build_circuit() returns.
"""
from circuit import Circuit

COEFFS = [7, 17, 32, 46, 52, 46, 32, 17, 7]   # b0..b8, same for both backends
INPUTS = ["sample"]                            # single input stream


# ---------------------------------------------------------------- SystemC
def _build_systemc():
    """Balanced tree, mul(coeff, sample), delays [0..8]."""
    delays = [0, 1, 2, 3, 4, 5, 6, 7, 8]
    c = Circuit("fir_systemc")
    c.input("sample")
    M = [c.add(f"M{i}", "mul", c.const(k), c.tap("sample", d))
         for i, (k, d) in enumerate(zip(COEFFS, delays))]
    S0 = c.add("S0", "add", M[0], M[1]); S1 = c.add("S1", "add", M[2], M[3])
    S2 = c.add("S2", "add", M[4], M[5]); S3 = c.add("S3", "add", M[6], M[7])
    S4 = c.add("S4", "add", S0, S1);     S5 = c.add("S5", "add", S2, S3)
    S6 = c.add("S6", "add", S4, S5)
    S7 = c.add("S7", "add", S6, M[8])
    c.set_output("S7")
    return c

# annotated-file line -> node (from the SystemC candidate table)
_LINE_SYSTEMC = {ln: f"M{i}" for i, ln in enumerate(range(63, 72))}          # 63..71
for _i, _ln in enumerate([85, 86, 87, 88, 91, 92, 93, 94]):
    _LINE_SYSTEMC[_ln] = f"S{_i}"                                            # adders


# ---------------------------------------------------------------- Verilog
def _build_verilog():
    """Linear cascade, mul(sample, coeff), delays [0,0,1..7]."""
    delays = [0, 0, 1, 2, 3, 4, 5, 6, 7]
    c = Circuit("fir_verilog")
    c.input("sample")
    M = [c.add(f"M{i}", "mul", c.tap("sample", d), c.const(k))
         for i, (k, d) in enumerate(zip(COEFFS, delays))]
    p = c.add("P1", "add", M[1], M[0])
    for i in range(2, 9):
        p = c.add(f"P{i}", "add", p, M[i])
    c.set_output("P8")
    return c

_LINE_VERILOG = {ln: f"M{i}" for i, ln in enumerate(range(50, 59))}          # 50..58
for _i, _ln in enumerate(range(71, 79)):
    _LINE_VERILOG[_ln] = f"P{_i+1}"                                          # 71..78


# ---------------------------------------------------------------- public API
BACKENDS = {
    "systemc": {"build": _build_systemc, "line_map": _LINE_SYSTEMC,
                "node_order": [f"M{i}" for i in range(9)] + [f"S{i}" for i in range(8)]},
    "verilog": {"build": _build_verilog, "line_map": _LINE_VERILOG,
                "node_order": [f"M{i}" for i in range(9)] + [f"P{i}" for i in range(1, 9)]},
}

def build_circuit(backend):
    return BACKENDS[backend]["build"]()

def line_map(backend):
    return BACKENDS[backend]["line_map"]

def node_order(backend):
    return BACKENDS[backend]["node_order"]
