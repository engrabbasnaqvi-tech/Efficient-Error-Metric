# A clean start is always good. 
# do not use -all, that one will delete all std cell libraries
remove_design -design 
set target_library \ {./LIB/cmos22nm.db}
set link_library \ {./LIB/cmos22nm.db}
analyze -f verilog components.v
analyze -f verilog test.v
elaborate fir_approx

# set delay constraints
create_clock "clk" -name "global_clk" -period 0.2
# now let's just compile and see what comes up 
compile -area_effort low
# get some useful report about area and timing
report_power
report_area
report_timing

report_area > area_report_test.txt
report_power  > power_report_test.txt
report_timing  > timing_report_test.txt

exit

