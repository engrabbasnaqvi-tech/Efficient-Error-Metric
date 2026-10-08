//Top level module for conv_kernel, K=3
module conv_kernel_k3 (px0, px1, px2, px3, px4, px5, px6, px7, px8, result);
input [31:0] px0;
input [31:0] px1;
input [31:0] px2;
input [31:0] px3;
input [31:0] px4;
input [31:0] px5;
input [31:0] px6;
input [31:0] px7;
input [31:0] px8;
output [31:0] result;

parameter C_0 = 32'd1;
parameter C_1 = 32'd2;
parameter C_2 = 32'd1;
parameter C_3 = 32'd2;
parameter C_4 = 32'd4;
parameter C_5 = 32'd2;
parameter C_6 = 32'd1;
parameter C_7 = 32'd2;
parameter C_8 = 32'd1;

wire [31:0] prod [0:8];
wire [31:0] sum [0:7];

adder_16bit component_01(prod[0], prod[1], sum[0]);
adder_16bit component_02(prod[2], prod[3], sum[1]);
adder_16bit component_03(prod[4], prod[5], sum[2]);
adder_16bit component_04(prod[6], prod[7], sum[3]);
adder_16bit component_05(sum[0], sum[1], sum[4]);
adder_16bit component_06(sum[2], sum[3], sum[5]);
adder_16bit component_07(sum[4], sum[5], sum[6]);
adder_16bit component_08(sum[6], prod[8], sum[7]);

mul16u_exact component_09(px0, C_0, prod[0]);
mul16u_exact component_10(px1, C_1, prod[1]);
mul16u_exact component_11(px2, C_2, prod[2]);
mul16u_exact component_12(px3, C_3, prod[3]);
mul16u_exact component_13(px4, C_4, prod[4]);
mul16u_exact component_14(px5, C_5, prod[5]);
mul16u_exact component_15(px6, C_6, prod[6]);
mul16u_exact component_16(px7, C_7, prod[7]);
mul16u_exact component_17(px8, C_8, prod[8]);

wire [31:0] shifted = sum[7] >> 4;
assign result = (shifted > 32'd255) ? 32'd255 : shifted;

endmodule
