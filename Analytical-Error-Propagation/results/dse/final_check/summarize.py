import json, statistics
from pathlib import Path

HERE = Path(__file__).resolve().parent
rows = []
for mode in ("analytical", "sim", "hybrid"):
    for seed in (0, 1, 2):
        path = HERE / f"{mode}_s{seed}.json"
        if not path.exists():
            continue
        d = json.loads(path.read_text()); f = d["final"]; s = d["synthesis"]; t = d["timing"]; ev = d["evaluations"]
        rows.append(dict(
            mode=mode, seed=seed, rms=f["sim_rms"], lib=f["saving_pct"],
            area=s["final"]["total_area"], exact=s["exact"]["total_area"], ared=s["area_reduction_pct"], pred=s["power_reduction_pct"],
            slack=s["final"]["slack"], search=t.get("analytical search", 0) + t.get("handoff check", 0) + t.get("simulation search", 0),
            synth=t["synthesis"], total=t["total"], sims=ev["simulation"]["calls"], models=ev["analytical"]["calls"]))

print(f"{'mode':<11}{'seed':>4} {'sim RMS':>8} {'DC area':>8} {'area red.':>9} {'power red.':>10} {'lib sav.':>8} "
      f"{'search':>8} {'synth':>7} {'total':>8} {'sims':>5} {'model':>6} {'slack':>6}")
for r in rows:
    print(f"{r['mode']:<11}{r['seed']:>4} {r['rms']:7.3f}% {r['area']:8.1f} {r['ared']:8.1f}% {r['pred']:9.1f}% {r['lib']:7.1f}% "
          f"{r['search']:7.1f}s {r['synth']:6.1f}s {r['total']:7.1f}s {r['sims']:5} {r['models']:6} {r['slack']:6.2f}")
print()
for mode in ("analytical", "sim", "hybrid"):
    m = [r for r in rows if r["mode"] == mode]
    if not m:
        continue
    best = max(m, key=lambda r: r["ared"])
    print(f"{mode:<11} area reduction mean {statistics.mean(r['ared'] for r in m):5.1f} %  best {best['ared']:5.1f} % (seed {best['seed']}, "
          f"{best['rms']:.3f} %)   search time mean {statistics.mean(r['search'] for r in m):7.1f} s   total mean {statistics.mean(r['total'] for r in m):7.1f} s")
