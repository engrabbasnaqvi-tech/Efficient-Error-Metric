#!/usr/bin/env bash
# same benchmark, cap, start, samples, depth and synthesis for every run; runs are sequential so timings are comparable
cd "$(dirname "$0")/../../.."
OUT=results/dse/final_check
COMMON="fir --cap 5 --start root --samples 1000000 --depth 2 --synth --quiet"
for seed in 0 1 2; do
  python3 pipeline.py $COMMON --seed $seed --mode analytical --budget 1000 --out $OUT/analytical_s$seed.json
  python3 pipeline.py $COMMON --seed $seed --mode sim        --budget 1000 --out $OUT/sim_s$seed.json
  python3 pipeline.py $COMMON --seed $seed --mode hybrid     --analytical-budget 1000 --budget 1000 --out $OUT/hybrid_s$seed.json
done
echo ALL DONE
