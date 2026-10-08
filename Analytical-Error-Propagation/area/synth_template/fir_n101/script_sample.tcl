# A clean start is always good.
# do not use -all, that one will delete all std cell libraries
remove_design -design
set target_library \ {./LIB/cmos22nm.db}
set link_library \ {./LIB/cmos22nm.db}
analyze -f verilog components.v
analyze -f verilog test.v
elaborate fir_n101

# set delay constraints
set_max_delay 12 -to [all_outputs]
# now let's just compile and see what comes up
compile -area_effort low
# get some useful report about area and timing
# report_power
report_area

report_area > test_area_report.txt
report_power  > test_power_report.txt
report_timing > test_timing_report.txt

exit
