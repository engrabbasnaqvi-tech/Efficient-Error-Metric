"""
Optional validation helpers -- NOT part of the analytic model.

monte_carlo() runs the real approximate hardware over the full sample stream to
get ground-truth ME/MSE/SNR. It is what you check analytic_propagate() against.
It DOES run the exact circuit (needed as the error reference), so it is sample-
count-dependent -- that's expected; it's the yardstick, not the model.

inaccuracy() is the paper's eq (14): |est - sim| / |sim|.

Usage:
    from model import InputStats, analytic_propagate
    from validate import monte_carlo, inaccuracy
    stats = InputStats(circuit, {"sample": samples})
    ana = analytic_propagate(stats, assign)
    sim = monte_carlo(circuit, {"sample": samples}, assign)
    print(inaccuracy(ana, sim))
"""
import numpy as np
from component_lib import call_unit, exact as hw_exact


def _tap(x, delay, n):
    t = np.zeros(n, dtype=np.int64)
    if delay == 0:
        t[:] = x
    else:
        t[delay:] = x[:-delay]
    return t


def _broadcast_pair(va, vb):
    if np.ndim(va) == 0 and np.ndim(vb) > 0:
        va = np.broadcast_to(va, vb.shape)
    elif np.ndim(vb) == 0 and np.ndim(va) > 0:
        vb = np.broadcast_to(vb, va.shape)
    return va.astype(np.int64), vb.astype(np.int64)


def monte_carlo(circuit, input_data, assign):
    """Ground truth: full real simulation using the real approximate units."""
    data = {k: np.asarray(v, dtype=np.int64) for k, v in input_data.items()}
    n = len(next(iter(data.values())))
    stream = {}

    def val(operand):
        kind = operand[0]
        if kind == "const":
            return np.int64(operand[1])
        if kind == "input":
            _, name, delay = operand
            return _tap(data[name], delay, n)
        return stream[operand[1]]

    # exact reference
    exact_stream = {}
    for k in circuit.topo_order():
        op, sa, sb = circuit.nodes[k]
        def ev(operand):
            kind = operand[0]
            if kind == "const": return np.int64(operand[1])
            if kind == "input":
                _, name, delay = operand
                return _tap(data[name], delay, n)
            return exact_stream[operand[1]]
        a, b = _broadcast_pair(ev(sa), ev(sb))
        exact_stream[k] = hw_exact(op, a, b)
    out_exact = exact_stream[circuit.output].astype(np.float64)
    Eout2 = float((out_exact ** 2).mean())

    # approximate run
    for k in circuit.topo_order():
        op, sa, sb = circuit.nodes[k]
        unit = assign.get(k, "exact")
        va, vb = _broadcast_pair(val(sa), val(sb))
        stream[k] = hw_exact(op, va, vb) if unit == "exact" else call_unit(op, unit, va, vb)

    out = stream[circuit.output].astype(np.float64)
    err = out - out_exact
    me = float(err.mean()); mse = float((err ** 2).mean())
    snr = 10 * np.log10(Eout2 / mse) if mse > 0 else float("inf")
    return {"ME": me, "MSE": mse, "SNR": snr}


def inaccuracy(est, sim):
    """Paper eq (14): |EM_est - EM_sim| / |EM_sim|, for ME and MSE, plus SNR diff."""
    def rel(a, b):
        return abs(a - b) / abs(b) if b != 0 else (0.0 if a == 0 else float("inf"))
    return {"ME%": 100 * rel(est["ME"], sim["ME"]),
            "MSE%": 100 * rel(est["MSE"], sim["MSE"]),
            "SNRdiff": est["SNR"] - sim["SNR"]}
