//Top level module for fir_n4, N=4
module fir_n4 (clk, Reset, sample, result);
input clk, Reset;
input [31:0] sample;
output [31:0] result;

wire [31:0] tap [0:3];
wire [31:0] ir [0:3];
wire [31:0] sum [0:2];

assign tap[0] = sample;

// Tap delay line (shift register)
register fir_reg_0(Reset, clk, tap[0], tap[1]);
register fir_reg_1(Reset, clk, tap[1], tap[2]);
register fir_reg_2(Reset, clk, tap[2], tap[3]);

adder_16bit component_01(ir[0], ir[1], sum[0]);
adder_16bit component_02(sum[0], ir[2], sum[1]);
adder_16bit component_03(sum[1], ir[3], sum[2]);

mul16u_exact component_04(tap[0], 32'd98, ir[0]);
mul16u_exact component_05(tap[1], 32'd926, ir[1]);
mul16u_exact component_06(tap[2], 32'd926, ir[2]);
mul16u_exact component_07(tap[3], 32'd98, ir[3]);

assign result = sum[2];

endmodule
