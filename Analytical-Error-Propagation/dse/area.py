import json
from pathlib import Path

from sim.simulate import EXACT, UNITS

AREA_LIBRARY = Path(__file__).resolve().parent / "lib" / "area_library.json"


def load_areas(lib):
    # {code: area} for every unit code in the library
    components = json.loads(AREA_LIBRARY.read_text())["components"]
    exact = {e["op"]: e["area"] for e in components.values() if e["exact"]}
    return {
        code: exact[cv.op] if code == EXACT[cv.op] else components[cv.name]["area"]
        for code, cv in lib.items()
        if code == EXACT[cv.op] or cv.name in components
    }


def variants(dfg, lib, areas):
    # {node_id: [(code, name), ...]}: every approximate unit smaller than the exact one, plus exact
    out = {}
    for node in dfg.nodes:
        exact = EXACT[node.op]
        moves = [(code, lib[code].name) for code in UNITS[node.op] if areas[code] < areas[exact]]
        out[node.id] = moves + [(exact, "exact")]
    return out


def circuit_area(codes, areas):
    return sum(areas[code] for code in codes.values())
