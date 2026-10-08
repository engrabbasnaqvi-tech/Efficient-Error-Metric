import re

from sim.simulate import EXACT

EXACT_MODULE = {"add": "adder_16bit", "mul": "mul16u_exact"}


def module_name(code, op, lib):
    # Verilog module of a unit code; the exact units have their own names in components.v
    return EXACT_MODULE[op] if code == EXACT[op] else lib[code].name


def build_netlist(dfg, codes, lib, top):
    # structural Verilog of the whole circuit, one component instance per DFG node;
    # inputs with a fixed range (lo == hi) become constants
    constants = {w: int(lo) for w, (lo, hi) in dfg.input_ranges.items() if lo == hi}
    ports = [w for w in dfg.inputs if w not in constants]
    internal = [node.output for node in dfg.nodes if node.output != dfg.output]

    def wire(name):
        return f"32'd{constants[name]}" if name in constants else name

    lines = [f"module {top}({', '.join(ports + [dfg.output])});"]
    lines += [f"input  [31:0] {w};" for w in ports]
    lines.append(f"output [31:0] {dfg.output};")
    lines += [f"wire   [31:0] {w};" for w in internal]
    lines.append("")
    for node in dfg.nodes:
        a, b = node.inputs
        module = module_name(codes[node.id], node.op, lib)
        lines.append(f"{module} u_{node.id}({wire(a)}, {wire(b)}, {node.output});")
    lines += ["", "endmodule"]
    return "\n".join(lines) + "\n"


def rewrite_original(text, dfg, codes, lib):
    # the n-th DFG adder (multiplier) goes into the n-th adder (multiplier) instance,
    # instances ordered by their component_<i> index
    pattern = re.compile(r"^(\s*)(\w+)(\s+)(component_(\d+))(\s*\()", re.MULTILINE)
    instances = {"add": [], "mul": []}
    for match in pattern.finditer(text):
        op = "mul" if "mul" in match.group(2).lower() else "add"
        instances[op].append((int(match.group(5)), match.group(4)))
    nodes = {op: [node.id for node in dfg.nodes if node.op == op] for op in instances}
    for op in instances:
        if len(instances[op]) != len(nodes[op]):
            raise ValueError(f"original netlist has {len(instances[op])} {op} instances, the DFG {len(nodes[op])} {op} nodes")

    module_of = {}
    for op in instances:
        for (_, instance), nid in zip(sorted(instances[op]), nodes[op]):
            module_of[instance] = module_name(codes[nid], op, lib)

    def substitute(match):
        module = module_of[match.group(4)]
        return f"{match.group(1)}{module}{match.group(3)}{match.group(4)}{match.group(6)}"

    top = re.search(r"^\s*module\s+(\w+)", text, re.MULTILINE).group(1)
    return pattern.sub(substitute, text), top
