"""
Analytic error-propagation model (Chen et al., ISVLSI 2024) -- analytic-only build.

"""
import numpy as np
from component_lib import call_unit, exact as hw_exact
from circuit import Circuit
from unit_lut import lut_eval, save_all as _lut_save

# Cap on PMF support points before MIA-style interval merging (paper Sec IV-C).

MAX_SUPPORT = 2000


class InputStats:
    """Holds real input data + statistics. The analytic path needs only the
    circuit, the raw input arrays (to build input PMFs), and per-input value
    moments E[x], E[x^2]. """

    def __init__(self, circuit: Circuit, input_data: dict):
        circuit.validate()
        self.circuit = circuit
        self.input_data = {k: np.asarray(v, dtype=np.int64) for k, v in input_data.items()}
        lengths = {len(v) for v in self.input_data.values()}
        if len(lengths) != 1:
            raise ValueError(f"all input streams must have the same length, got {lengths}")
        self.n = lengths.pop()
        self.input_moments = {}
        for name, arr in self.input_data.items():
            arr_f = arr.astype(np.float64)
            self.input_moments[name] = (float(arr_f.mean()), float((arr_f ** 2).mean()))


def _combine_pmf(values, probs):
    order = np.argsort(values)
    v_sorted = values[order]; p_sorted = probs[order]
    uniq_v, start_idx = np.unique(v_sorted, return_index=True)
    uniq_p = np.add.reduceat(p_sorted, start_idx)
    return uniq_v, uniq_p


def _compress_pmf(values, probs, max_support=MAX_SUPPORT):
    """MIA-style interval merging (paper Sec IV-C): bin nearby values, sum prob."""
    if values.size <= max_support:
        return values, probs
    lo, hi = values.min(), values.max()
    bin_width = max(1, int(np.ceil((hi - lo + 1) / max_support)))
    bin_idx = (values - lo) // bin_width
    order = np.argsort(bin_idx)
    bi_sorted = bin_idx[order]; v_sorted = values[order]; p_sorted = probs[order]
    uniq_bi, start_idx = np.unique(bi_sorted, return_index=True)
    bin_p = np.add.reduceat(p_sorted, start_idx)
    weighted_v = np.add.reduceat(v_sorted.astype(np.float64) * p_sorted, start_idx)
    bin_v = np.round(weighted_v / bin_p).astype(np.int64)
    return bin_v, bin_p


def _empirical_input_pmf(input_array, max_support=MAX_SUPPORT):
    """Exact empirical PMF over the WHOLE input stream -- the one place samples
    enter the analytic path."""
    vals, counts = np.unique(input_array, return_counts=True)
    probs = counts.astype(np.float64) / counts.sum()
    return _compress_pmf(vals.astype(np.int64), probs, max_support)


def _combine_moments(op, ma1, ma2, da1, da2, mb1, mb2, db1, db2, r1, r2):
    """Paper eq 12 (add) / eq 13 (mul).
    m*1=E[val], m*2=E[val^2]; d*1=E[err], d*2=E[err^2]; r=this node's own error.
    Returns (mc1,mc2,dc1,dc2): mc=u,h (signal moments), dc=v,g (error moments)."""
    if op == "mul":
        mc1 = ma1 * mb1
        mc2 = ma2 * mb2
        dc1 = da1 * mb1 + db1 * ma1 + r1
        dc2 = (da2 * mb2 + db2 * ma2 + r2
               + 2 * da1 * mb1 * db1 * ma1
               + 2 * mb1 * da1 * r1
               + 2 * ma1 * db1 * r1)
    else:
        mc1 = ma1 + mb1
        mc2 = ma2 + 2 * ma1 * mb1 + mb2
        dc1 = da1 + db1 + r1
        dc2 = da2 + db2 + r2 + 2 * da1 * db1 + 2 * da1 * r1 + 2 * db1 * r1
    return mc1, mc2, dc1, dc2


class PMFModel:
    """Pass 1: per node, E[Dr]/E[Dr^2] (error module) + output PMF (output module)."""

    # Class-level cache for multipliers whose operands are (const, input): their
    # arriving distribution is the fixed input PMF (config-independent), so both
    # r_exact and the output PMF depend only on (unit, coeff) for a given dataset.
    # Keyed by (unit, coeff, input_name, data_id) so it auto-invalidates if the
    # input data changes. Computed once, reused across all configs.
    _mul_cache = {}

    def __init__(self, stats: InputStats, max_support=MAX_SUPPORT):
        self.g = stats
        self.max_support = max_support
        self.pmf = {}
        self.r_exact = {}
        self._input_pmf_cache = {}
        # identity of this input dataset, so the mul cache can't go stale
        self._data_id = id(self.g.input_data)

    def _get_input_pmf(self, name):
        if name not in self._input_pmf_cache:
            self._input_pmf_cache[name] = _empirical_input_pmf(
                self.g.input_data[name], self.max_support)
        return self._input_pmf_cache[name]

    def _operand_pmf(self, operand):
        kind = operand[0]
        if kind == "const":
            return np.array([operand[1]], dtype=np.int64), np.array([1.0])
        if kind == "input":
            _, name, _delay = operand
            return self._get_input_pmf(name)
        return self.pmf[operand[1]]

    def _mul_cache_key(self, op, sa, sb, unit):
        """Return a cache key if this node is a multiplier with (const, input)
        operands (in either order) -- the config-independent case. Else None."""
        if op != "mul" or unit == "exact":
            return None
        kinds = {sa[0], sb[0]}
        if kinds != {"const", "input"}:
            return None
        const_op = sa if sa[0] == "const" else sb
        input_op = sa if sa[0] == "input" else sb
        return (unit, const_op[1], input_op[1], self._data_id)

    def build(self, assign: dict):
        circuit = self.g.circuit
        for k in circuit.topo_order():
            op, sa, sb = circuit.nodes[k]
            unit = assign.get(k, "exact")

            # --- fast path: cached input-fed multiplier (config-independent) ---
            ck = self._mul_cache_key(op, sa, sb, unit)
            if ck is not None and ck in PMFModel._mul_cache:
                self.r_exact[k], self.pmf[k] = PMFModel._mul_cache[ck]
                continue

            va, pa = self._operand_pmf(sa)
            vb, pb = self._operand_pmf(sb)

            A, B = np.meshgrid(va, vb, indexing="ij")
            PA, PB = np.meshgrid(pa, pb, indexing="ij")
            a_flat = A.ravel().astype(np.int64); b_flat = B.ravel().astype(np.int64)
            joint_p = (PA * PB).ravel()

            exact_out = hw_exact(op, a_flat, b_flat)
            approx_out = exact_out if unit == "exact" else lut_eval(op, unit, a_flat, b_flat)

            dr = (approx_out - exact_out).astype(np.float64)
            r1 = float(np.sum(dr * joint_p))
            r2 = float(np.sum((dr ** 2) * joint_p))
            self.r_exact[k] = (r1, r2)

            out_vals, out_probs = _combine_pmf(approx_out, joint_p)
            out_vals, out_probs = _compress_pmf(out_vals, out_probs, self.max_support)
            self.pmf[k] = (out_vals, out_probs)

            # store in the config-independent cache if this is such a multiplier
            if ck is not None:
                PMFModel._mul_cache[ck] = (self.r_exact[k], self.pmf[k])
        _lut_save()
        return self.r_exact


def analytic_propagate(stats: InputStats, assign: dict, max_support=MAX_SUPPORT):
    """Pass 1 (PMFModel) -> each node's Dr. Pass 2 -> propagate value+error
    moments via eq 12/13 to the output. Returns ME, MSE, Ecexa2, SNR (all moments)."""
    model = PMFModel(stats, max_support)
    r_exact = model.build(assign)
    circuit = stats.circuit

    def moments(operand):
        kind = operand[0]
        if kind == "const":
            v = float(operand[1]); return (v, v * v, 0.0, 0.0)
        if kind == "input":
            _, name, _delay = operand
            vals, probs = model._get_input_pmf(name)
            m1 = float(np.sum(vals * probs))
            m2 = float(np.sum((vals.astype(np.float64) ** 2) * probs))
            return (m1, m2, 0.0, 0.0)
        return st[operand[1]]

    st = {}
    for k in circuit.topo_order():
        op, sa, sb = circuit.nodes[k]
        ma1, ma2, da1, da2 = moments(sa)
        mb1, mb2, db1, db2 = moments(sb)
        r1, r2 = r_exact[k]
        st[k] = _combine_moments(op, ma1, ma2, da1, da2, mb1, mb2, db1, db2, r1, r2)

    mc1, mc2, dc1, dc2 = st[circuit.output]
    snr = 10 * np.log10(mc2 / dc2) if dc2 > 0 else float("inf")
    return {"ME": dc1, "MSE": dc2, "Ecexa2": mc2, "SNR": snr, "state": st}
