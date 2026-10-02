import sys
import time

from rich import box
from rich.console import Console
from rich.table import Table

from model.data import DATA, load_benchmark, load_library, read_json
from model.propagate import compute_wire_max, propagate_circuit_scalar, scalar_rms
from sim.simulate import EXACT

REPEATS = 200
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


def compare(name, lib, range_libs):
    truth = read_json(DATA / "ground_truth" / f"{name}.json")
    benchmark = truth["benchmark"]
    dfg = load_benchmark(benchmark)
    node_lib = read_json(DATA / "node_metrics" / f"{benchmark}.json")
    feeder_lib = read_json(DATA / "node_metrics" / f"{benchmark}_feeder.json")
    wire_max = compute_wire_max(dfg)
    m_out = wire_max[dfg.output]
    tables = (lib, *range_libs, wire_max, node_lib, feeder_lib)

    adds = sum(node.op == "add" for node in dfg.nodes)
    source = "node + feeder tables" if feeder_lib else "node tables" if node_lib else "range libraries"
    console.rule(f"[bold]{name}" + (f"[/] ({benchmark})" if name != benchmark else ""))
    console.print(
        f"{len(dfg.nodes)} nodes ({adds} add, {len(dfg.nodes) - adds} mul)   "
        f"M_out {m_out:,.0f}   ground truth {truth['samples']:,} samples   own error from {source}",
        style="dim",
    )

    accuracy = Table(title="Accuracy", box=box.SIMPLE_HEAD, header_style="bold")
    for header in ("Config", "ME\ntruth", "ME\nanalytical", "RMS %\ntruth", "RMS %\nanalytical", "Inaccuracy"):
        accuracy.add_column(header, justify="left" if header == "Config" else "right", no_wrap=True)

    runs = Table(title="Configurations and run time", box=box.SIMPLE_HEAD, header_style="bold")
    for header in ("Config", "Analytical", "Simulation", "Speed-up"):
        runs.add_column(header, justify="left" if header == "Config" else "right", no_wrap=True)
    runs.add_column("Approximate units")

    rows = []
    for config in truth["configs"]:
        codes = config["codes"]
        out, seconds = analytical(dfg, codes, tables)
        rms = scalar_rms(out, m_out)
        error = inaccuracy(rms, config["rms_pct"])
        speedup = config["seconds"] / seconds
        rows.append((error, speedup))
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
    names = sys.argv[1:] or sorted(p.stem for p in (DATA / "ground_truth").glob("*.json"))
    if not names:
        sys.exit("no ground truth found — run generate.py first")
    lib = load_library()
    range_libs = (read_json(DATA / "adder_range_metrics.json"), read_json(DATA / "mul_range_metrics.json"))

    summary = Table(title="Summary", box=box.SIMPLE_HEAD, header_style="bold")
    for column in ("Ground truth", "Configs", "Mean inaccuracy", "Max inaccuracy", "Mean speed-up"):
        summary.add_column(column, justify="left" if column == "Ground truth" else "right")
    for name in names:
        rows = compare(name, lib, range_libs)
        errors, speedups = zip(*rows)
        summary.add_row(name, str(len(rows)), shade(sum(errors) / len(errors)), shade(max(errors)),
                        f"{sum(speedups) / len(speedups):,.0f}×")
    console.print(summary)
    console.print("Inaccuracy = |RMS analytical − RMS truth| / RMS truth  (eq 14)", style="dim")


if __name__ == "__main__":
    main()
