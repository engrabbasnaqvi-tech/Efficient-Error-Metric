"""
Baseline: MCTS with simulation from the all-exact circuit.
python3 -B baseline/baseline.py fir_n4 --cap 5 --budget 10000 --synth

"""
import argparse
import json
import random
import socket
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import numpy as np

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from area.synopsys_dc import constraint, dc_available, synthesise
from dse.area import circuit_area, load_areas, variants
from dse.mcts import run_mcts
from model.data import load_benchmark, load_library
from model.propagate import compute_wire_max
from sim.simulate import EXACT, SEED_INPUTS, sample_inputs, simulate

FINAL_SAMPLES = 1_000_000
RESULTS = ROOT / "results" / "baseline"


def rmse(dfg, codes, inputs, m_out):
    # raw RMSE at the output, computed as in the Partitioning project (simulator.mc_stats)
    exact, approx = simulate(dfg, codes, inputs)
    errors = (approx.astype(np.float64) - exact.astype(np.float64)) / m_out * 100.0
    return float(np.sqrt(np.mean(errors ** 2))) / 100.0 * m_out


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("benchmark")
    parser.add_argument("--cap", type=float, default=5.0, metavar="PCT",
                        help="RMS error cap in %% of the largest output (default: 5)")
    parser.add_argument("--budget", type=int, default=10000, metavar="N",
                        help="MCTS iterations (default: 10000)")
    parser.add_argument("--samples", type=int, default=1_000_000, metavar="N",
                        help="simulation samples per search step (default: 1000000)")
    parser.add_argument("--seed", type=int, metavar="N",
                        help="seed of the search (default: none, as in the Partitioning project)")
    parser.add_argument("--synth", action="store_true",
                        help="synthesise the exact and the final circuit with Design Compiler")
    parser.add_argument("--out", type=Path, metavar="FILE",
                        help="result JSON, the log goes next to it (default: results/baseline/<benchmark>_c<cap>.json)")
    args = parser.parse_args()
    if args.synth and not dc_available():
        sys.exit("--synth: Design Compiler not available")

    start = time.perf_counter()
    random.seed(args.seed)
    dfg, lib = load_benchmark(args.benchmark), load_library()
    areas = load_areas(lib)
    m_out = compute_wire_max(dfg)[dfg.output]
    cap = args.cap / 100.0 * m_out
    inputs = sample_inputs(dfg, args.samples, SEED_INPUTS)
    exact = {node.id: EXACT[node.op] for node in dfg.nodes}
    exact_area = circuit_area(exact, areas)

    out = args.out or RESULTS / f"{args.benchmark}_c{args.cap:g}.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    with open(out.with_suffix(".log"), "w", buffering=1) as log:
        log.write(f"{args.benchmark}  cap {args.cap:g} % ({cap:.2f} of {m_out:.0f})  budget {args.budget}  "
                  f"samples {args.samples:,}  seed {args.seed}  host {socket.gethostname()}\n")

        def step(it, child, best):
            log.write(f"iter {it}/{args.budget}  level {child.level}  {child.node_id}={child.name}  "
                      f"RMSE {child.rms:.1f}" + (f"  area {child.area:.3f}" if child.area is not None else "  over cap")
                      + (f"  best {(1 - best.area / exact_area) * 100:.2f} %" if best else "") + "\n")

        t_search = time.perf_counter()
        found = run_mcts([node.id for node in dfg.nodes], variants(dfg, lib, areas), exact,
                         lambda codes: rmse(dfg, codes, inputs, m_out), lambda codes: circuit_area(codes, areas),
                         cap, args.budget, on_step=step)
        search_s = time.perf_counter() - t_search

        final_inputs = inputs if args.samples == FINAL_SAMPLES else sample_inputs(dfg, FINAL_SAMPLES, SEED_INPUTS)
        final_rmse = rmse(dfg, found["codes"], final_inputs, m_out)
        result = {
            "benchmark": args.benchmark, "cap_pct": args.cap, "budget": args.budget, "samples": args.samples,
            "seed": args.seed, "host": socket.gethostname(), "m_out": m_out, "cap_raw": cap,
            "codes": found["codes"], "path": found["path"],
            "iterations": found["iterations"], "total_error_evals": found["expanded"],
            "search_rmse": found["rms"], "raw_rmse": final_rmse, "rms_pct": final_rmse / m_out * 100.0,
            "library_area": found["area"], "library_exact_area": exact_area,
            "library_saving_pct": (1 - found["area"] / exact_area) * 100.0,
            "dse_elapsed_s": search_s,
        }
        if args.synth:
            t_synth = time.perf_counter()
            jobs = {"exact": (exact, "baseline_exact"), "final": (found["codes"], "baseline_final")}
            with ThreadPoolExecutor(len(jobs)) as pool:
                runs = {name: pool.submit(synthesise, args.benchmark, codes, tag) for name, (codes, tag) in jobs.items()}
                runs = {name: future.result() for name, future in runs.items()}
            e, f = runs["exact"]["total_area"], runs["final"]["total_area"]
            result["synthesis"] = {**runs, "constraint": constraint(args.benchmark),
                                   "area_reduction_pct": (1 - f / e) * 100.0 if e and f else None,
                                   "seconds": time.perf_counter() - t_synth}
        result["pipeline_elapsed_s"] = time.perf_counter() - start
        out.write_text(json.dumps(result, indent=1))
        log.write(f"done: RMSE {final_rmse:.2f} ({result['rms_pct']:.4f} %), library saving "
                  f"{result['library_saving_pct']:.2f} %, {result['total_error_evals']} simulations, "
                  f"{result['pipeline_elapsed_s']:.1f} s\n")
    print(f"{args.benchmark} cap {args.cap:g} %: error {result['rms_pct']:.4f} %, library saving "
          f"{result['library_saving_pct']:.2f} %"
          + (f", DC saving {result['synthesis']['area_reduction_pct']:.2f} %" if args.synth else "")
          + f", {result['total_error_evals']} simulations, {result['pipeline_elapsed_s']:.1f} s -> {out}")


if __name__ == "__main__":
    main()
