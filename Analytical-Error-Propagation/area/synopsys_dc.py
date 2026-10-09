"""
Area and power of a configuration, synthesised with Synopsys Design Compiler.

    python3 -m area.synopsys_dc <benchmark> [--codes FILE] [--tag NAME] [--netlist original|generated]
                                [--max-delay NS] [--dry-run]

The units are substituted into area/synth_template/<benchmark>/*_original.v and synthesised
with the constraint of its script_sample.tcl, or, with --netlist generated, into a netlist built
from the benchmark JSON with the set_max_delay of synth.json. Each run writes its netlist,
script and reports to area/runs/<benchmark>_<tag>/.

Design Compiler runs from the Apptainer image in SYNOPSYS_DC_IMAGE or area/synopsys_dc.sif.
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

from area.netlist import build_netlist, rewrite_original
from area.tcl import synthesis_tcl
from model.data import load_benchmark, load_library
from sim.simulate import EXACT

HERE = Path(__file__).resolve().parent
TEMPLATES = HERE / "synth_template"
RUNS = HERE / "runs"
SIF_IMAGE = Path(os.environ.get("SYNOPSYS_DC_IMAGE", HERE / "synopsys_dc.sif"))
CONTAINER = shutil.which("apptainer") or shutil.which("singularity")
DC_JUNK = ("*.mr", "*-verilog.pvl", "*-verilog.syn", "command.log", "default.svf", "filenames_*.log")
NUM = r"[-+]?\d[\d,]*\.?\d*(?:[eE][-+]?\d+)?"
TO_MW = {"w": 1000.0, "mw": 1.0, "uw": 1e-3, "nw": 1e-6, "pw": 1e-9}


def dc_available():
    return CONTAINER is not None and SIF_IMAGE.exists()


def template(benchmark):
    folder = TEMPLATES / benchmark
    components, library = folder / "components.v", folder / "LIB" / "cmos22nm.db"
    for path in (components, library):
        if not path.exists():
            raise FileNotFoundError(f"missing {path}")
    return components, library


def default_max_delay(benchmark):
    # set_max_delay of a benchmark, from synth_template/<benchmark>/synth.json
    path = TEMPLATES / benchmark / "synth.json"
    if not path.exists():
        raise FileNotFoundError(f"no {path}: pass the delay explicitly or add the file")
    return float(json.loads(path.read_text())["max_delay"])


def original_netlist(benchmark):
    # the hand-written netlist of the benchmark, synth_template/<benchmark>/*_original.v
    found = sorted((TEMPLATES / benchmark).glob("*_original.v"))
    if len(found) != 1:
        raise FileNotFoundError(f"expected one *_original.v in {TEMPLATES / benchmark}, found {len(found)}")
    return found[0]


def template_constraint(benchmark):
    # the timing constraint of synth_template/<benchmark>/script_sample.tcl
    path = TEMPLATES / benchmark / "script_sample.tcl"
    if not path.exists():
        raise FileNotFoundError(f"missing {path}")
    text = path.read_text()
    clock = re.search(rf'^\s*create_clock\s+"?(\w+)"?.*?-period\s+({NUM})', text, re.MULTILINE)
    if clock:
        return {"clock": clock.group(1), "clock_period": float(clock.group(2))}
    delay = re.search(rf"^\s*set_max_delay\s+({NUM})", text, re.MULTILINE)
    if delay:
        return {"max_delay": float(delay.group(1))}
    raise ValueError(f"{path} has neither create_clock nor set_max_delay")


def constraint(benchmark, netlist="original", max_delay=None):
    # an explicit max_delay wins; otherwise the original netlist keeps its script_sample.tcl
    # constraint and the generated one takes synth.json
    if max_delay is not None:
        return {"max_delay": float(max_delay)}
    if netlist == "original":
        return template_constraint(benchmark)
    return {"max_delay": default_max_delay(benchmark)}


def describe(timing):
    if "clock" in timing:
        return f"clock {timing['clock']} period {timing['clock_period']:g} ns"
    return f"max_delay {timing['max_delay']:g} ns"


def run_dc(tcl, folder, report):
    # plain exec first, --fakeroot as a fallback; only a freshly written report counts
    before = report.stat().st_mtime if report.exists() else None
    log = ""
    for flags in ([], ["--fakeroot"]):
        proc = subprocess.run(
            [CONTAINER, "exec", *flags, str(SIF_IMAGE), "dc_shell", "-f", tcl],
            cwd=folder, capture_output=True, text=True, check=False,
        )
        for pattern in DC_JUNK:
            for junk in folder.glob(pattern):
                junk.unlink(missing_ok=True)
        log += proc.stdout + proc.stderr
        if report.exists() and report.stat().st_mtime != before:
            (folder / "dc.log").write_text(log)
            return True
    (folder / "dc.log").write_text(log)
    return False


def parse_area(report):
    # (total area, cell area)
    total = cell = None
    for line in report.read_text().splitlines():
        match = re.search(rf"({NUM})\s*$", line)
        if match is None:
            continue
        value = float(match.group(1).replace(",", ""))
        if "Total cell area" in line:
            cell = value
        elif "Total area" in line:
            total = value
    return total, cell


def parse_power(report):
    # (dynamic, leakage, total) in mW
    text = report.read_text()

    def find(pattern):
        match = re.search(pattern, text, re.IGNORECASE | re.MULTILINE)
        return float(match.group(1).replace(",", "")) * TO_MW[match.group(2).lower()] if match else None

    unit = r"([mupn]?W)"
    return (
        find(rf"Total Dynamic Power\s*=\s*({NUM})\s*{unit}"),
        find(rf"Cell Leakage Power\s*=\s*({NUM})\s*{unit}"),
        find(rf"^Total\s+(?:{NUM}\s*[mupn]?W\s+){{3}}({NUM})\s*{unit}"),
    )


def parse_slack(report):
    # worst slack in ns; negative means the max_delay constraint is violated
    match = re.search(rf"^\s*slack \(.*?\)\s+({NUM})", report.read_text(), re.MULTILINE)
    return float(match.group(1)) if match else None


def synthesise(benchmark, codes, tag="final", max_delay=None, netlist="original", dry_run=False):
    timing = constraint(benchmark, netlist, max_delay)
    dfg = load_benchmark(benchmark)
    lib = load_library()
    components, library = template(benchmark)
    declared = set(re.findall(r"^\s*module\s+(\w+)", components.read_text(), re.MULTILINE))

    if netlist == "original":
        design, top = rewrite_original(original_netlist(benchmark).read_text(), dfg, codes, lib)
    elif netlist == "generated":
        design, top = build_netlist(dfg, codes, lib, top=benchmark), benchmark
    else:
        raise ValueError(f"unknown netlist {netlist!r} (original or generated)")
    body = re.sub(r"//.*", "", design)
    used = set(re.findall(r"^\s*(\w+)\s+\w+\s*\(", body, re.MULTILINE))
    used -= {"module", "assign", "output", "input", "wire", "reg"}
    missing = sorted(used - declared)
    if missing:
        raise ValueError(f"{components} does not declare: {', '.join(missing)}")

    # the container only sees the run folder, so the components and the library are copied in
    folder = RUNS / f"{benchmark}_{tag}"
    if any(c.isspace() for c in str(folder)):
        # Design Compiler silently drops the internal power when the path has a space
        raise ValueError(f"run folder path must not contain spaces: {folder}")
    (folder / "LIB").mkdir(parents=True, exist_ok=True)
    shutil.copy(components, folder / "components.v")
    shutil.copy(library, folder / "LIB" / library.name)
    (folder / "design.v").write_text(design)
    (folder / "codes.json").write_text(json.dumps(codes, indent=1))
    (folder / "script.tcl").write_text(
        synthesis_tcl(top, ["components.v", "design.v"], f"./LIB/{library.name}", prefix=benchmark, **timing)
    )
    result = {"folder": str(folder), "netlist": netlist, "constraint": timing, "ran": False,
              "total_area": None, "cell_area": None, "slack": None,
              "dynamic_mw": None, "leakage_mw": None, "total_mw": None, "seconds": None}
    if dry_run or not dc_available():
        return result

    start = time.perf_counter()
    area_report = folder / f"{benchmark}_area_report.txt"
    fresh = run_dc("script.tcl", folder, area_report)
    result["seconds"] = time.perf_counter() - start
    if fresh:
        result["total_area"], result["cell_area"] = parse_area(area_report)
    result["ran"] = result["total_area"] is not None
    if result["ran"]:
        timing_report = folder / f"{benchmark}_timing_report.txt"
        if timing_report.exists():
            result["slack"] = parse_slack(timing_report)
        power_report = folder / f"{benchmark}_power_report.txt"
        if power_report.exists():
            result["dynamic_mw"], result["leakage_mw"], result["total_mw"] = parse_power(power_report)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("benchmark")
    parser.add_argument("--codes", type=Path, help='JSON with a codes dict or {"codes": {...}}; default all exact')
    parser.add_argument("--tag", default="final", help="name of the run folder (default final)")
    parser.add_argument("--netlist", choices=("original", "generated"), default="original",
                        help="original: units substituted into *_original.v (default); generated: built from the DFG")
    parser.add_argument("--max-delay", type=float,
                        help="set_max_delay in ns (default: script_sample.tcl for original, synth.json for generated)")
    parser.add_argument("--dry-run", action="store_true", help="write the netlist and the script only")
    args = parser.parse_args()

    dfg = load_benchmark(args.benchmark)
    codes = {node.id: EXACT[node.op] for node in dfg.nodes}
    if args.codes:
        data = json.loads(args.codes.read_text())
        given = data.get("codes", data)
        unknown = set(given) - set(codes)
        if unknown:
            sys.exit(f"unknown nodes in {args.codes}: {', '.join(sorted(unknown))}")
        codes.update({nid: int(code) for nid, code in given.items()})

    if not args.dry_run and not dc_available():
        print(f"Design Compiler not available (image {SIF_IMAGE}), writing the netlist and the script only")
    result = synthesise(args.benchmark, codes, args.tag, args.max_delay, args.netlist, args.dry_run)
    print(f"run folder: {result['folder']}   {args.netlist} netlist, {describe(result['constraint'])}")
    if result["ran"]:
        print(f"total area {result['total_area']}, cell area {result['cell_area']}, "
              f"power {result['total_mw']} mW, slack {result['slack']} ns, {result['seconds']:.1f} s")
    elif not args.dry_run and dc_available():
        print("Design Compiler did not write an area report, see dc.log in the run folder")


if __name__ == "__main__":
    main()
