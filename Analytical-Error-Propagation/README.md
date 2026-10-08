# Analytical Error Propagation

Estimates the output error of a circuit built from approximate EvoApproxLib
adders and multipliers without simulating it, and checks the estimate against a
simulation of the real hardware.

The propagation uses equations 12 and 13 of Chen et al. (ISVLSI 2024). What is
specific to this model is how each node's own error is obtained, described below.

## Quick start

Requires Python 3 with `numpy`, `pydantic` and `rich`, plus `gcc` and `g++`.

```bash
sh sim/build.sh                                # build the simulation binaries (once)
python3 -m characterization.characterize fir   # build the error tables (once per benchmark)
python3 -m validation.generate fir 5           # pick 5 random configurations and simulate them
python3 -m validation.run                      # compare the model with the simulation
```

`validation.generate <benchmark> [configs] [samples]` draws random configurations
until each error band between 0.5 % and 10 % RMS holds one, simulates them on
200,000 samples and stores the result in `data/ground_truth/`.
`validation.run [name ...]` evaluates the model on those configurations and prints
the comparison.

Fixed configurations can be simulated instead of random ones:

```bash
python3 -m validation.generate --fixed data/configs/fir_deepapprox.json
python3 -m validation.run fir_deepapprox
```

With `--depth 2`, `validation.run` takes each adder's own error from the depth-2
table built by `python3 -m characterization.characterize_deep <benchmark>`
(`data/node_metrics/<benchmark>_d2/`, not tracked; about 10 hours on 12 cores for
`fir`). The table also records the units one level further upstream of each
feeder. Benchmarks without the table fall back to depth 1, which is the default.

```bash
python3 -m validation.run --depth 1   # node + feeder tables (default)
python3 -m validation.run --depth 2   # node + depth-2 feeder tables
```

`data/configs/fir_deepapprox.json` holds the eight DeepApprox configurations
used in the EEM experiments (`sc_best`, `sc_cfg2` to `sc_cfg8`), mapped onto
the nodes of `fir`. `mul16u_BMC` in `sc_cfg8` is an exact multiplier
(MAE 0, WCE 0) and is treated as exact.

## Design space exploration

`pipeline.py` searches for the smallest-area configuration under an RMS error cap
with Monte Carlo tree search (`dse/mcts.py`), using simulation, the analytical
model, or both:

```bash
python3 pipeline.py fir --mode hybrid --cap 5 --synth --quiet
```

- `sim`: every search step is simulated (1,000,000 samples by default).
- `analytical`: every search step uses the model.
- `hybrid`: the model searches first under `cap - margin`, the result is
  simulated once, and the simulation continues the search from it.

Area during the search comes from the unit area library
`dse/lib/area_library.json` (rebuilt with `python3 -m area.build_area_library`).
With `--synth`, the all-exact and the final configuration are synthesised with
Synopsys Design Compiler (`area/`), by default by substituting the units into the
benchmark's `*_original.v` in `area/synth_template/`; the real area and power
reductions are reported. The final configuration is always simulated once. Each
run writes a JSON result and its terminal log to `results/dse/`.
`python3 pipeline.py --help` lists all options.

## Results

| Benchmark | Configurations | Mean inaccuracy | Max inaccuracy | Speed-up over simulation |
|---|---|---|---|---|
| `fir` (9 multipliers, 8 adders) | 5 | 0.37 % | 1.14 % | ~1,200× |
| `ter_sum_nine` (8 adders) | 3 | 0.23 % | 0.36 % | ~5,000× |
| `fir`, DeepApprox configurations | 8 | 1.68 % | 11.08 % | ~4,000× |

Inaccuracy is `|RMS model − RMS simulation| / RMS simulation` (eq. 14 of the
paper). One evaluation takes 10–30 µs; the simulation takes 10–50 ms. On a
wider test with 60 extra random configurations per benchmark, the error is
typically 0.1–0.3 %, and 60 of 65 (`fir`) and 58 of 63 (`ter_sum_nine`)
configurations are within 1 %. Seven of the eight DeepApprox configurations
are within about 1 %; `sc_cfg4` is 11 % off, a case of the first known
limitation below (it drops to 0.02 % when three levels of history are used).

## How it works

Every wire carries four numbers: the mean and mean square of the exact value,
`E[x]` and `E[x²]`, and of the error, `E[Δ]` and `E[Δ²]`. They are pushed from
the inputs to the output with eq. 12 for adders and eq. 13 for multipliers. Each
node adds its own error, taken from the tables built by `characterization.characterize`:

- **Node table.** The own error of every unit, measured on the operands that
  actually reach that node in the circuit.
- **Feeder table (adders).** The own error of every unit, measured behind every
  pair of units that can feed the node, together with how it varies with the
  incoming error.

The feeder table is needed because of how the approximate adders are built.
Each one adds exactly above some bit `k`, guesses the carry into bit `k`, and
fills the bits below `k` with copies or simple functions of other input bits,
mostly high ones. Its own error therefore depends on the exact bit pattern of
its operands. Behind another approximate unit, that pattern is shaped by the
unit in front, so the adder's own error changes and is no longer independent of
the incoming error. Eq. 12 assumes it is independent; the feeder table supplies
the measured values instead.

## Approaches tried

**1. EPMF.** Each wire carried a compressed error distribution (up
to 64 weighted intervals), and node errors came from a table indexed by input
magnitude. It supported adders only, and was 8–46 % off the simulation, mostly
because each node used a single worst-case table entry. Replaced by the scalar
model.

**2. Mean and mean-square propagation with generic libraries.** The four
numbers per wire described above, with each unit's own error looked up by input
range in libraries characterised on two uniform operands. It added multipliers,
was about 1,000× faster than EPMF, and typically 3–8 % off. It failed where the
operands are not uniform: a FIR multiplier has one constant coefficient (up to
350 % off), and chains of approximate adders were 24–55 % off.

**3. Per-node tables.** Each node's own error measured on its real operands.
This fixed the multipliers (within 0.1 %) but not the adder chains, which were
still 13–36 % off.

**4. Feeder table (current).** Adds the feeder table described above. Chains
of identical adders are within 0.3 %, and random configurations are typically
within 0.1–0.3 %.

## Known limitations

- The feeder table only knows the units that directly feed a node. Further
  upstream it assumes the same unit as the feeder. When a path mixes different
  units, the error can reach a few percent (up to about 7 % on `fir` in the
  random test). The depth-2 table (`--depth 2`) adds one more level of
  history: on 63 random `fir` configurations between 0.5 and 10 % RMS the mean
  error drops from 0.23 % to 0.11 % and the maximum from 3.7 % to 0.31 %. It holds
  27 million entries (827 MB) and is built only for `fir`. `sc_cfg4` needs three
  levels and stays 11 % off.
- The units read only the low 16 bits of each operand. Wires that exceed 16 bits
  wrap in hardware, which the model does not represent. All benchmarks stay
  within 16 bits in the worst case, except the last adder of `ter_sum_nine`
  (worst case 73,719).

## Layout

```
pipeline.py         design space exploration (sim, analytical or hybrid)
dse/                MCTS search, unit areas and the unit area library
area/               Design Compiler synthesis; synth_template/ holds the Verilog per benchmark
characterization/   node and feeder tables, and the depth-2 table (hours of run time)
validation/         ground truth (generate) and model vs simulation (run)
model/propagate.py  the propagation (eq. 12 and 13)
model/models.py     circuit and component classes
model/data.py       loaders
sim/                simulation binaries: sources in sim/src, built into sim/bin
data/benchmarks/    circuits, grouped by family
data/node_metrics/  node and feeder tables (<benchmark>_d2/ not tracked)
data/configs/       fixed configurations for validation.generate --fixed
data/ground_truth/  configurations and simulation results
results/            accuracy results; results/dse/ holds DSE runs
```

## Unit codes

```
adders        0 exact, 2 0AV, 3 0EM, 4 0Q7, 5 073, 6 0M0, 7 0DL, 8 0GK, 9 02E, 10 0MH
multipliers  11 exact, 13 AQ1, 14 5FA, 15 DAE, 16 F6B, 17 CK3, 18 8VH, 19 GPF, 20 HGP, 21 HGY
```

The simulation binaries come from the Partioning repository; the multiplier
sources are EvoApproxLib v1.0.
