"""
Search for the configuration of approximate units with the smallest area under an error cap.

  python3 pipeline.py <benchmark> --mode {sim,analytical,hybrid,eem} [options]

An MCTS picks a unit for every adder and multiplier of the benchmark. The mode decides where
the error of each configuration comes from:

  sim         simulation
  analytical  the analytical model; the search runs once per seed, and the candidates are
              simulated (Pareto front first, then smallest area first) until one is under the cap
  hybrid      like analytical, but under cap - margin; the simulation search then continues
              from the first candidate that is under the cap
  eem         the EEM model (../EEM/EEM, or set EEM_PATH)

The final configuration is always simulated, so its real error is reported. The result JSON
and the terminal log go to results/dse/.

examples:
  python3 pipeline.py fir --mode sim --budget 500 --quiet
  python3 pipeline.py adder_tree_64 --mode analytical --rand-start medium --budget 100 --quiet
  python3 pipeline.py fir_n25 --mode hybrid --budget 500 --margin 1 --synth --quiet
"""
import argparse
import json
import random
import sys
import time
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor, as_completed
from pathlib import Path

import numpy as np

from area.netlist import rewrite_original
from area.synopsys_dc import (
    SIF_IMAGE,
    constraint,
    dc_available,
    describe,
    original_netlist,
    synthesise,
    template,
)
from dse.area import circuit_area, load_areas, variants
from dse.eem import EEM_ROOT, eem_error
from dse.mcts import run_mcts
from model.data import (
    D1,
    DATA,
    load_benchmark,
    load_deep_table,
    load_library,
    read_json,
)
from model.propagate import circuit_rms, compute_wire_max, propagate_circuit_scalar
from pipeline_console import PipelineConsole
from sim.simulate import (
    EXACT,
    SEED_INPUTS,
    UNITS,
    sample_inputs,
    simulate,
)

RESULTS = Path(__file__).resolve().parent / "results" / "dse"
ui = PipelineConsole()


def fail(message):
    ui.error(message)
    sys.exit(1)


class Timer:
    def __enter__(self):
        self.start = time.perf_counter()
        return self

    def __exit__(self, *exc):
        self.seconds = time.perf_counter() - self.start


class Evaluator:
    # wraps an error function with a cache, a call counter and a timer
    def __init__(self, name, fn):
        self.name = name
        self.fn = fn
        self.cache = {}
        self.calls = 0
        self.hits = 0
        self.seconds = 0.0

    def __call__(self, codes):
        key = tuple(sorted(codes.items()))
        if key in self.cache:
            self.hits += 1
        else:
            start = time.perf_counter()
            self.cache[key] = self.fn(codes)
            self.seconds += time.perf_counter() - start
            self.calls += 1
        return self.cache[key]

    def fresh(self, codes):
        start = time.perf_counter()
        value = self.fn(codes)
        self.seconds += time.perf_counter() - start
        self.calls += 1
        return value

    def cached(self, codes):
        return self.cache.get(tuple(sorted(codes.items())))

    def snapshot(self):
        return self.calls, self.hits, self.seconds

    def since(self, snap):
        calls, hits, seconds = snap
        return {"calls": self.calls - calls, "hits": self.hits - hits, "seconds": self.seconds - seconds}

    def mean(self):
        return self.seconds / self.calls if self.calls else None


def simulation_error(dfg, samples, m_out):
    inputs = sample_inputs(dfg, samples, SEED_INPUTS)

    def rms(codes):
        exact, approx = simulate(dfg, codes, inputs)
        error = (approx - exact).astype(np.float64)
        return float(np.sqrt((error ** 2).mean())) / m_out * 100

    return rms



def analytical_error(dfg, benchmark, depth, wire_max, lib):
    node_lib = read_json(D1 / f"{benchmark}.json")
    feeder_lib = read_json(D1 / f"{benchmark}_feeder.json")
    deep_lib = load_deep_table(benchmark) if depth == 2 else None
    if deep_lib:
        source = "node + depth-2 feeder tables"
    elif feeder_lib:
        source = "node + feeder tables"
    elif node_lib:
        source = "node tables"
    else:
        source = f"range libraries (no node tables, run python3 -m characterization.characterize {benchmark})"
    if depth == 2 and deep_lib is None:
        source += f" (no {benchmark}_d2 table, using depth 1)"
    tables = (
        lib,
        read_json(DATA / "adder_range_metrics.json"),
        read_json(DATA / "mul_range_metrics.json"),
        wire_max,
        node_lib,
        feeder_lib,
        deep_lib,
    )
    m_out = wire_max[dfg.output]

    def rms(codes):
        return circuit_rms(propagate_circuit_scalar(dfg, codes, *tables), dfg.output, m_out)

    return rms, source


def start_units(lib, areas, share):
    # per op, the mildest share of the units smaller than the exact one, by library MSE
    units = {}
    for op, exact in EXACT.items():
        smaller = sorted((c for c in UNITS[op] if c in areas and areas[c] < areas[exact]), key=lambda c: lib[c].error)
        units[op] = smaller[:max(1, round(len(smaller) * share))]
    return units


def step_down(dfg, codes, mild, error_fn, cap, rng):
    # random nodes step down, a stronger unit to a mild one and a mild one to exact, until under the cap
    while error_fn(codes) > cap:
        stronger = [n for n in dfg.nodes if codes[n.id] != EXACT[n.op] and codes[n.id] not in mild[n.op]]
        pool = stronger or [n for n in dfg.nodes if codes[n.id] != EXACT[n.op]]
        node = pool[rng.integers(len(pool))]
        codes[node.id] = int(rng.choice(mild[node.op])) if stronger else EXACT[node.op]
    return codes


def random_start(dfg, units, level, error_fns, cap, seed):
    rng = np.random.default_rng(seed)
    codes = {node.id: int(rng.choice(units[level][node.op])) for node in dfg.nodes}
    for error_fn in error_fns:
        codes = step_down(dfg, codes, units["mild"], error_fn, cap, rng)
    return codes


FIRST_SUM_BIT = {8: 2 ** 12, 9: 2 ** 14, 10: 2 ** 15}


def range_rule(moves, dfg, lib):
    # 0GK, 02E and 0MH only at adders whose mean exact output reaches their first real sum bit
    exact = propagate_circuit_scalar(dfg, {node.id: EXACT[node.op] for node in dfg.nodes}, lib)
    for node in dfg.nodes:
        if node.op == "add":
            level = exact[node.output]["ex"]
            moves[node.id] = [(c, name) for c, name in moves[node.id] if level >= FIRST_SUM_BIT.get(c, 0)]
    return moves


def search(error_fn, base, cap, budget, dfg, lib, areas, quiet):
    snap = error_fn.snapshot()
    exact_area = circuit_area({node.id: EXACT[node.op] for node in dfg.nodes}, areas)
    with Timer() as timer, ui.search(error_fn, budget, exact_area, quiet) as step:
        result = run_mcts([n.id for n in dfg.nodes], variants(dfg, lib, areas), base, error_fn,
                          lambda codes: circuit_area(codes, areas), cap, budget, on_step=step)
    evals = error_fn.since(snap)
    result.update(evaluator=error_fn.name, cap=cap, budget=budget, seconds=timer.seconds,
                  evaluations=evals["calls"], cache_hits=evals["hits"], eval_seconds=evals["seconds"],
                  search_seconds=timer.seconds - evals["seconds"],
                  saving_pct=(1 - result["area"] / exact_area) * 100)
    return result


def seed_search(seed, dfg, moves, areas, error_fn, start_for, cap, budget, on_step=None):
    # one model search from start_for(seed)
    base = start_for(seed)
    random.seed(seed)
    r = run_mcts([n.id for n in dfg.nodes], moves, base, error_fn, lambda codes: circuit_area(codes, areas), cap, budget,
                 on_step=on_step)
    return {"seed": seed, "codes": r["codes"], "rms": r["rms"], "area": r["area"], "iterations": r["iterations"],
            "start_area": circuit_area(base, areas)}


worker = {}


def init_seed_worker(benchmark, depth, level, cap, rule):
    # each process of --seed-jobs loads the benchmark and the model once
    dfg = load_benchmark(benchmark)
    lib = load_library()
    areas = load_areas(lib)
    model = Evaluator("analytical", analytical_error(dfg, benchmark, depth, compute_wire_max(dfg), lib)[0])
    units = {"mild": start_units(lib, areas, 1 / 3), "medium": start_units(lib, areas, 2 / 3)}
    exact = {node.id: EXACT[node.op] for node in dfg.nodes}
    start_for = (lambda seed: random_start(dfg, units, level, [model], cap, seed)) if level else (lambda seed: dict(exact))
    moves = variants(dfg, lib, areas)
    worker.update(dfg=dfg, moves=range_rule(moves, dfg, lib) if rule else moves, areas=areas, model=model,
                  start_for=start_for, cap=cap)


def worker_seed_search(seed, budget):
    w = worker
    snap = w["model"].snapshot()
    r = seed_search(seed, w["dfg"], w["moves"], w["areas"], w["model"], w["start_for"], w["cap"], budget)
    return {**r, "evals": w["model"].since(snap)}


def multi_search(error_fn, start_for, cap, budget, seeds, dfg, lib, areas, jobs=1, worker_args=None, rule=False):
    # one search per seed from start_for(seed); the distinct results are the handoff candidates.
    # With jobs > 1 the seeds run in that many processes, set up by init_seed_worker(*worker_args, rule)
    snap = error_fn.snapshot()
    exact_area = circuit_area({node.id: EXACT[node.op] for node in dfg.nodes}, areas)
    moves = variants(dfg, lib, areas)
    if rule:
        moves = range_rule(moves, dfg, lib)
    candidates, iterations, start_areas = {}, 0, []
    with Timer() as timer, ui.seeds(seeds, budget, jobs) as (step, done):
        if jobs > 1:
            with ProcessPoolExecutor(jobs, initializer=init_seed_worker, initargs=(*worker_args, rule)) as pool:
                futures = {pool.submit(worker_seed_search, seed, budget): seed for seed in seeds}
                for future in as_completed(futures):
                    done(futures[future])
                results = [future.result() for future in futures]
            for r in results:
                error_fn.calls += r["evals"]["calls"]
                error_fn.hits += r["evals"]["hits"]
                error_fn.seconds += r["evals"]["seconds"]
        else:
            results = []
            for seed in seeds:
                def progress(it, child, best, seed=seed):
                    step(seed, it, best and ((1 - best.area / exact_area) * 100, best.rms))
                results.append(seed_search(seed, dfg, moves, areas, error_fn, start_for, cap, budget, progress))
                done(seed)
        for r in results:
            start_areas.append(r["start_area"])
            iterations += r["iterations"]
            candidates.setdefault(tuple(sorted(r["codes"].items())),
                                  {"codes": r["codes"], "rms": r["rms"], "area": r["area"], "seed": r["seed"]})
    ranked = sorted(candidates.values(), key=lambda c: c["area"])
    for c in ranked:
        c["saving_pct"] = (1 - c["area"] / exact_area) * 100
    evals = error_fn.since(snap)
    best = ranked[0]
    result = {**best, "candidates": ranked, "seeds": len(seeds), "evaluator": error_fn.name, "cap": cap,
              "budget": budget, "iterations": iterations, "seconds": timer.seconds, "evaluations": evals["calls"],
              "cache_hits": evals["hits"], "eval_seconds": evals["seconds"], "jobs": jobs,
              "search_seconds": timer.seconds - evals["seconds"] if jobs == 1 else None}
    ui.analytical(result, [(1 - a / exact_area) * 100 for a in start_areas])
    return result


def pareto(candidates):
    # smallest area first; a candidate is kept only if its model error is below that of every smaller one
    front, lowest = [], float("inf")
    for c in sorted(candidates, key=lambda c: c["area"]):
        if c["rms"] < lowest:
            front.append(c)
            lowest = c["rms"]
    return front


def verify(candidates, sim, cap, whole_front):
    # simulate the Pareto front smallest area first, then the other candidates smallest area first;
    # the first one under the cap is chosen
    front = pareto(candidates)
    on_front = {id(c) for c in front}
    order = front + sorted((c for c in candidates if id(c) not in on_front), key=lambda c: c["area"])
    chosen = None
    with ui.candidates(cap) as (add, note):
        for i, cand in enumerate(order, 1):
            keep_going = whole_front and i <= len(front)
            if chosen and not keep_going:
                break
            if i == len(front) + 1:
                note("rest, smallest area first")
            with Timer() as t_one:
                cand["sim_rms"] = sim(cand["codes"])
            ok = cand["sim_rms"] <= cap
            add(cand, ok and not chosen, t_one.seconds)
            if ok and not chosen:
                chosen = cand
    return chosen, front


def reduction(before, after):
    return (1 - after / before) * 100 if before and after is not None else None


def synthesis(benchmark, exact, final, mode, max_delay, netlist, timing):
    jobs = {"exact": (exact, f"{mode}_exact"), "final": (final, f"{mode}_final")}
    with ui.working("Design Compiler: exact and final circuit", "orange"), ThreadPoolExecutor(len(jobs)) as pool:
        futures = {name: pool.submit(synthesise, benchmark, codes, tag, max_delay, netlist)
                   for name, (codes, tag) in jobs.items()}
        runs = {name: future.result() for name, future in futures.items()}

    report = {
        **runs,
        "netlist": netlist,
        "constraint": timing,
        "area_reduction_pct": reduction(runs["exact"]["total_area"], runs["final"]["total_area"]),
        "power_reduction_pct": reduction(runs["exact"]["total_mw"], runs["final"]["total_mw"]),
    }
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("benchmark", help="benchmark name, e.g. fir, fir_n25, adder_tree_64, mac_n10")
    parser.add_argument("--mode", choices=("sim", "analytical", "hybrid", "eem"), required=True,
                        help="where the error of each search step comes from (see above)")

    group = parser.add_argument_group("search")
    group.add_argument("--cap", type=float, default=5.0, metavar="PCT",
                       help="error cap: RMS error in %% of the largest output (default: 5)")
    group.add_argument("--budget", type=int, default=500, metavar="N",
                       help="MCTS iterations, per search in analytical; in hybrid, those of the simulation "
                            "search (default: 500)")
    group.add_argument("--rand-start", nargs="?", const="mild", choices=("mild", "medium"),
                       help="start from random units instead of all exact: the mildest third of each "
                            "node's units (mild, the default) or two thirds (medium), stepped down until "
                            "under the cap")
    group.add_argument("--seed", type=int, default=0, metavar="N",
                       help="seed of the search and the random start (default: 0)")

    group = parser.add_argument_group("analytical and hybrid")
    group.add_argument("--seeds", type=int, metavar="N",
                       help="number of searches with the model, one per seed; their results are the "
                            "candidates (default: 100)")
    group.add_argument("--analytical-budget", type=int, default=2000, metavar="N",
                       help="hybrid: MCTS iterations of each model search (default: 2000)")
    group.add_argument("--margin", type=float, metavar="PCT",
                       help="the model searches under cap - margin (default: 0 in analytical, 1 in hybrid)")
    group.add_argument("--verify-front", action="store_true",
                       help="simulate the whole Pareto front of the candidates, not only until one passes")
    group.add_argument("--seed-jobs", type=int, default=1, metavar="N",
                       help="analytical: run the seeds in N processes at once (default: 1, one after another)")
    group.add_argument("--rule", action="store_true",
                       help="analytical: allow add16u_0GK, 02E and 0MH only at adders whose mean exact output "
                            "reaches their first real sum bit")

    group = parser.add_argument_group("error")
    group.add_argument("--samples", type=int, default=1_000_000, metavar="N",
                       help="simulation samples (default: 1000000)")
    group.add_argument("--depth", type=int, choices=(1, 2), default=2,
                       help="table depth of the analytical model; falls back to 1 without a depth-2 "
                            "table (default: 2)")

    group = parser.add_argument_group("synthesis")
    group.add_argument("--synth", action="store_true",
                       help="synthesise the exact and the final configuration with Design Compiler")
    group.add_argument("--netlist", choices=("original", "generated"), default="original",
                       help="original: the benchmark's hand-written Verilog with the units swapped in "
                            "(default); generated: a netlist built from the benchmark JSON")
    group.add_argument("--max-delay", type=float, metavar="NS",
                       help="timing constraint in ns (default: the benchmark's own, from "
                            "script_sample.tcl or synth.json)")

    group = parser.add_argument_group("output")
    group.add_argument("--out", type=Path, metavar="FILE",
                       help="result JSON, the log goes next to it (default: results/dse/<benchmark>_<mode>.json)")
    group.add_argument("--quiet", action="store_true",
                       help="a progress bar instead of one line per iteration")
    args = parser.parse_args()
    searching = args.mode in ("analytical", "hybrid")
    for flag, value in (("--seeds", args.seeds), ("--margin", args.margin)):
        if value is not None and not searching:
            fail(f"{flag} only applies with --mode analytical or hybrid")
    if args.verify_front and not searching:
        fail("--verify-front only applies with --mode analytical or hybrid")
    args.seeds = 100 if args.seeds is None else args.seeds
    if args.margin is None:
        args.margin = 1.0 if args.mode == "hybrid" else 0.0
    if searching and not 0 <= args.margin < args.cap:
        fail("--margin must be at least 0 and smaller than --cap")
    if args.seeds < 1:
        fail("--seeds must be at least 1")
    if args.seed_jobs < 1:
        fail("--seed-jobs must be at least 1")
    if args.seed_jobs > 1 and args.mode != "analytical":
        fail("--seed-jobs only applies with --mode analytical")
    if args.rule and args.mode != "analytical":
        fail("--rule only applies with --mode analytical")
    if args.max_delay is not None and not args.synth:
        fail("--max-delay only applies with --synth")
    synth_timing = None
    if args.synth:
        # fail before the search, not after it
        if not dc_available():
            fail(f"--synth: Design Compiler not available (image {SIF_IMAGE})")
        try:
            template(args.benchmark)
            synth_timing = constraint(args.benchmark, args.netlist, args.max_delay)
            if args.netlist == "original":
                dfg = load_benchmark(args.benchmark)
                rewrite_original(original_netlist(args.benchmark).read_text(), dfg,
                                 {node.id: EXACT[node.op] for node in dfg.nodes}, load_library())
        except (FileNotFoundError, ValueError) as error:
            fail(f"--synth: {error}")

    run_start = time.perf_counter()
    timing = {}
    random.seed(args.seed)

    with Timer() as t_load:
        dfg = load_benchmark(args.benchmark)
        lib = load_library()
        areas = load_areas(lib)
        wire_max = compute_wire_max(dfg)
        m_out = wire_max[dfg.output]
    with Timer() as t_model:
        model_fn, source = analytical_error(dfg, args.benchmark, args.depth, wire_max, lib)
        model = Evaluator("analytical", model_fn)
    with Timer() as t_sim:
        sim = Evaluator("simulation", simulation_error(dfg, args.samples, m_out))
    eem, t_eem = None, Timer()
    if args.mode == "eem":
        try:
            with t_eem:
                eem = Evaluator("eem", eem_error(dfg, args.samples, m_out, lib))
        except (FileNotFoundError, ValueError) as error:
            fail(f"--mode eem: {error}")
    estimator = eem or model

    adds = sum(node.op == "add" for node in dfg.nodes)
    fields = [("benchmark", args.benchmark, f"{len(dfg.nodes)} nodes ({adds} add, {len(dfg.nodes) - adds} mul)"),
              ("mode", args.mode, ""),
              ("cap", f"{args.cap:g} %", f"margin {args.margin:g}" if searching and args.margin else "no margin")]
    if searching:
        budget = args.analytical_budget if args.mode == "hybrid" else args.budget
        fields.append(("analytical", f"{args.seeds} × {budget:,}",
                       "seeds × iter." + (f", {min(args.seed_jobs, args.seeds)} at once" if args.seed_jobs > 1 else "")
                       + (", range rule" if args.rule else "")))
    if args.mode in ("sim", "hybrid", "eem"):
        fields.append((args.mode if args.mode == "eem" else "simulation", f"{args.budget:,}", "iterations"))
    fields.append(("samples", f"{args.samples:,}", f"input seed {SEED_INPUTS}"))
    fields.append(("start", f"random {args.rand_start}" if args.rand_start else "all exact", f"seed {args.seed}"))
    fields.append(("synthesis", f"DC {args.netlist}" if args.synth else "off",
                   (describe(synth_timing) + (" (--max-delay)" if args.max_delay is not None else "")) if args.synth else ""))
    subtitle = {"sim": "MCTS · simulation", "analytical": "analytical model · MCTS · verified by simulation",
                "hybrid": "analytical model · MCTS · simulation", "eem": "EEM model · MCTS · simulation"}[args.mode]
    warnings = []
    if eem and eem.fn.different:
        warnings.append(f"EEM implements these units differently from the simulation: {', '.join(eem.fn.different)}")
    ui.header(fields, subtitle, warnings)

    first = sim if args.mode == "sim" else estimator
    search_cap = args.cap - args.margin if searching else args.cap
    units = {"mild": start_units(lib, areas, 1 / 3), "medium": start_units(lib, areas, 2 / 3)}
    checks = [estimator] if first is estimator else [estimator, first]

    def start_for(seed):
        return random_start(dfg, units, args.rand_start, checks, search_cap, seed)

    exact = {node.id: EXACT[node.op] for node in dfg.nodes}
    with Timer() as t_start:
        start = start_for(args.seed) if args.rand_start else dict(exact)
        start_rms = first(start)
    exact_area = circuit_area(exact, areas)
    timing["setup"] = t_load.seconds + t_model.seconds + t_sim.seconds + (t_eem.seconds if eem else 0.0) + t_start.seconds
    ui.setup(timing["setup"], f"EEM ({EEM_ROOT})" if eem else source,
             {"saving_pct": (1 - circuit_area(start, areas) / exact_area) * 100, "rms": start_rms}, first.name)
    ui.gap_line()

    phases = {}
    verified = start
    if args.mode in ("analytical", "hybrid"):
        budget = args.analytical_budget if args.mode == "hybrid" else args.budget
        seeds = range(args.seed, args.seed + args.seeds)
        per_seed = start_for if args.rand_start else lambda seed: start
        jobs = min(args.seed_jobs, args.seeds)
        phases["analytical"] = multi_search(model, per_seed, search_cap, budget, seeds, dfg, lib, areas,
                                            jobs, (args.benchmark, args.depth, args.rand_start, search_cap),
                                            rule=args.rule)
        timing["analytical search"] = phases["analytical"]["seconds"]

        name = "Handoff check" if args.mode == "hybrid" else "Verification"
        found = phases["analytical"]
        fallback = None
        with Timer() as t_verify:
            chosen, front = verify(found["candidates"], sim, args.cap, args.verify_front)
            if chosen:
                verified = chosen["codes"]
            elif start != exact and sim(start) <= args.cap:
                fallback = ("start", (1 - circuit_area(start, areas) / exact_area) * 100)
            else:
                verified = exact
                fallback = ("exact", 0.0)
        found["front"] = [{k: c.get(k) for k in ("area", "saving_pct", "rms", "sim_rms", "seed")} for c in front]
        found["chosen"] = chosen and {k: chosen.get(k) for k in ("area", "saving_pct", "rms", "sim_rms", "seed")}
        timing[name.lower()] = t_verify.seconds
        ui.verified("Hand-off check" if args.mode == "hybrid" else "Verification", t_verify.seconds,
                    len(found["candidates"]), [c for c in found["candidates"] if c.get("sim_rms") is not None],
                    args.cap, chosen, fallback)

    base = verified if args.mode == "hybrid" else start

    if args.mode == "eem":
        phases["eem"] = search(eem, start, args.cap, args.budget, dfg, lib, areas, args.quiet)
        timing["eem search"] = phases["eem"]["seconds"]
        ui.searched("EEM search", phases["eem"], (1 - phases["eem"]["base_area"] / exact_area) * 100)

    if args.mode in ("sim", "hybrid"):
        phases["simulation"] = search(sim, base, args.cap, args.budget, dfg, lib, areas, args.quiet)
        timing["simulation search"] = phases["simulation"]["seconds"]
        ui.searched("Simulation search", phases["simulation"], (1 - phases["simulation"]["base_area"] / exact_area) * 100)

    final_codes = verified if args.mode == "analytical" else list(phases.values())[-1]["codes"]
    searched_rms = sim.cached(final_codes)
    with Timer() as t_final, ui.working(f"final simulation on {args.samples:,} samples"):
        final_rms = sim.fresh(final_codes)
    final_area = circuit_area(final_codes, areas)
    final = {"codes": final_codes, "sim_rms": final_rms, "area": final_area, "exact_area": exact_area,
             "saving_pct": (1 - final_area / exact_area) * 100}
    if args.mode != "sim":
        final["model"] = estimator.name
        final["model_rms"] = estimator(final_codes)
        final["model_inaccuracy_pct"] = abs(final["model_rms"] - final_rms) / final_rms * 100 if final_rms else 0.0
    timing["final simulation"] = t_final.seconds
    ui.final(final, args.cap, args.samples, t_final.seconds, searched_rms)

    synth = None
    if args.synth:
        with Timer() as t_synth:
            synth = synthesis(args.benchmark, exact, final_codes, args.mode, args.max_delay, args.netlist, synth_timing)
        timing["synthesis"] = t_synth.seconds
        ui.synthesised(synth, t_synth.seconds)

    total_seconds = time.perf_counter() - run_start

    out = args.out or RESULTS / f"{args.benchmark}_{args.mode}.json"
    log = out.with_suffix(".log")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({
        "benchmark": args.benchmark,
        "args": {k: str(v) if isinstance(v, Path) else v for k, v in vars(args).items()},
        "start": start,
        "phases": phases,
        "final": final,
        "synthesis": synth,
        "timing": {**timing, "total": total_seconds},
        "evaluations": {ev.name: {"calls": ev.calls, "cache_hits": ev.hits, "seconds": ev.seconds,
                                  "mean_seconds": ev.mean()} for ev in (sim, estimator)},
    }, indent=1))
    ui.results(final, args.cap, synth, {**timing, "total": total_seconds}, {"simulation": sim, estimator.name: estimator},
               phases, out, log)
    ui.save(log)


if __name__ == "__main__":
    main()
