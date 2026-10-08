//Top level module for mac, N=10
module mac_n10 (x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, result);
input [31:0] x0;
input [31:0] x1;
input [31:0] x2;
input [31:0] x3;
input [31:0] x4;
input [31:0] x5;
input [31:0] x6;
input [31:0] x7;
input [31:0] x8;
input [31:0] x9;
output [31:0] result;

parameter W_0 = 32'd12;
parameter W_1 = 32'd34;
parameter W_2 = 32'd25;
parameter W_3 = 32'd31;
parameter W_4 = 32'd62;
parameter W_5 = 32'd42;
parameter W_6 = 32'd6;
parameter W_7 = 32'd12;
parameter W_8 = 32'd3;
parameter W_9 = 32'd29;

wire [31:0] prod [0:9];
wire [31:0] sum [0:8];

adder_16bit component_01(prod[0], prod[1], sum[0]);
adder_16bit component_02(prod[2], prod[3], sum[1]);
adder_16bit component_03(prod[4], prod[5], sum[2]);
adder_16bit component_04(prod[6], prod[7], sum[3]);
adder_16bit component_05(prod[8], prod[9], sum[4]);
adder_16bit component_06(sum[0], sum[1], sum[5]);
adder_16bit component_07(sum[2], sum[3], sum[6]);
adder_16bit component_08(sum[5], sum[6], sum[7]);
adder_16bit component_09(sum[7], sum[4], sum[8]);

mul16u_exact component_10(x0, W_0, prod[0]);
mul16u_exact component_11(x1, W_1, prod[1]);
mul16u_exact component_12(x2, W_2, prod[2]);
mul16u_exact component_13(x3, W_3, prod[3]);
mul16u_exact component_14(x4, W_4, prod[4]);
mul16u_exact component_15(x5, W_5, prod[5]);
mul16u_exact component_16(x6, W_6, prod[6]);
mul16u_exact component_17(x7, W_7, prod[7]);
mul16u_exact component_18(x8, W_8, prod[8]);
mul16u_exact component_19(x9, W_9, prod[9]);

assign result = sum[8];

endmodule
