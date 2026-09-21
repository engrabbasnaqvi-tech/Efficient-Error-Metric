# Efficient Error Metric — analytic error-propagation model

Validates the Chen et al. (ISVLSI 2024) analytic error-propagation model against
DeepApprox simulation, for any benchmark and backend.

## Layout

```
core/                 the fixed model — NEVER edit per benchmark
  circuit.py          generic dataflow graph
  model.py            paper model (eq 12/13): ME, MSE, SNR
  component_lib.py    ctypes bridge to engine.so (found by path)
  engine/
    engine.cpp        dispatch shim
    components.cpp     EvoApproxLib units (quadrant-swap fix applied)
    components.h
    engine.so         prebuilt

benchmarks/
  fir/
    spec.py           the ONLY per-benchmark code: topology + line maps,
                      for BOTH backends (systemc tree, verilog cascade)
    data/sample.txt   input stream
    hdl/              annotated sources (.cpp, .v) + components.v
    configs/          saved DeepApprox configs to test
  fft/        (add later — same shape)
  rgb2gray/   (add later — same shape)

run.py                one runner for all benchmarks & backends
```

## Run

```
python3 run.py <benchmark> <backend> <config_file> [sample_file]

python3 run.py fir systemc benchmarks/fir/configs/sc_cfg2.txt
python3 run.py fir verilog benchmarks/fir/configs/v_test.txt
```

A config file holds the DeepApprox conf_path dict (the leading `conf_path:` from
a log is stripped automatically):

```
{63:'mul16u_exact-mul16u_8VH', 85:'adder_16bit-add16u_0M0'}
```

## Output

Four metrics, analytic (paper model) vs SIM (real units = what DeepApprox measures):
- **ME, MSE, SNR** — the three moment metrics the model produces exactly.
- **PSNR** — derived from MSE via DeepApprox's own formula; use for the accept/
  reject threshold (DeepApprox already supports `ErrorMetric=PSNR`).
- **MREP** — printed for SIM only. It is a per-sample ratio, not a moment, so the
  analytic model cannot produce it without joint (gold,error) PMF tracking.

`inacc%` on each analytic line is the estimation inaccuracy (paper eq 14).

## Adding a new benchmark (fft, rgb2gray, ...)

1. `mkdir -p benchmarks/<name>/{data,hdl,configs}`
2. Put `sample.txt` in `data/`, annotated sources in `hdl/`.
3. Write `benchmarks/<name>/spec.py` exposing:
   - `INPUTS` (list of input names)
   - `BACKENDS` dict: for each backend a `build` fn (returns a Circuit),
     a `line_map` (annotated line -> node), and `node_order`.
   - `build_circuit(backend)`, `line_map(backend)`, `node_order(backend)`.
4. Run: `python3 run.py <name> <backend> benchmarks/<name>/configs/<cfg>.txt`

The core/ model is never touched. Note: the current model core supports
feedforward add/mul DAGs only. FFT adds `negate` and heavy reconvergence — it
will run, but expect larger inaccuracy where the independence assumption strains,
and it may need the joint-PMF extension for tight accuracy. IIR/feedback is out
of scope (the topological sort rejects cycles).

## Rebuild engine.so (only if a unit's C source changes)

```
g++ -O2 -shared -fPIC core/engine/engine.cpp core/engine/components.cpp -o core/engine/engine.so
```

Sanity check the fixed build (8VH×32 must zero on mid-range input):
```
python3 -c "import sys; sys.path.insert(0,'core'); import numpy as np, component_lib as cl; \
  print(cl.call_unit('mul','mul16u_8VH',np.array([2000]),np.array([32])))"   # -> [0]
```
