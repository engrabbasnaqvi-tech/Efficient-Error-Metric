import json
from pathlib import Path

import numpy as np

from .models import BenchmarkDFG, ComponentVariant

DATA = Path(__file__).resolve().parents[1] / "data"
D1 = DATA / "node_metrics" / "d1"
D2 = DATA / "node_metrics" / "d2"


def read_json(path):
    return json.loads(path.read_text()) if path.exists() else None


def benchmark_path(name):
    # data/benchmarks/<name>.json, or data/benchmarks/<family>/<name>.json
    flat = DATA / "benchmarks" / f"{name}.json"
    if flat.exists():
        return flat
    found = sorted((DATA / "benchmarks").glob(f"*/{name}.json"))
    if len(found) != 1:
        raise FileNotFoundError(f"no benchmark {name!r} in {DATA / 'benchmarks'}" if not found
                                else f"benchmark {name!r} found more than once: {', '.join(map(str, found))}")
    return found[0]


def load_benchmark(name):
    return BenchmarkDFG.model_validate(read_json(benchmark_path(name)))


def load_library():
    return {
        int(code): ComponentVariant(name=e["name"], op=e["op"], me=e["me"], error=e["mse"])
        for code, e in read_json(DATA / "final_library.json").items()
    }


def load_deep_table(benchmark):
    folder = D2 / f"{benchmark}_d2"
    meta = read_json(folder / "meta.json")
    if meta is None:
        return None
    # nodes with identical input cones share the table of the first one ("table" in meta.json)
    loaded, tables = {}, {}
    for node_id, node in meta["nodes"].items():
        owner = node.get("table", node_id)
        if owner not in loaded:
            if not np.load(folder / f"{owner}_done.npy").all():
                raise ValueError(f"{folder.name}/{owner}: table incomplete, finish characterization/characterize_deep.py first")
            loaded[owner] = np.load(folder / f"{owner}.npy", mmap_mode="r")
        tables[node_id] = loaded[owner]
    return meta, tables
