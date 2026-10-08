import csv
import sys
import time

from rich import box
from rich.console import Console
from rich.table import Table

from model.data import DATA, load_benchmark, load_deep_table, load_library, read_json
from model.propagate import compute_wire_max, propagate_circuit_scalar, scalar_rms
from sim.simulate import EXACT

REPEATS = 200
USAGE = "usage: python3 -m validation.run [--depth 1|2] [--worst N] [--max-rms P] [--save] [ground-truth name ...]"
console = Console()


def analytical(dfg, codes, tables):
    propagate_circuit_scalar(dfg, codes, *tables)
    start = time.perf_counter()
    for _ in range(REPEATS):
        out = propagate_circuit_scalar(dfg, codes, *tables)[dfg.output]
    return out, (time.perf_counter() - start) / REPEATS


def inaccuracy(estimate, truth):
    return abs(estimate - truth) / abs(truth) * 100 if truth else 0.0


def shade(pct):
    colour = "green" if pct <= 2 else "yellow" if pct <= 5 else "red"
    return f"[{colour}]{pct:.2f} %[/]"


def duration(seconds):
    return f"{seconds * 1e6:.1f} µs" if seconds < 1e-3 else f"{seconds * 1e3:.1f} ms"


def short(name):
    return name.split("_", 1)[1]


def compare(name, lib, range_libs, depth, worst=None, save=False, max_rms=None):
    truth = read_json(DATA / "ground_truth" / f"{name}.json")
    if truth is None:
        sys.exit(f"no ground truth named {name!r} in data/ground_truth/\n{USAGE}")
    total = len(truth["configs"])
    if max_rms is not None:
        truth["configs"] = [c for c in truth["configs"] if c["rms_pct"] <= max_rms]
    benchmark = truth["benchmark"]
    dfg = load_benchmark(benchmark)
    node_lib = read_json(DATA / "node_metrics" / f"{benchmark}.json")
    feeder_lib = read_json(DATA / "node_metrics" / f"{benchmark}_feeder.json")
    deep_lib = load_deep_table(benchmark) if depth == 2 else None
    wire_max = compute_wire_max(dfg)
    m_out = wire_max[dfg.output]
    tables = (lib, *range_libs, wire_max, node_lib, feeder_lib, deep_lib)

    adds = sum(node.op == "add" for node in dfg.nodes)
    source = "node + feeder tables" if feeder_lib else "node tables" if node_lib else "range libraries"
    if deep_lib:
        source = "node + depth-2 feeder tables"
    elif depth == 2:
        source += f" [yellow](depth 1: no {benchmark}_d2 table, run python3 -m characterization.characterize_deep {benchmark})[/]"
    console.rule(f"[bold]{name}" + (f"[/] ({benchmark})" if name != benchmark else ""))
    console.print(
        f"{len(dfg.nodes)} nodes ({adds} add, {len(dfg.nodes) - adds} mul)   "
        f"M_out {m_out:,.0f}   ground truth {truth['samples']:,} samples   own error from {source}",
        style="dim",
    )
    if max_rms is not None:
        console.print(f"{len(truth['configs'])} of {total} configurations with RMS truth ≤ {max_rms:g} %", style="dim")
    if not truth["configs"]:
        console.print()
        return []

    accuracy = Table(title="Accuracy", box=box.SIMPLE_HEAD, header_style="bold")
    for header in ("Config", "ME\ntruth", "ME\nanalytical", "RMS %\ntruth", "RMS %\nanalytical", "Inaccuracy"):
        accuracy.add_column(header, justify="left" if header == "Config" else "right", no_wrap=True)

    runs = Table(title="Configurations and run time", box=box.SIMPLE_HEAD, header_style="bold")
    for header in ("Config", "Analytical", "Simulation", "Speed-up"):
        runs.add_column(header, justify="left" if header == "Config" else "right", no_wrap=True)
    runs.add_column("Approximate units")

    rows, results = [], []
    for config in truth["configs"]:
        out, seconds = analytical(dfg, config["codes"], tables)
        rms = scalar_rms(out, m_out)
        error = inaccuracy(rms, config["rms_pct"])
        speedup = config["seconds"] / seconds
        rows.append((error, speedup))
        results.append((config, out, rms, error, seconds, speedup))
    if save:
        suffix = f"_rms{max_rms:g}" if max_rms is not None else ""
        path = DATA.parent / "results" / f"{name}_depth{depth}{suffix}.csv"
        path.parent.mkdir(exist_ok=True)
        with path.open("w", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(["config", "me_truth", "me_analytical", "rms_pct_truth", "rms_pct_analytical",
                             "inaccuracy_pct", "analytical_us", "simulation_ms", "speedup", "approximate_units"])
            for config, out, rms, error, seconds, speedup in results:
                units = " ".join(f"{node}={short(lib[code].name)}" for node, code in config["codes"].items()
                                 if code not in EXACT.values())
                writer.writerow([config["name"], f"{config['me']:.4f}", f"{out['me']:.4f}",
                                 f"{config['rms_pct']:.6f}", f"{rms:.6f}", f"{error:.4f}",
                                 f"{seconds * 1e6:.2f}", f"{config['seconds'] * 1e3:.3f}", f"{speedup:.0f}", units])
        console.print(f"saved {path.relative_to(DATA.parent)}", style="dim")
    if worst is not None:
        results = sorted(results, key=lambda r: -r[3])[:worst]
        accuracy.title = f"Accuracy, worst {len(results)} of {len(rows)}"
        runs.title = f"Configurations and run time, worst {len(results)} of {len(rows)}"

    for config, out, rms, error, seconds, speedup in results:
        codes = config["codes"]
        accuracy.add_row(
            config["name"],
            f"{config['me']:,.1f}",
            f"{out['me']:,.1f}",
            f"{config['rms_pct']:.4f}",
            f"{rms:.4f}",
            shade(error),
        )
        units = [f"{node} [cyan]{short(lib[code].name)}[/]" for node, code in codes.items()
                 if code not in EXACT.values()]
        runs.add_row(config["name"], duration(seconds), duration(config["seconds"]), f"{speedup:,.0f}×",
                     "  ".join(units))

    console.print(accuracy)
    console.print(runs)
    console.print()
    return rows


def main():
    args = sys.argv[1:]
    depth = 1
    if "--depth" in args:
        i = args.index("--depth")
        if i + 1 >= len(args) or args[i + 1] not in ("1", "2"):
            sys.exit(USAGE)
        depth = int(args[i + 1])
        del args[i:i + 2]
    save = "--save" in args
    if save:
        args.remove("--save")
    max_rms = None
    if "--max-rms" in args:
        i = args.index("--max-rms")
        try:
            max_rms = float(args[i + 1])
        except (IndexError, ValueError):
            sys.exit(USAGE)
        del args[i:i + 2]
    worst = None
    if "--worst" in args:
        i = args.index("--worst")
        if i + 1 >= len(args) or not args[i + 1].isdigit() or int(args[i + 1]) < 1:
            sys.exit(USAGE)
        worst = int(args[i + 1])
        del args[i:i + 2]
    names = args or sorted(p.stem for p in (DATA / "ground_truth").glob("*.json"))
    if not names:
        sys.exit("no ground truth found — run python3 -m validation.generate first")
    lib = load_library()
    range_libs = (read_json(DATA / "adder_range_metrics.json"), read_json(DATA / "mul_range_metrics.json"))

    scope = f", RMS truth ≤ {max_rms:g} %" if max_rms is not None else ""
    summary = Table(title=f"Summary (depth {depth}{scope})", box=box.SIMPLE_HEAD, header_style="bold")
    for column in ("Ground truth", "Configs", "Mean inaccuracy", "Median inaccuracy", "Max inaccuracy",
                   "Within 1 %", "Mean speed-up"):
        summary.add_column(column, justify="left" if column == "Ground truth" else "right")
    for name in names:
        rows = compare(name, lib, range_libs, depth, worst, save, max_rms)
        if not rows:
            summary.add_row(name, "0", "-", "-", "-", "-", "-")
            continue
        errors, speedups = zip(*rows)
        ordered = sorted(errors)
        median = (ordered[(len(ordered) - 1) // 2] + ordered[len(ordered) // 2]) / 2
        within = sum(e <= 1 for e in errors)
        summary.add_row(name, str(len(rows)), shade(sum(errors) / len(errors)), shade(median), shade(max(errors)),
                        f"{within} ({within / len(errors) * 100:.0f} %)", f"{sum(speedups) / len(speedups):,.0f}×")
    console.print(summary)
    console.print("Inaccuracy = |RMS analytical − RMS truth| / RMS truth  (eq 14)", style="dim")


if __name__ == "__main__":
    main()
