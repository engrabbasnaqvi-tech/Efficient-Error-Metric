import json
from pathlib import Path

from .models import BenchmarkDFG, ComponentVariant

DATA = Path(__file__).resolve().parents[1] / "data"


def read_json(path):
    return json.loads(path.read_text()) if path.exists() else None


def load_benchmark(name):
    return BenchmarkDFG.model_validate(read_json(DATA / "benchmarks" / f"{name}.json"))


def load_library():
    return {
        int(code): ComponentVariant(name=e["name"], op=e["op"], me=e["me"], error=e["mse"])
        for code, e in read_json(DATA / "final_library.json").items()
    }
