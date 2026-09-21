"""
circuit.py -- generic Data Flow Graph (DFG) representation for the paper's
error-propagation model (Chen et al., ISVLSI 2024).

A Circuit is built from named nodes; each node is an 'add' or 'mul' of two
operands (the paper's two supported node types, Section III). Operands are
one of:

  ('const', value)              a fixed value (e.g. a filter coefficient)
  ('input', name, delay)        a real input stream, optionally delayed by
                                 `delay` samples (delay=0: current sample,
                                 e.g. rgb2gray's R/G/B; delay>0: a tapped-
                                 delay-line value, e.g. FIR filter taps)
  ('node', name)                 the output of another node in this circuit

This file has NO knowledge of any specific circuit -- the 9-tap FIR,
rgb2gray, an FFT butterfly stage, etc. are all just different Circuit
instances assembled from these three primitives. See benchmarks/ for
concrete circuit definitions.

Scope (see paper Sections II-IV): this model supports feedforward DAGs of
add/mul nodes only. Feedback (IIR-style) circuits and non-arithmetic
operations (compare, shift, saturate, etc.) are out of scope -- adding a new
op type requires deriving its own u,h,v,g propagation rules (paper eq 12/13
are specific to add/mul) before it can be added here.
"""
from collections import deque


class Circuit:
    def __init__(self, name="circuit"):
        self.name = name
        self.nodes = {}          # node_name -> (op, operand_a, operand_b)
        self.inputs = set()      # declared input stream names
        self.output = None       # name of the node that is the circuit's result
        self._topo_cache = None

    def input(self, name):
        """Declare a named real-valued input stream (e.g. 'sample', 'R', 'G', 'B')."""
        self.inputs.add(name)
        return name

    def const(self, value):
        return ("const", value)

    def tap(self, input_name, delay=0):
        """Reference an input stream, optionally delayed by `delay` samples."""
        if input_name not in self.inputs:
            raise ValueError(f"input '{input_name}' was not declared -- call circuit.input() first")
        return ("input", input_name, delay)

    def add(self, name, op, a, b):
        """Define a node. op is 'add' or 'mul'. a, b are operand descriptors
        (from const()/tap()/a previous add()'s return value). Returns
        ('node', name) so it can be chained directly into the next add() call."""
        if op not in ("add", "mul"):
            raise ValueError(
                f"unsupported op '{op}' -- only 'add'/'mul' have paper-derived "
                f"propagation rules (eq 12/13); adding a new op requires deriving "
                f"its own u,h,v,g formulas first")
        if name in self.nodes:
            raise ValueError(f"node '{name}' already defined")
        self.nodes[name] = (op, a, b)
        self._topo_cache = None
        return ("node", name)

    def set_output(self, node_name):
        if node_name not in self.nodes:
            raise ValueError(f"'{node_name}' is not a defined node")
        self.output = node_name

    def _deps(self, node_name):
        op, a, b = self.nodes[node_name]
        return [operand[1] for operand in (a, b) if operand[0] == "node"]

    def topo_order(self):
        """Kahn's-algorithm topological sort over node dependencies -- computed
        automatically from the graph structure, no manual ordering required."""
        if self._topo_cache is not None:
            return self._topo_cache
        indegree = {k: 0 for k in self.nodes}
        children = {k: [] for k in self.nodes}
        for k in self.nodes:
            for dep in self._deps(k):
                if dep not in self.nodes:
                    raise ValueError(f"node '{k}' depends on undefined node '{dep}'")
                indegree[k] += 1
                children[dep].append(k)
        queue = deque([k for k, d in indegree.items() if d == 0])
        order = []
        while queue:
            k = queue.popleft()
            order.append(k)
            for c in children[k]:
                indegree[c] -= 1
                if indegree[c] == 0:
                    queue.append(c)
        if len(order) != len(self.nodes):
            raise ValueError(
                "circuit has a cycle -- this model only supports feedforward "
                "DAGs (no feedback/IIR-style loops); see module docstring")
        self._topo_cache = order
        return order

    def mul_nodes(self):
        return [k for k, (op, _, _) in self.nodes.items() if op == "mul"]

    def add_nodes(self):
        return [k for k, (op, _, _) in self.nodes.items() if op == "add"]

    def validate(self):
        """Sanity checks: output is set, graph is acyclic, all node refs resolve."""
        if self.output is None:
            raise ValueError("circuit has no output set -- call set_output()")
        self.topo_order()   # raises on cycles / dangling refs
        return True
