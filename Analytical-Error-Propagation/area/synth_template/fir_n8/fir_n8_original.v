//Top level module for fir_n8, N=8
module fir_n8 (clk, Reset, sample, result);
input clk, Reset;
input [31:0] sample;
output [31:0] result;

wire [31:0] tap [0:7];
wire [31:0] ir [0:7];
wire [31:0] sum [0:6];

assign tap[0] = sample;

// Tap delay line (shift register)
register fir_reg_0(Reset, clk, tap[0], tap[1]);
register fir_reg_1(Reset, clk, tap[1], tap[2]);
register fir_reg_2(Reset, clk, tap[2], tap[3]);
register fir_reg_3(Reset, clk, tap[3], tap[4]);
register fir_reg_4(Reset, clk, tap[4], tap[5]);
register fir_reg_5(Reset, clk, tap[5], tap[6]);
register fir_reg_6(Reset, clk, tap[6], tap[7]);

adder_16bit component_01(ir[0], ir[1], sum[0]);
adder_16bit component_02(sum[0], ir[2], sum[1]);
adder_16bit component_03(sum[1], ir[3], sum[2]);
adder_16bit component_04(sum[2], ir[4], sum[3]);
adder_16bit component_05(sum[3], ir[5], sum[4]);
adder_16bit component_06(sum[4], ir[6], sum[5]);
adder_16bit component_07(sum[5], ir[7], sum[6]);

mul16u_exact component_08(tap[0], 32'd78, ir[0]);
mul16u_exact component_09(tap[1], 32'd424, ir[1]);
mul16u_exact component_10(tap[2], 32'd1305, ir[2]);
mul16u_exact component_11(tap[3], 32'd2290, ir[3]);
mul16u_exact component_12(tap[4], 32'd2290, ir[4]);
mul16u_exact component_13(tap[5], 32'd1305, ir[5]);
mul16u_exact component_14(tap[6], 32'd424, ir[6]);
mul16u_exact component_15(tap[7], 32'd78, ir[7]);

assign result = sum[6];

endmodule
