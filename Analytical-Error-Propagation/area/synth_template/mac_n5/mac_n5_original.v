//Top level module for mac, N=5
module mac_n5 (x0, x1, x2, x3, x4, result);
input [31:0] x0;
input [31:0] x1;
input [31:0] x2;
input [31:0] x3;
input [31:0] x4;
output [31:0] result;

parameter W_0 = 32'd18;
parameter W_1 = 32'd53;
parameter W_2 = 32'd39;
parameter W_3 = 32'd49;
parameter W_4 = 32'd97;

wire [31:0] prod [0:4];
wire [31:0] sum [0:3];

adder_16bit component_01(prod[0], prod[1], sum[0]);
adder_16bit component_02(prod[2], prod[3], sum[1]);
adder_16bit component_03(sum[0], sum[1], sum[2]);
adder_16bit component_04(sum[2], prod[4], sum[3]);

mul16u_exact component_05(x0, W_0, prod[0]);
mul16u_exact component_06(x1, W_1, prod[1]);
mul16u_exact component_07(x2, W_2, prod[2]);
mul16u_exact component_08(x3, W_3, prod[3]);
mul16u_exact component_09(x4, W_4, prod[4]);

assign result = sum[3];

endmodule
