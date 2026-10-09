"""
Design space exploration with MCTS, using simulation, the analytical model, or both.

    python3 pipeline.py <benchmark> --mode sim|analytical|hybrid|eem [options]

Modes:
    sim         every search step is simulated
    analytical  every search step uses the analytical model
    hybrid      the analytical model searches first, once per seed (--seeds), with the cap lowered
                by --margin; its results are simulated smallest area first, and the simulation
                continues the search from the first one under the cap
    eem         every search step uses the EEM model (../EEM/EEM, or EEM_PATH)

Start configuration (--start):
    root        all units exact (default)
    random      a random configuration under the cap
    FILE        a JSON file with a codes dict, {"codes": {...}}, or a list of named
    FILE:NAME   configurations ("configs"), from which NAME is picked
"""
import argparse
import json
import random
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import numpy as np
from rich import box
from rich.console import Console
from rich.progress import (
    BarColumn,
    MofNCompleteColumn,
    Progress,
    TextColumn,
    TimeElapsedColumn,
)
from rich.table import Table

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
from model.data import DATA, load_benchmark, load_deep_table, load_library, read_json
from model.propagate import compute_wire_max, propagate_circuit_scalar, scalar_rms
from sim.simulate import (
    EXACT,
    SEED_INPUTS,
    UNITS,
    random_config,
    sample_inputs,
    simulate,
)

RESULTS = Path(__file__).resolve().parent / "results" / "dse"
RANDOM_TRIES = 1000
console = Console(record=True)
progress_console = Console(stderr=True)  # keeps the progress bar out of the log


def duration(seconds):
    if seconds < 1e-3:
        return f"{seconds * 1e6:.1f} µs"
    if seconds < 1:
        return f"{seconds * 1e3:.1f} ms"
    if seconds < 60:
        return f"{seconds:.2f} s"
    if seconds < 3600:
        return f"{seconds / 60:.1f} min"
    return f"{seconds / 3600:.2f} h"


def fail(message):
    console.print(f"[bold red]error:[/] {message}")
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
    node_lib = read_json(DATA / "node_metrics" / f"{benchmark}.json")
    feeder_lib = read_json(DATA / "node_metrics" / f"{benchmark}_feeder.json")
    deep_lib = load_deep_table(benchmark) if depth == 2 else None
    if deep_lib:
        source = "node + depth-2 feeder tables"
    elif feeder_lib:
        source = "node + feeder tables"
    elif node_lib:
        source = "node tables"
    else:
        source = f"range libraries [yellow](no node tables, run python3 -m characterization.characterize {benchmark})[/]"
    if depth == 2 and deep_lib is None:
        source += f" [yellow](no {benchmark}_d2 table, using depth 1)[/]"
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
        return scalar_rms(propagate_circuit_scalar(dfg, codes, *tables)[dfg.output], m_out)

    return rms, source


def load_start_file(spec, dfg):
    path, _, name = spec.partition(":")
    data = read_json(Path(path))
    if data is None:
        fail(f"start file {path} not found")
    if name:
        configs = data.get("configs")
        if isinstance(configs, list):
            configs = {c["name"]: c["codes"] for c in configs}
        if not configs or name not in configs:
            fail(f"no configuration named {name!r} in {path}")
        codes = configs[name]
    else:
        codes = data.get("codes", data)

    ops = {node.id: node.op for node in dfg.nodes}
    unknown = set(codes) - set(ops)
    if unknown:
        fail(f"start configuration names unknown nodes: {', '.join(sorted(unknown))}")
    start = {nid: EXACT[op] for nid, op in ops.items()}
    for nid, code in codes.items():
        code = int(code)
        if code != EXACT[ops[nid]] and code not in UNITS[ops[nid]]:
            fail(f"start configuration: code {code} is not a {ops[nid]} unit ({nid})")
        start[nid] = code
    return start


def start_config(args, dfg, error_fn):
    if args.start == "root":
        return {node.id: EXACT[node.op] for node in dfg.nodes}
    if args.start == "random":
        rng = np.random.default_rng(args.seed)
        for _ in range(RANDOM_TRIES):
            codes = random_config(dfg, rng)
            if error_fn(codes) <= args.cap:
                return codes
        fail(f"no random configuration under {args.cap} % in {RANDOM_TRIES} draws")
    codes = load_start_file(args.start, dfg)
    rms = error_fn(codes)
    if rms > args.cap:
        fail(f"start configuration is already over the cap ({error_fn.name} RMS {rms:.3f} % > {args.cap} %)")
    return codes


def approximated(codes, lib):
    return [f"{nid}={lib[code].name}" for nid, code in codes.items() if code != EXACT[lib[code].op]]


def search(error_fn, base, cap, budget, dfg, lib, areas, quiet):
    console.print(f"  evaluator    : [cyan]{error_fn.name}[/]   cap [yellow]{cap:.3f} %[/]   budget [cyan]{budget}[/] iterations")
    snap = error_fn.snapshot()
    exact_area = circuit_area({node.id: EXACT[node.op] for node in dfg.nodes}, areas)

    def line(it, child, best):
        move = f"{child.node_id}={child.name}"
        if child.dead:
            status = f"[red]RMS {child.rms:7.3f} %  over cap[/]"
        else:
            status = f"[green]RMS {child.rms:7.3f} %[/]  area {child.area:9.1f}"
        mark = "  [bold green]new best[/]" if best is child else ""
        console.print(f"  [dim]iter {it:>6}  level {child.level:>3}[/]  {status}  [dim]{move}[/]{mark}")

    with Timer() as timer:
        if quiet:
            columns = (TextColumn("  {task.description}"), BarColumn(), MofNCompleteColumn(), TimeElapsedColumn())
            with Progress(*columns, console=progress_console, transient=True) as progress:
                task = progress.add_task(error_fn.name, total=budget + 1)
                result = run_mcts([n.id for n in dfg.nodes], variants(dfg, lib, areas), base, error_fn,
                                  lambda codes: circuit_area(codes, areas), cap, budget,
                                  on_step=lambda it, child, best: progress.update(task, completed=it + 1))
        else:
            result = run_mcts([n.id for n in dfg.nodes], variants(dfg, lib, areas), base, error_fn,
                              lambda codes: circuit_area(codes, areas), cap, budget, on_step=line)

    evals = error_fn.since(snap)
    result.update(evaluator=error_fn.name, cap=cap, budget=budget, seconds=timer.seconds,
                  evaluations=evals["calls"], cache_hits=evals["hits"], eval_seconds=evals["seconds"],
                  search_seconds=timer.seconds - evals["seconds"],
                  saving_pct=(1 - result["area"] / exact_area) * 100)
    mean = evals["seconds"] / evals["calls"] if evals["calls"] else 0.0
    console.print(f"  -> best RMS [green]{result['rms']:.3f} %[/] ({error_fn.name})   library area "
                  f"[cyan]{result['area']:.1f}[/] ([green]{result['saving_pct']:.1f} %[/] saved)")
    console.print(f"     {result['iterations']} iterations, {result['evaluations']} evaluations, "
                  f"{result['cache_hits']} cache hits   [bold]{duration(timer.seconds)}[/] "
                  f"[dim](evaluation {duration(evals['seconds'])}, mean {duration(mean)} · "
                  f"tree search {duration(result['search_seconds'])})[/]")
    console.print()
    return result


def multi_search(error_fn, base, cap, budget, seeds, dfg, lib, areas):
    # one search per seed; the distinct results are the handoff candidates
    console.print(f"  evaluator    : [cyan]{error_fn.name}[/]   cap [yellow]{cap:.3f} %[/]   budget [cyan]{budget}[/] "
                  f"iterations x [cyan]{len(seeds)}[/] seeds")
    snap = error_fn.snapshot()
    exact_area = circuit_area({node.id: EXACT[node.op] for node in dfg.nodes}, areas)
    moves = variants(dfg, lib, areas)
    candidates, iterations = {}, 0
    with Timer() as timer:
        columns = (TextColumn("  {task.description}"), BarColumn(), MofNCompleteColumn(), TimeElapsedColumn())
        with Progress(*columns, console=progress_console, transient=True) as progress:
            task = progress.add_task(f"{error_fn.name} seeds", total=len(seeds))
            for seed in seeds:
                random.seed(seed)
                r = run_mcts([n.id for n in dfg.nodes], moves, base, error_fn,
                             lambda codes: circuit_area(codes, areas), cap, budget)
                iterations += r["iterations"]
                candidates.setdefault(tuple(sorted(r["codes"].items())),
                                      {"codes": r["codes"], "rms": r["rms"], "area": r["area"], "seed": seed})
                progress.update(task, advance=1)
    ranked = sorted(candidates.values(), key=lambda c: c["area"])
    for c in ranked:
        c["saving_pct"] = (1 - c["area"] / exact_area) * 100
    evals = error_fn.since(snap)
    best = ranked[0]
    result = {**best, "candidates": ranked, "seeds": len(seeds), "evaluator": error_fn.name, "cap": cap,
              "budget": budget, "iterations": iterations, "seconds": timer.seconds, "evaluations": evals["calls"],
              "cache_hits": evals["hits"], "eval_seconds": evals["seconds"],
              "search_seconds": timer.seconds - evals["seconds"]}
    console.print(f"  -> {len(seeds)} seeds, {len(ranked)} distinct candidates: saving "
                  f"{ranked[-1]['saving_pct']:.1f} to {best['saving_pct']:.1f} %, model RMS "
                  f"{min(c['rms'] for c in ranked):.3f} to {max(c['rms'] for c in ranked):.3f} %")
    console.print(f"     {iterations} iterations, {evals['calls']} evaluations, {evals['hits']} cache hits   "
                  f"[bold]{duration(timer.seconds)}[/] [dim](evaluation {duration(evals['seconds'])} · "
                  f"tree search {duration(result['search_seconds'])})[/]")
    console.print()
    return result


def reduction(before, after):
    return (1 - after / before) * 100 if before and after is not None else None


def synthesis(benchmark, exact, final, mode, max_delay, netlist, timing):
    console.print(f"  netlist      : [cyan]{netlist}[/]   {describe(timing)}   two Design Compiler runs in parallel")
    jobs = {"exact": (exact, f"{mode}_exact"), "final": (final, f"{mode}_final")}
    with progress_console.status("  running Design Compiler ..."), ThreadPoolExecutor(len(jobs)) as pool:
        futures = {name: pool.submit(synthesise, benchmark, codes, tag, max_delay, netlist)
                   for name, (codes, tag) in jobs.items()}
        runs = {name: future.result() for name, future in futures.items()}

    table = Table(box=box.SIMPLE_HEAVY)
    for column in ("Config", "Total area", "Cell area", "Power", "Slack", "DC time"):
        table.add_column(column, justify="left" if column == "Config" else "right")
    for name, run in runs.items():
        if not run["ran"]:
            table.add_row(name, "[red]failed[/]", "", "", "", duration(run["seconds"] or 0.0))
            continue
        slack = run["slack"]
        slack_text = "-" if slack is None else f"[{'green' if slack >= 0 else 'red'}]{slack:.2f} ns[/]"
        power = "-" if run["total_mw"] is None else f"{run['total_mw']:.4f} mW"
        table.add_row(name, f"{run['total_area']:.1f}", f"{run['cell_area']:.1f}", power, slack_text,
                      duration(run["seconds"]))
    console.print(table)

    report = {
        **runs,
        "netlist": netlist,
        "constraint": timing,
        "area_reduction_pct": reduction(runs["exact"]["total_area"], runs["final"]["total_area"]),
        "power_reduction_pct": reduction(runs["exact"]["total_mw"], runs["final"]["total_mw"]),
    }
    for name, run in runs.items():
        if not run["ran"]:
            console.print(f"  [red]{name}: Design Compiler did not write an area report, see {run['folder']}/dc.log[/]")
        elif run["slack"] is not None and run["slack"] < 0:
            console.print(f"  [yellow]{name}: timing violated (slack {run['slack']} ns), loosen it with --max-delay[/]")
    if report["area_reduction_pct"] is not None:
        console.print(f"  area reduction  : [bold green]{report['area_reduction_pct']:.1f} %[/]")
    if report["power_reduction_pct"] is not None:
        console.print(f"  power reduction : [bold green]{report['power_reduction_pct']:.1f} %[/]")
    console.print()
    return report


def render_summary(args, phases, final, sim, estimator, timing, synth, total_seconds):
    console.rule("[bold blue]SUMMARY[/]")

    table = Table(box=box.SIMPLE_HEAVY, title="[bold]Search phases[/]")
    for column in ("Phase", "Cap", "Iterations", "Evaluations", "Best RMS", "Library area", "Saving", "Time"):
        table.add_column(column, justify="left" if column == "Phase" else "right")
    for name, phase in phases.items():
        table.add_row(name, f"{phase['cap']:.2f} %", str(phase["iterations"]), str(phase["evaluations"]),
                      f"{phase['rms']:.3f} %", f"{phase['area']:.1f}", f"{phase['saving_pct']:.1f} %",
                      duration(phase["seconds"]))
    console.print(table)

    table = Table(box=box.SIMPLE_HEAVY, title="[bold]Timing[/]")
    for column in ("Stage", "Time", "Share"):
        table.add_column(column, justify="left" if column == "Stage" else "right")
    for name, seconds in timing.items():
        table.add_row(name, duration(seconds), f"{seconds / total_seconds * 100:.1f} %")
    table.add_section()
    table.add_row("[bold]total[/]", f"[bold]{duration(total_seconds)}[/]", "100.0 %")
    console.print(table)

    table = Table(box=box.SIMPLE_HEAVY, title="[bold]Error evaluators[/]")
    for column in ("Evaluator", "Calls", "Cache hits", "Total time", "Mean per call"):
        table.add_column(column, justify="left" if column == "Evaluator" else "right")
    for ev in (sim, estimator):
        mean = ev.mean()
        table.add_row(ev.name, str(ev.calls), str(ev.hits), duration(ev.seconds), duration(mean) if mean else "-")
    console.print(table)
    if sim.mean() and estimator.mean():
        ratio = sim.mean() / estimator.mean()
        speed = f"[bold green]{ratio:,.0f}× faster[/]" if ratio >= 1 else f"[bold yellow]{1 / ratio:,.1f}× slower[/]"
        console.print(f"{estimator.name} evaluation is {speed} than simulation ({duration(estimator.mean())} over "
                      f"{estimator.calls} calls vs {duration(sim.mean())} over {sim.calls})")

    console.print()
    within = final["sim_rms"] <= args.cap
    verdict = "[green]within cap[/]" if within else "[bold red]over cap[/]"
    console.print(f"final RMS (simulated)  : [bold]{final['sim_rms']:.3f} %[/]   cap {args.cap} %   {verdict}")
    if final.get("model_rms") is not None:
        console.print(f"{'final RMS (' + estimator.name + ')':<23}: {final['model_rms']:.3f} %   "
                      f"inaccuracy {final['model_inaccuracy_pct']:.2f} %")
    if synth and synth["area_reduction_pct"] is not None:
        exact_run, final_run = synth["exact"], synth["final"]
        console.print(f"area (DC)              : {final_run['total_area']:.1f} of {exact_run['total_area']:.1f}   "
                      f"reduction [bold green]{synth['area_reduction_pct']:.1f} %[/]")
        if synth["power_reduction_pct"] is not None:
            console.print(f"power (DC)             : {final_run['total_mw']:.4f} of {exact_run['total_mw']:.4f} mW   "
                          f"reduction [bold green]{synth['power_reduction_pct']:.1f} %[/]")
    else:
        console.print(f"library area           : {final['area']:.1f} of {final['exact_area']:.1f}   "
                      f"saving [bold green]{final['saving_pct']:.1f} %[/]")
    units = approximated(final["codes"], final["lib"])
    console.print(f"approximated units     : {len(units)} of {len(final['codes'])}")
    console.print(f"[dim]{', '.join(units) or '(none)'}[/]")
    console.print(f"\ntotal time : [bold]{duration(total_seconds)}[/]")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("benchmark")
    parser.add_argument("--mode", choices=("sim", "analytical", "hybrid", "eem"), required=True)
    parser.add_argument("--cap", type=float, default=5.0, help="RMS error cap in %% of the maximum output (default 5)")
    parser.add_argument("--budget", type=int, default=500,
                        help="MCTS iterations with simulation, or with the model in analytical and eem mode")
    parser.add_argument("--analytical-budget", type=int, default=2000,
                        help="MCTS iterations of the analytical phase in hybrid mode")
    parser.add_argument("--margin", type=float, default=1.0,
                        help="hybrid: the analytical phase searches under cap - margin (RMS %%)")
    parser.add_argument("--seeds", type=int,
                        help="hybrid: analytical searches with seeds seed..seed+N-1, their results are the handoff "
                             "candidates (default 100)")
    parser.add_argument("--handoff-sims", type=int, default=10,
                        help="hybrid: candidates simulated at most, smallest area first, until one is under the cap")
    parser.add_argument("--start", default="root", help="root, random, FILE or FILE:NAME (default root)")
    parser.add_argument("--samples", type=int, default=1_000_000,
                        help="simulation samples, in the search and the final check (default 1000000)")
    parser.add_argument("--depth", type=int, choices=(1, 2), default=2,
                        help="analytical feeder table depth (default 2; falls back to 1 without a depth-2 table)")
    parser.add_argument("--seed", type=int, default=0, help="seed of the search and of --start random")
    parser.add_argument("--out", type=Path, help="result file (default results/dse/<benchmark>_<mode>.json)")
    parser.add_argument("--quiet", action="store_true", help="a progress bar instead of one line per iteration")
    parser.add_argument("--synth", action="store_true",
                        help="synthesise the exact and the final configuration with Design Compiler at the end")
    parser.add_argument("--netlist", choices=("original", "generated"), default="original",
                        help="with --synth: original = units substituted into *_original.v (default); "
                             "generated = netlist built from the DFG")
    parser.add_argument("--max-delay", type=float,
                        help="with --synth: set_max_delay in ns (default: script_sample.tcl for original, "
                             "synth.json for generated)")
    args = parser.parse_args()
    if args.mode == "hybrid" and args.margin >= args.cap:
        fail("--margin must be smaller than --cap")
    if args.seeds is not None and args.mode != "hybrid":
        fail("--seeds only applies with --mode hybrid")
    args.seeds = 100 if args.seeds is None else args.seeds
    if args.seeds < 1 or args.handoff_sims < 1:
        fail("--seeds and --handoff-sims must be at least 1")
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

    stages = ["Setup"]
    stages += {"sim": ["Simulation search"], "analytical": ["Analytical search"],
               "hybrid": ["Analytical search", "Handoff check", "Simulation search"], "eem": ["EEM search"]}[args.mode]
    stages += ["Final simulation"] + (["Synthesis"] if args.synth else [])
    stage_no = iter(range(1, len(stages) + 1))

    def stage(name):
        console.print(f"[bold]Stage {next(stage_no)}/{len(stages)}[/]  {name}")

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
    console.rule("[bold blue]DSE PIPELINE[/]")
    console.print(f"  benchmark    : [cyan]{args.benchmark}[/]   ({len(dfg.nodes)} nodes: {adds} add, "
                  f"{len(dfg.nodes) - adds} mul, M_out {m_out:,.0f})")
    described = {"sim": "simulation only", "analytical": "analytical model only",
                 "hybrid": "analytical search, then simulation", "eem": "EEM model only"}[args.mode]
    console.print(f"  mode         : [cyan]{args.mode}[/]   ({described})")
    if args.mode == "hybrid":
        console.print(f"  cap          : [yellow]{args.cap} %[/]   margin [yellow]{args.margin} %[/]  ->  "
                      f"analytical cap [yellow]{args.cap - args.margin:g} %[/]")
        seeds = f" x [cyan]{args.seeds}[/] seeds"
        console.print(f"  budget       : [cyan]{args.analytical_budget}[/] analytical{seeds} + [cyan]{args.budget}[/] "
                      f"simulation iterations, up to [cyan]{args.handoff_sims}[/] handoff simulations")
    else:
        console.print(f"  cap          : [yellow]{args.cap} %[/]")
        console.print(f"  budget       : [cyan]{args.budget}[/] iterations")
    console.print(f"  start        : [cyan]{args.start}[/]   seed [cyan]{args.seed}[/]")
    console.print(f"  simulation   : [cyan]{args.samples:,}[/] samples (input seed {SEED_INPUTS})")
    console.print(f"  model        : {f'EEM ({EEM_ROOT})' if eem else source}")
    if eem and eem.fn.different:
        console.print(f"  [yellow]EEM implements these units differently from the simulation: {', '.join(eem.fn.different)}[/]")
    console.print("  area         : unit area library" + (f" + Design Compiler at the end ([cyan]{args.netlist}[/] "
                  f"netlist, {describe(synth_timing)})" if args.synth else " [dim](no synthesis)[/]"))
    console.print()

    stage("Setup")
    with Timer() as t_start:
        first = sim if args.mode == "sim" else estimator
        start = start_config(args, dfg, first)
        start_rms = first(start)
    exact = {node.id: EXACT[node.op] for node in dfg.nodes}
    exact_area = circuit_area(exact, areas)
    console.print(f"  load benchmark and libraries : {duration(t_load.seconds)}")
    console.print(f"  analytical tables            : {duration(t_model.seconds)}")
    if eem:
        console.print(f"  EEM model and input PMFs     : {duration(t_eem.seconds)}")
    console.print(f"  simulation inputs            : {duration(t_sim.seconds)}")
    console.print(f"  start configuration          : {duration(t_start.seconds)}   "
                  f"library area {circuit_area(start, areas):.1f} of {exact_area:.1f}, {first.name} RMS {start_rms:.3f} %")
    timing["setup"] = t_load.seconds + t_model.seconds + t_sim.seconds + (t_eem.seconds if eem else 0.0) + t_start.seconds
    console.print()

    phases = {}
    if args.mode in ("analytical", "hybrid"):
        stage("Analytical search")
        cap = args.cap - args.margin if args.mode == "hybrid" else args.cap
        budget = args.analytical_budget if args.mode == "hybrid" else args.budget
        if args.mode == "hybrid":
            seeds = range(args.seed, args.seed + args.seeds)
            phases["analytical"] = multi_search(model, start, cap, budget, seeds, dfg, lib, areas)
        else:
            phases["analytical"] = search(model, start, cap, budget, dfg, lib, areas, args.quiet)
        timing["analytical search"] = phases["analytical"]["seconds"]

    base = start
    if args.mode == "hybrid":
        stage("Handoff check")
        found = phases["analytical"]
        candidates = found.get("candidates") or [{"codes": found["codes"], "rms": found["rms"], "area": found["area"],
                                                  "saving_pct": found["saving_pct"]}]
        # smallest area first; the first one under the cap is handed over
        tried, chosen = [], None
        with Timer() as t_handoff:
            for i, cand in enumerate(candidates[:args.handoff_sims], 1):
                with Timer() as t_one:
                    real = sim(cand["codes"])
                tried.append({"area": cand["area"], "model_rms": cand["rms"], "sim_rms": real})
                ok = real <= args.cap
                console.print(f"  candidate {i:>2}: saving {cand['saving_pct']:5.1f} %   model [cyan]{cand['rms']:.3f} %[/]  ->  "
                              f"simulated [cyan]{real:.3f} %[/]   {'[green]within cap[/]' if ok else '[red]over cap[/]'}   "
                              f"{duration(t_one.seconds)}")
                if ok:
                    chosen = cand
                    break
        found["handoff"] = tried
        if chosen:
            base = chosen["codes"]
            found["sim_rms"] = tried[-1]["sim_rms"]
            console.print(f"  -> candidate {len(tried)} handed over: saving {chosen['saving_pct']:.1f} %, "
                          f"simulated {tried[-1]['sim_rms']:.3f} %")
        else:
            console.print(f"  [yellow]none of {len(tried)} candidates is under the cap ({args.cap} %), "
                          f"the simulation starts from the start configuration[/]")
        timing["handoff check"] = t_handoff.seconds
        console.print()

    if args.mode == "eem":
        stage("EEM search")
        phases["eem"] = search(eem, start, args.cap, args.budget, dfg, lib, areas, args.quiet)
        timing["eem search"] = phases["eem"]["seconds"]

    if args.mode in ("sim", "hybrid"):
        stage("Simulation search")
        phases["simulation"] = search(sim, base, args.cap, args.budget, dfg, lib, areas, args.quiet)
        timing["simulation search"] = phases["simulation"]["seconds"]

    stage("Final simulation")
    final_codes = list(phases.values())[-1]["codes"]
    searched_rms = sim.cached(final_codes)
    with Timer() as t_final:
        final_rms = sim.fresh(final_codes)
    final_area = circuit_area(final_codes, areas)
    final = {"codes": final_codes, "sim_rms": final_rms, "area": final_area, "exact_area": exact_area,
             "saving_pct": (1 - final_area / exact_area) * 100}
    if args.mode != "sim":
        final["model"] = estimator.name
        final["model_rms"] = estimator(final_codes)
        final["model_inaccuracy_pct"] = abs(final["model_rms"] - final_rms) / final_rms * 100 if final_rms else 0.0
    cached = ""
    if searched_rms is not None:
        same = abs(searched_rms - final_rms) < 1e-9
        cached = (" [dim](same as during the search)[/]" if same
                  else f" [yellow](search gave {searched_rms:.3f} %)[/]")
    verdict = "[green]within cap[/]" if final_rms <= args.cap else "[bold red]over cap[/]"
    console.print(f"  simulated RMS [bold]{final_rms:.3f} %[/] on {args.samples:,} samples   cap {args.cap} %   "
                  f"{verdict}   {duration(t_final.seconds)}{cached}")
    timing["final simulation"] = t_final.seconds
    console.print()

    synth = None
    if args.synth:
        stage("Synthesis")
        with Timer() as t_synth:
            synth = synthesis(args.benchmark, exact, final_codes, args.mode, args.max_delay, args.netlist, synth_timing)
        timing["synthesis"] = t_synth.seconds

    total_seconds = time.perf_counter() - run_start
    render_summary(args, phases, {**final, "lib": lib}, sim, estimator, timing, synth, total_seconds)

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
    console.print(f"\n[dim]result JSON  : {out}[/]")
    console.print(f"[dim]terminal log : {log}[/]")
    console.save_text(log)


if __name__ == "__main__":
    main()
