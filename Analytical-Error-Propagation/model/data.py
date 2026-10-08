import json
from pathlib import Path

import numpy as np

from .models import BenchmarkDFG, ComponentVariant

DATA = Path(__file__).resolve().parents[1] / "data"


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
    folder = DATA / "node_metrics" / f"{benchmark}_d2"
    meta = read_json(folder / "meta.json")
    if meta is None:
        return None
    tables = {}
    for node_id in meta["nodes"]:
        if not np.load(folder / f"{node_id}_done.npy").all():
            raise ValueError(f"{folder.name}/{node_id}: table incomplete, finish characterization/characterize_deep.py first")
        tables[node_id] = np.load(folder / f"{node_id}.npy", mmap_mode="r")
    return meta, tables
