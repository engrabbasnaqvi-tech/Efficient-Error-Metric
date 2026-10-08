//Top level module for ternary sum nine
module ter_sum_nine_16(a,b,c,d,e,f,g,h,i,out);
parameter SIZE = 32;
input[SIZE-1:0] a;
input[SIZE-1:0] b;
input[SIZE-1:0] c;
input[SIZE-1:0] d;
input[SIZE-1:0] e;
input[SIZE-1:0] f;
input[SIZE-1:0] g;
input[SIZE-1:0] h;
input[SIZE-1:0] i;
output [SIZE-1:0] out;
wire [SIZE-1:0] sum_0,sum_1,sum_2,sum_3,sum_4,sum_5,sum_6,sum_7,sum_8; 	
// Behavioral description of operation	
//Intermediate results for ternary nodes
adder_16bit component_01(a,b,sum_0);
adder_16bit component_02(c,d,sum_1);
adder_16bit component_03(e,f,sum_2);
adder_16bit component_04(g,h,sum_3);
adder_16bit component_05(sum_0, sum_1,sum_4);
adder_16bit component_06(sum_2, sum_3,sum_5);
adder_16bit component_07(sum_4, sum_5,sum_6);
adder_16bit component_08(sum_6,i,sum_7);

// Final result
assign out = sum_7;
endmodule
