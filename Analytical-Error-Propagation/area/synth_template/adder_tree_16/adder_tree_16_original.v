//Top level module for adder tree, N=16
module adder_tree_16 (in0, in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, out);
input [15:0] in0;
input [15:0] in1;
input [15:0] in2;
input [15:0] in3;
input [15:0] in4;
input [15:0] in5;
input [15:0] in6;
input [15:0] in7;
input [15:0] in8;
input [15:0] in9;
input [15:0] in10;
input [15:0] in11;
input [15:0] in12;
input [15:0] in13;
input [15:0] in14;
input [15:0] in15;
output [31:0] out;

wire [31:0] lvl1_0, lvl1_1, lvl1_2, lvl1_3, lvl1_4, lvl1_5, lvl1_6, lvl1_7, lvl2_0, lvl2_1, lvl2_2, lvl2_3, lvl3_0, lvl3_1, lvl4_0;

adder_16bit component_01({16'b0,in0}, {16'b0,in1}, lvl1_0);
adder_16bit component_02({16'b0,in2}, {16'b0,in3}, lvl1_1);
adder_16bit component_03({16'b0,in4}, {16'b0,in5}, lvl1_2);
adder_16bit component_04({16'b0,in6}, {16'b0,in7}, lvl1_3);
adder_16bit component_05({16'b0,in8}, {16'b0,in9}, lvl1_4);
adder_16bit component_06({16'b0,in10}, {16'b0,in11}, lvl1_5);
adder_16bit component_07({16'b0,in12}, {16'b0,in13}, lvl1_6);
adder_16bit component_08({16'b0,in14}, {16'b0,in15}, lvl1_7);
adder_16bit component_09(lvl1_0, lvl1_1, lvl2_0);
adder_16bit component_10(lvl1_2, lvl1_3, lvl2_1);
adder_16bit component_11(lvl1_4, lvl1_5, lvl2_2);
adder_16bit component_12(lvl1_6, lvl1_7, lvl2_3);
adder_16bit component_13(lvl2_0, lvl2_1, lvl3_0);
adder_16bit component_14(lvl2_2, lvl2_3, lvl3_1);
adder_16bit component_15(lvl3_0, lvl3_1, lvl4_0);

assign out = lvl4_0;

endmodule
