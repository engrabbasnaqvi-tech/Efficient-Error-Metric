//Top level module for fir_n25, N=25
module fir_n25 (clk, Reset, sample, result);
input clk, Reset;
input [31:0] sample;
output [31:0] result;

wire [31:0] tap [0:24];
wire [31:0] ir [0:24];
wire [31:0] sum [0:23];

assign tap[0] = sample;

// Tap delay line (shift register)
register fir_reg_0(Reset, clk, tap[0], tap[1]);
register fir_reg_1(Reset, clk, tap[1], tap[2]);
register fir_reg_2(Reset, clk, tap[2], tap[3]);
register fir_reg_3(Reset, clk, tap[3], tap[4]);
register fir_reg_4(Reset, clk, tap[4], tap[5]);
register fir_reg_5(Reset, clk, tap[5], tap[6]);
register fir_reg_6(Reset, clk, tap[6], tap[7]);
register fir_reg_7(Reset, clk, tap[7], tap[8]);
register fir_reg_8(Reset, clk, tap[8], tap[9]);
register fir_reg_9(Reset, clk, tap[9], tap[10]);
register fir_reg_10(Reset, clk, tap[10], tap[11]);
register fir_reg_11(Reset, clk, tap[11], tap[12]);
register fir_reg_12(Reset, clk, tap[12], tap[13]);
register fir_reg_13(Reset, clk, tap[13], tap[14]);
register fir_reg_14(Reset, clk, tap[14], tap[15]);
register fir_reg_15(Reset, clk, tap[15], tap[16]);
register fir_reg_16(Reset, clk, tap[16], tap[17]);
register fir_reg_17(Reset, clk, tap[17], tap[18]);
register fir_reg_18(Reset, clk, tap[18], tap[19]);
register fir_reg_19(Reset, clk, tap[19], tap[20]);
register fir_reg_20(Reset, clk, tap[20], tap[21]);
register fir_reg_21(Reset, clk, tap[21], tap[22]);
register fir_reg_22(Reset, clk, tap[22], tap[23]);
register fir_reg_23(Reset, clk, tap[23], tap[24]);

adder_16bit component_01(ir[0], ir[1], sum[0]);
adder_16bit component_02(sum[0], ir[2], sum[1]);
adder_16bit component_03(sum[1], ir[3], sum[2]);
adder_16bit component_04(sum[2], ir[4], sum[3]);
adder_16bit component_05(sum[3], ir[5], sum[4]);
adder_16bit component_06(sum[4], ir[6], sum[5]);
adder_16bit component_07(sum[5], ir[7], sum[6]);
adder_16bit component_08(sum[6], ir[8], sum[7]);
adder_16bit component_09(sum[7], ir[9], sum[8]);
adder_16bit component_10(sum[8], ir[10], sum[9]);
adder_16bit component_11(sum[9], ir[11], sum[10]);
adder_16bit component_12(sum[10], ir[12], sum[11]);
adder_16bit component_13(sum[11], ir[13], sum[12]);
adder_16bit component_14(sum[12], ir[14], sum[13]);
adder_16bit component_15(sum[13], ir[15], sum[14]);
adder_16bit component_16(sum[14], ir[16], sum[15]);
adder_16bit component_17(sum[15], ir[17], sum[16]);
adder_16bit component_18(sum[16], ir[18], sum[17]);
adder_16bit component_19(sum[17], ir[19], sum[18]);
adder_16bit component_20(sum[18], ir[20], sum[19]);
adder_16bit component_21(sum[19], ir[21], sum[20]);
adder_16bit component_22(sum[20], ir[22], sum[21]);
adder_16bit component_23(sum[21], ir[23], sum[22]);
adder_16bit component_24(sum[22], ir[24], sum[23]);

mul16u_exact component_25(tap[0], 32'd6, ir[0]);
mul16u_exact component_26(tap[1], 32'd12, ir[1]);
mul16u_exact component_27(tap[2], 32'd22, ir[2]);
mul16u_exact component_28(tap[3], 32'd38, ir[3]);
mul16u_exact component_29(tap[4], 32'd62, ir[4]);
mul16u_exact component_30(tap[5], 32'd96, ir[5]);
mul16u_exact component_31(tap[6], 32'd139, ir[6]);
mul16u_exact component_32(tap[7], 32'd191, ir[7]);
mul16u_exact component_33(tap[8], 32'd248, ir[8]);
mul16u_exact component_34(tap[9], 32'd303, ir[9]);
mul16u_exact component_35(tap[10], 32'd350, ir[10]);
mul16u_exact component_36(tap[11], 32'd382, ir[11]);
mul16u_exact component_37(tap[12], 32'd393, ir[12]);
mul16u_exact component_38(tap[13], 32'd382, ir[13]);
mul16u_exact component_39(tap[14], 32'd350, ir[14]);
mul16u_exact component_40(tap[15], 32'd303, ir[15]);
mul16u_exact component_41(tap[16], 32'd248, ir[16]);
mul16u_exact component_42(tap[17], 32'd191, ir[17]);
mul16u_exact component_43(tap[18], 32'd139, ir[18]);
mul16u_exact component_44(tap[19], 32'd96, ir[19]);
mul16u_exact component_45(tap[20], 32'd62, ir[20]);
mul16u_exact component_46(tap[21], 32'd38, ir[21]);
mul16u_exact component_47(tap[22], 32'd22, ir[22]);
mul16u_exact component_48(tap[23], 32'd12, ir[23]);
mul16u_exact component_49(tap[24], 32'd6, ir[24]);

assign result = sum[23];

endmodule
