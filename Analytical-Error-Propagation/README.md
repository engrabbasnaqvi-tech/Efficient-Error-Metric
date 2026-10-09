# Analytical Error Propagation and DSE

A design space exploration (DSE) that searches for the configuration of
approximate EvoApproxLib units with the smallest area under an error cap. The
error of each configuration comes from simulation, my analytical model, a mix of
both, or your EEM model.

## Setup

Python 3 with `numpy`, `pydantic` and `rich`, plus `gcc`/`g++`. Run everything
from this folder.

```bash
sh sim/build.sh      # builds my unit binaries into sim/bin, once per machine
```

Not in git (ask me): the depth-2 tables `data/node_metrics/<benchmark>_d2/`, and
for `--synth` the Design Compiler image `area/synopsys_dc.sif` and the library
`area/synth_template/*/LIB/cmos22nm.db`. Your EEM code is expected at
`../EEM/EEM` (or set `EEM_PATH`).

## Run

```bash
python3 pipeline.py fir --mode analytical --quiet
python3 pipeline.py fir --mode sim --budget 500 --quiet --synth
python3 pipeline.py fir --mode hybrid --seeds 100 --analytical-budget 2000 --budget 500 --margin 1 --quiet
python3 pipeline.py fir --mode eem --budget 500 --quiet
```

| Mode | Error used in each search step |
|---|---|
| `sim` | simulation with my unit binaries (ground truth) |
| `analytical` | my analytical model |
| `hybrid` | my model searches with many seeds under `cap - margin`; the results are simulated smallest area first, and simulation continues from the first one under the cap |
| `eem` | your EEM model, called unchanged through `dse/eem.py` |

| Flag | Default | Meaning |
|---|---|---|
| `--mode` | | `sim`, `analytical`, `hybrid` or `eem` |
| `--cap` | 5 | error cap, RMS in % of the largest output |
| `--budget` | 500 | MCTS iterations (the simulation phase in `hybrid`) |
| `--analytical-budget` | 2000 | `hybrid`: iterations of the analytical phase |
| `--margin` | 1.0 | `hybrid`: the analytical phase searches under `cap - margin` |
| `--seeds` | 100 | `hybrid`: number of analytical searches (seeds); their results are the handoff candidates |
| `--handoff-sims` | 10 | `hybrid`: candidates simulated at most, smallest area first, until one is under the cap |
| `--start` | `root` | start configuration: `root` (all exact), `random`, `FILE` or `FILE:NAME` |
| `--seed` | 0 | seed of the search |
| `--samples` | 1000000 | simulation samples |
| `--depth` | 2 | table depth of my model (falls back to 1 without a depth-2 table) |
| `--synth` | off | synthesise the exact and the final configuration with Design Compiler |
| `--netlist` | `original` | with `--synth`: the benchmark's hand-written netlist, or `generated` from the JSON |
| `--max-delay` | | with `--synth`: timing constraint in ns |
| `--quiet` | off | progress bar instead of one line per iteration |
| `--out` | | result file (default `results/dse/<benchmark>_<mode>.json`) |

The final configuration is always simulated once, so the real error is shown
next to the model's estimate. With `--synth` the real area and power reductions
are reported. Each run writes a JSON result and its terminal log to
`results/dse/`.

## Where I am with hybrid

In the first version of hybrid, my model searched once, and its result was
simulated before the simulation took over. The problem: when my model
underestimates a bit (it said 4.97 % on `adder_tree_32`, the simulation said
5.18 %), that one result is over the cap, and hybrid had to throw it away and
start the simulation from scratch.

So now hybrid works like this:

1. My model runs the search many times, each time with a different seed
   (`--seeds`, 100 by default). My model always gives the same error for the
   same configuration, but the search itself depends a lot on the seed, so
   this gives many different good configurations in a few seconds.
2. These candidates are simulated one by one, the one with the most area
   saving first (at most `--handoff-sims`, 10 by default).
3. The first one that is really under the cap is handed to the simulation,
   which continues the search from there.

On `adder_tree_32` the best candidate was just over the cap, the second one was
under it with 77.8 % saving, so it took two simulations instead of starting
over.

## What is where

```
pipeline.py         the DSE
dse/mcts.py         the MCTS (from the Partitioning project), over-cap reward -2
dse/eem.py          converts a benchmark and configuration for your EEM model
dse/lib/            area of every unit, used during the search
area/               Design Compiler synthesis and the Verilog per benchmark
model/              my analytical model (eq. 12/13 with per-node error tables)
sim/                the simulation and my unit binaries
characterization/   builds the tables of my model
validation/         ground truth and model vs simulation
data/benchmarks/    fir, ter_sum_nine, adder_tree, conv_kernel, mac
```

## Tables of my model

My model looks up each node's own error in tables measured once per benchmark.
`fir` and `ter_sum_nine` have them; other benchmarks fall back to generic
libraries (a few % off) until their tables are built:

```bash
python3 -m characterization.characterize adder_tree_16        # depth 1, minutes
python3 -m characterization.characterize_deep adder_tree_16   # depth 2, hours
characterization/remote_d2.sh vm01 adder_tree_16              # both, on a VM in tmux
```

To compare my model with simulation:

```bash
python3 -m validation.generate fir 5
python3 -m validation.run --depth 2 fir
```

## Unit codes

```
adders        0 exact, 2 0AV, 3 0EM, 4 0Q7, 5 073, 6 0M0, 7 0DL, 8 0GK, 9 02E, 10 0MH
multipliers  11 exact, 13 AQ1, 14 5FA, 15 DAE, 16 F6B, 17 CK3, 18 8VH, 19 GPF, 20 HGP, 21 HGY
```
