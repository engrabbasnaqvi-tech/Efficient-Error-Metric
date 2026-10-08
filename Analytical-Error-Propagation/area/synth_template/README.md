# Synthesis templates

One folder per benchmark, named as in `data/benchmarks/`:

- `components.v`: Verilog of the exact and approximate units
- `LIB/cmos22nm.db`: technology library
- `<benchmark>_original.v`: the hand-written netlist (registers included). By default the chosen
  units are substituted into it by position, the n-th DFG adder (multiplier) into the n-th
  adder (multiplier) instance
- `script_sample.tcl`: its timing constraint (`create_clock` or `set_max_delay`) is used with the
  original netlist
- `synth.json`: `set_max_delay` in ns for `--netlist generated`, where the netlist is built from
  the benchmark JSON instead; chosen so the exact circuit meets timing at its minimum area
  (fir and ter_sum_nine: about 2 ns, so 2.5); only these two have one so far

`--max-delay` replaces the constraint with either netlist.
