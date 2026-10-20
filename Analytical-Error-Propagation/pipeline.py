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
from rich import box
from rich.align import Align
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
from model.data import (
    D1,
    DATA,
    load_benchmark,
    load_deep_table,
    load_library,
    read_json,
)
from model.propagate import circuit_rms, compute_wire_max, propagate_circuit_scalar
from sim.simulate import (
    EXACT,
    SEED_INPUTS,
    UNITS,
    sample_inputs,
    simulate,
)

RESULTS = Path(__file__).resolve().parent / "results" / "dse"
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


def seed_search(seed, dfg, moves, areas, error_fn, start_for, cap, budget):
    # one model search from start_for(seed)
    base = start_for(seed)
    random.seed(seed)
    r = run_mcts([n.id for n in dfg.nodes], moves, base, error_fn, lambda codes: circuit_area(codes, areas), cap, budget)
    return {"seed": seed, "codes": r["codes"], "rms": r["rms"], "area": r["area"], "iterations": r["iterations"],
            "start_area": circuit_area(base, areas)}


worker = {}


def init_seed_worker(benchmark, depth, level, cap):
    # each process of --seed-jobs loads the benchmark and the model once
    dfg = load_benchmark(benchmark)
    lib = load_library()
    areas = load_areas(lib)
    model = Evaluator("analytical", analytical_error(dfg, benchmark, depth, compute_wire_max(dfg), lib)[0])
    units = {"mild": start_units(lib, areas, 1 / 3), "medium": start_units(lib, areas, 2 / 3)}
    exact = {node.id: EXACT[node.op] for node in dfg.nodes}
    start_for = (lambda seed: random_start(dfg, units, level, [model], cap, seed)) if level else (lambda seed: dict(exact))
    worker.update(dfg=dfg, moves=variants(dfg, lib, areas), areas=areas, model=model, start_for=start_for, cap=cap)


def worker_seed_search(seed, budget):
    w = worker
    snap = w["model"].snapshot()
    r = seed_search(seed, w["dfg"], w["moves"], w["areas"], w["model"], w["start_for"], w["cap"], budget)
    return {**r, "evals": w["model"].since(snap)}


def multi_search(error_fn, start_for, cap, budget, seeds, dfg, lib, areas, jobs=1, worker_args=None):
    # one search per seed from start_for(seed); the distinct results are the handoff candidates.
    # With jobs > 1 the seeds run in that many processes, set up by init_seed_worker(*worker_args)
    console.print(f"  evaluator    : [cyan]{error_fn.name}[/]   cap [yellow]{cap:.3f} %[/]   budget [cyan]{budget}[/] "
                  f"iterations x [cyan]{len(seeds)}[/] seeds" + (f" in [cyan]{jobs}[/] processes" if jobs > 1 else ""))
    snap = error_fn.snapshot()
    exact_area = circuit_area({node.id: EXACT[node.op] for node in dfg.nodes}, areas)
    moves = variants(dfg, lib, areas)
    candidates, iterations, start_areas = {}, 0, []
    with Timer() as timer:
        columns = (TextColumn("  {task.description}"), BarColumn(), MofNCompleteColumn(), TimeElapsedColumn())
        with Progress(*columns, console=progress_console, transient=True) as progress:
            task = progress.add_task(f"{error_fn.name} seeds", total=len(seeds))
            if jobs > 1:
                with ProcessPoolExecutor(jobs, initializer=init_seed_worker, initargs=worker_args) as pool:
                    futures = [pool.submit(worker_seed_search, seed, budget) for seed in seeds]
                    for future in as_completed(futures):
                        progress.update(task, advance=1)
                    results = [future.result() for future in futures]
                for r in results:
                    error_fn.calls += r["evals"]["calls"]
                    error_fn.hits += r["evals"]["hits"]
                    error_fn.seconds += r["evals"]["seconds"]
            else:
                results = []
                for seed in seeds:
                    results.append(seed_search(seed, dfg, moves, areas, error_fn, start_for, cap, budget))
                    progress.update(task, advance=1)
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
    if min(start_areas) < exact_area:
        console.print(f"     starts save {(1 - max(start_areas) / exact_area) * 100:.1f} to "
                      f"{(1 - min(start_areas) / exact_area) * 100:.1f} %")
    console.print(f"  -> {len(seeds)} seeds, {len(ranked)} distinct candidates: saving "
                  f"{ranked[-1]['saving_pct']:.1f} to {best['saving_pct']:.1f} %, model RMS "
                  f"{min(c['rms'] for c in ranked):.3f} to {max(c['rms'] for c in ranked):.3f} %")
    split = (f"evaluation {duration(evals['seconds'])} · tree search {duration(result['search_seconds'])}" if jobs == 1
             else f"{jobs} processes, evaluation {duration(evals['seconds'])} summed over them")
    console.print(f"     {iterations} iterations, {evals['calls']} evaluations, {evals['hits']} cache hits   "
                  f"[bold]{duration(timer.seconds)}[/] [dim]({split})[/]")
    console.print()
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
    console.print(f"  {len(candidates)} candidates, {len(front)} on the Pareto front (area vs model error)")
    chosen = None
    for i, cand in enumerate(order, 1):
        keep_going = whole_front and i <= len(front)
        if chosen and not keep_going:
            break
        if i == len(front) + 1:
            console.print("  no candidate on the front is under the cap, trying the others, smallest area first")
        with Timer() as t_one:
            cand["sim_rms"] = sim(cand["codes"])
        ok = cand["sim_rms"] <= cap
        mark = "  [bold green]chosen[/]" if ok and not chosen else ""
        console.print(f"  candidate {i:>2}: saving {cand['saving_pct']:5.1f} %   model [cyan]{cand['rms']:.3f} %[/]  ->  "
                      f"simulated [cyan]{cand['sim_rms']:.3f} %[/]   {'[green]within cap[/]' if ok else '[red]over cap[/]'}"
                      f"   {duration(t_one.seconds)}{mark}")
        if ok and not chosen:
            chosen = cand
    return chosen, front


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


# summary colours
TITLE, HEADER, BORDER = "bold #d4a373", "bold #e6c9a8", "#8a7360"
NAME, ERROR, GOOD, TIME = "#a3b18a", "#e9c46a", "#95c47a", "#c9a98a"


def divider():
    console.print("─" * (console.width // 2), style="grey35", justify="center")


def summary_table(title, columns):
    table = Table(box=box.SIMPLE_HEAVY, title=f"[{TITLE}]{title}[/]", header_style=HEADER, border_style=BORDER)
    for i, column in enumerate(columns):
        table.add_column(column, justify="left" if i == 0 else "right", style=NAME if i == 0 else None)
    return table


def render_result(args, final, estimator, synth):
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
    console.print()


def render_summary(args, phases, final, sim, estimator, timing, synth, total_seconds):
    render_result(args, final, estimator, synth)
    console.rule("[bold blue]SUMMARY[/]")

    table = summary_table("Search phases", ("Phase", "Cap", "Iterations", "Evaluations", "Best RMS", "Library area",
                                            "Saving", "Time"))
    for name, phase in phases.items():
        table.add_row(name, f"{phase['cap']:.2f} %", str(phase["iterations"]), str(phase["evaluations"]),
                      f"[{ERROR}]{phase['rms']:.3f} %[/]", f"{phase['area']:.1f}",
                      f"[{GOOD}]{phase['saving_pct']:.1f} %[/]", f"[{TIME}]{duration(phase['seconds'])}[/]")
    console.print(Align.center(table))
    divider()

    table = summary_table("Timing", ("Stage", "Time", "Share"))
    for name, seconds in timing.items():
        table.add_row(name, f"[{TIME}]{duration(seconds)}[/]", f"{seconds / total_seconds * 100:.1f} %")
    table.add_section()
    table.add_row("[bold]total[/]", f"[bold {TIME}]{duration(total_seconds)}[/]", "100.0 %")
    console.print(Align.center(table))
    divider()

    table = summary_table("Error evaluators", ("Evaluator", "Calls", "Cache hits", "Total time", "Mean per call"))
    for ev in (sim, estimator):
        mean = ev.mean()
        table.add_row(ev.name, str(ev.calls), str(ev.hits), f"[{TIME}]{duration(ev.seconds)}[/]",
                      f"[{TIME}]{duration(mean)}[/]" if mean else "-")
    console.print(Align.center(table))
    if sim.mean() and estimator.mean():
        ratio = sim.mean() / estimator.mean()
        speed = f"[bold {GOOD}]{ratio:,.0f}× faster[/]" if ratio >= 1 else f"[bold {ERROR}]{1 / ratio:,.1f}× slower[/]"
        console.print(f"{estimator.name} evaluation is {speed} than simulation ({duration(estimator.mean())} over "
                      f"{estimator.calls} calls vs {duration(sim.mean())} over {sim.calls})", justify="center")

    console.print(f"[{NAME}]total time[/] : [bold {TIME}]{duration(total_seconds)}[/]", justify="center")


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
    stages += {"sim": ["Simulation search"], "analytical": ["Analytical search", "Verification"],
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
                      "simulation iterations")
    elif args.mode == "analytical":
        margin = f"   margin [yellow]{args.margin} %[/]  ->  search cap [yellow]{args.cap - args.margin:g} %[/]" if args.margin else ""
        console.print(f"  cap          : [yellow]{args.cap} %[/]{margin}")
        console.print(f"  budget       : [cyan]{args.budget}[/] iterations x [cyan]{args.seeds}[/] seeds")
    else:
        console.print(f"  cap          : [yellow]{args.cap} %[/]")
        console.print(f"  budget       : [cyan]{args.budget}[/] iterations")
    start_name = f"random {args.rand_start}" if args.rand_start else "all exact"
    console.print(f"  start        : [cyan]{start_name}[/]   seed [cyan]{args.seed}[/]")
    console.print(f"  simulation   : [cyan]{args.samples:,}[/] samples (input seed {SEED_INPUTS})")
    console.print(f"  model        : {f'EEM ({EEM_ROOT})' if eem else source}")
    if eem and eem.fn.different:
        console.print(f"  [yellow]EEM implements these units differently from the simulation: {', '.join(eem.fn.different)}[/]")
    console.print("  area         : unit area library" + (f" + Design Compiler at the end ([cyan]{args.netlist}[/] "
                  f"netlist, {describe(synth_timing)}, "
                  f"{'set by --max-delay' if args.max_delay is not None else 'benchmark default'})"
                  if args.synth else " [dim](no synthesis)[/]"))
    console.print()

    stage("Setup")
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
    verified = start
    if args.mode in ("analytical", "hybrid"):
        stage("Analytical search")
        budget = args.analytical_budget if args.mode == "hybrid" else args.budget
        seeds = range(args.seed, args.seed + args.seeds)
        per_seed = start_for if args.rand_start else lambda seed: start
        jobs = min(args.seed_jobs, args.seeds)
        phases["analytical"] = multi_search(model, per_seed, search_cap, budget, seeds, dfg, lib, areas,
                                            jobs, (args.benchmark, args.depth, args.rand_start, search_cap))
        timing["analytical search"] = phases["analytical"]["seconds"]

        name = "Handoff check" if args.mode == "hybrid" else "Verification"
        stage(name)
        found = phases["analytical"]
        with Timer() as t_verify:
            chosen, front = verify(found["candidates"], sim, args.cap, args.verify_front)
            if chosen:
                verified = chosen["codes"]
            elif start != exact and sim(start) <= args.cap:
                console.print(f"  [yellow]no simulated candidate is under the cap ({args.cap} %), the start configuration is used[/]")
            else:
                verified = exact
                console.print(f"  [yellow]no simulated configuration is under the cap ({args.cap} %), the exact circuit is used[/]")
        found["front"] = [{k: c.get(k) for k in ("area", "saving_pct", "rms", "sim_rms", "seed")} for c in front]
        found["chosen"] = chosen and {k: chosen.get(k) for k in ("area", "saving_pct", "rms", "sim_rms", "seed")}
        timing[name.lower()] = t_verify.seconds
        console.print()

    base = verified if args.mode == "hybrid" else start

    if args.mode == "eem":
        stage("EEM search")
        phases["eem"] = search(eem, start, args.cap, args.budget, dfg, lib, areas, args.quiet)
        timing["eem search"] = phases["eem"]["seconds"]

    if args.mode in ("sim", "hybrid"):
        stage("Simulation search")
        phases["simulation"] = search(sim, base, args.cap, args.budget, dfg, lib, areas, args.quiet)
        timing["simulation search"] = phases["simulation"]["seconds"]

    stage("Final simulation")
    final_codes = verified if args.mode == "analytical" else list(phases.values())[-1]["codes"]
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
