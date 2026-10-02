# Analytical Error Propagation

Estimates the output error of a circuit built from approximate EvoApproxLib
adders and multipliers without simulating it, and checks the estimate against a
simulation of the real hardware.

The propagation uses equations 12 and 13 of Chen et al. (ISVLSI 2024). What is
specific to this model is how each node's own error is obtained, described below.

## Quick start

Requires Python 3 with `numpy`, `pydantic` and `rich`, plus `gcc` and `g++`.

```bash
sh sim/build.sh                  # build the simulation binaries (once)
python3 characterize.py fir      # build the error tables (once per benchmark)
python3 generate.py fir 5        # pick 5 random configurations and simulate them
python3 run.py                   # compare the model with the simulation
```

`generate.py <benchmark> [configs] [samples]` draws random configurations until
each error band between 0.5 % and 10 % RMS holds one, simulates them on 200,000
samples and stores the result in `data/ground_truth/`. `run.py [name ...]`
evaluates the model on those configurations and prints the comparison.

Fixed configurations can be simulated instead of random ones:

```bash
python3 generate.py --fixed data/configs/fir_deepapprox.json
python3 run.py fir_deepapprox
```

`data/configs/fir_deepapprox.json` holds the eight DeepApprox configurations
used in the EEM experiments (`sc_best`, `sc_cfg2` to `sc_cfg8`), mapped onto
the nodes of `fir`. `mul16u_BMC` in `sc_cfg8` is an exact multiplier
(MAE 0, WCE 0) and is treated as exact.

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
node adds its own error, taken from the tables built by `characterize.py`:

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
  random test). Taking one more level of history into account brings the worst
  case down to about 1 %, but the table then grows from thousands to millions of
  entries, so it is not used.
- The units read only the low 16 bits of each operand. Wires that exceed 16 bits
  wrap in hardware, which the model does not represent. Both included
  benchmarks stay within 16 bits.

## Layout

```
run.py                model vs stored simulation results
generate.py           random configurations and their simulation results
characterize.py       node and feeder tables
characterize_deep.py  depth-2 feeder table (experimental, hours of run time)
model/propagate.py    the propagation (eq. 12 and 13)
model/models.py       circuit and component classes
model/data.py         loaders
sim/                  simulation binaries: sources in sim/src, built into sim/bin
data/benchmarks/      circuits
data/node_metrics/    node and feeder tables (<benchmark>_d2/ from characterize_deep.py, not tracked)
data/configs/         fixed configurations for generate.py --fixed
data/ground_truth/    configurations and simulation results
```

## Unit codes

```
adders        0 exact, 2 0AV, 3 0EM, 4 0Q7, 5 073, 6 0M0, 7 0DL, 8 0GK, 9 02E, 10 0MH
multipliers  11 exact, 13 AQ1, 14 5FA, 15 DAE, 16 F6B, 17 CK3, 18 8VH, 19 GPF, 20 HGP, 21 HGY
```

The simulation binaries come from the Partioning repository; the multiplier
sources are EvoApproxLib v1.0.
