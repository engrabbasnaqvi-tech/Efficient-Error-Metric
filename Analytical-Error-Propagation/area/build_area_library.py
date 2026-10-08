"""
Area of every unit of the library, each synthesised on its own with Design Compiler.

    python3 -m area.build_area_library [--template fir] [--out dse/lib/area_library.json]
"""
import argparse
import json
import shutil
from pathlib import Path

from area.netlist import module_name
from area.synopsys_dc import RUNS, SIF_IMAGE, dc_available, parse_area, run_dc, template
from area.tcl import components_tcl
from model.data import load_library
from sim.simulate import EXACT

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--template", default="fir", help="synth_template folder whose components.v holds every unit (default fir)")
    parser.add_argument("--out", type=Path, default=ROOT / "dse" / "lib" / "area_library.json")
    args = parser.parse_args()
    if not dc_available():
        raise SystemExit(f"Design Compiler not available (image {SIF_IMAGE})")

    lib = load_library()
    units = {module_name(code, cv.op, lib): (cv.op, code == EXACT[cv.op]) for code, cv in sorted(lib.items())}
    components, library = template(args.template)

    folder = RUNS / "area_library"
    (folder / "LIB").mkdir(parents=True, exist_ok=True)
    shutil.copy(components, folder / "components.v")
    shutil.copy(library, folder / "LIB" / library.name)
    (folder / "script.tcl").write_text(components_tcl(units, ["components.v"], f"./LIB/{library.name}"))

    for name in units:
        (folder / f"{name}_area_report.txt").unlink(missing_ok=True)
    print(f"synthesising {len(units)} units from {components}")
    last = folder / f"{list(units)[-1]}_area_report.txt"
    if not run_dc("script.tcl", folder, last):
        raise SystemExit(f"Design Compiler did not finish, see {folder / 'dc.log'}")

    out = {}
    for name, (op, exact) in units.items():
        total, _ = parse_area(folder / f"{name}_area_report.txt")
        if total is None:
            raise SystemExit(f"no total area for {name}, see {folder / 'dc.log'}")
        out[name] = {"op": op, "area": total, "exact": exact}
        print(f"  {name:<14} {total:10.3f}")

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps({"components": out}, indent=1) + "\n")
    print(f"written to {args.out}")


if __name__ == "__main__":
    main()
