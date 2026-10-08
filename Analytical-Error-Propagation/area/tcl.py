def synthesis_tcl(top, sources, library, prefix, max_delay=None, clock=None, clock_period=None, effort="low"):
    # dc_shell script that synthesises one top module and writes area, power and timing reports
    lines = [
        "remove_design -design",
        f"set target_library {{{library}}}",
        f"set link_library   {{{library}}}",
    ]
    lines += [f"analyze -f verilog {src}" for src in sources]
    lines += [f"elaborate {top}", ""]
    if clock:
        lines.append(f'create_clock "{clock}" -name "global_clk" -period {clock_period}')
    if max_delay is not None:
        lines.append(f"set_max_delay {max_delay} -to [all_outputs]")
    lines += [
        f"compile -area_effort {effort}",
        "",
        f"report_area   > {prefix}_area_report.txt",
        f"report_power  > {prefix}_power_report.txt",
        f"report_timing > {prefix}_timing_report.txt",
        "",
        "exit",
    ]
    return "\n".join(lines) + "\n"


def components_tcl(modules, sources, library, effort="low"):
    # dc_shell script that synthesises every module on its own, one area report each
    lines = [
        f"set target_library {{{library}}}",
        f"set link_library   {{{library}}}",
    ]
    lines += [f"analyze -f verilog {src}" for src in sources]
    for name in modules:
        lines += [
            "",
            "remove_design -design",
            f"elaborate {name}",
            f"compile -area_effort {effort}",
            f"report_area > {name}_area_report.txt",
        ]
    lines += ["", "exit"]
    return "\n".join(lines) + "\n"
