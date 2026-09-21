#include "systemc.h"
#include "components.h"
//Top level module for FIR filter
SC_MODULE(FIR) {
	sc_in_clk clk;
 	sc_in<uint64_t> sample;
 	sc_out<uint64_t> result;

 	//For sampling
 	// sc_out<uint64_t> s_0;
 	// sc_out<uint64_t> s_1;
 	// sc_out<uint64_t> s_2;
 	// sc_out<uint64_t> s_3;
 	// sc_out<uint64_t> s_4;
 	// sc_out<uint64_t> s_5;
 	// sc_out<uint64_t> s_6;
 	// sc_out<uint64_t> s_7;
 	// sc_out<uint64_t> int_0;
 	// sc_out<uint64_t> int_1;
 	// sc_out<uint64_t> int_2;
 	// sc_out<uint64_t> int_3;
 	// sc_out<uint64_t> int_4;
 	// sc_out<uint64_t> int_5;
 	// sc_out<uint64_t> int_6;
 	// sc_out<uint64_t> int_7;
 	// sc_out<uint64_t> int_8;

 	// Constructor for FIR
 	SC_CTOR(FIR) {
 	SC_METHOD(behavior);
 	sensitive << clk.neg();
 } 
 private:
 	sc_buffer<uint64_t> i_1;
 	sc_buffer<uint64_t> i_2;
 	sc_buffer<uint64_t> i_3;
 	sc_buffer<uint64_t> i_4;
 	sc_buffer<uint64_t> i_5;
 	sc_buffer<uint64_t> i_6;
 	sc_buffer<uint64_t> i_7;
 	sc_buffer<uint64_t> i_8;
 
// Behavioral description of operation
void behavior() {
	uint64_t c_0,c_1,c_2,c_3,c_4,c_5,c_6,c_7,c_8;
	uint64_t ir_0,ir_1,ir_2,ir_3,ir_4,ir_5,ir_6,ir_7,ir_8;
	uint64_t sum_0,sum_1,sum_2,sum_3,sum_4,sum_5,sum_6,sum_7,sum_8;
 	//uint64_t i_0 = sample.read();

 	// Coefficients of filter
 	c_0=7;
 	c_1=17;
 	c_2=32;
 	c_3=46;
 	c_4=52;
 	c_5=46;
 	c_6=32;
 	c_7=17;
 	c_8=7;


 	//Intermediate results for multiplications
 	ir_0 = <<<CANDIDATE_1>>>mul16u_exact(c_0,sample.read());
 	ir_1 = <<<CANDIDATE_1>>>mul16u_exact(c_1,i_1.read());
 	ir_2 = <<<CANDIDATE_1>>>mul16u_exact(c_2,i_2.read());
 	ir_3 = <<<CANDIDATE_1>>>mul16u_exact(c_3,i_3.read());
 	ir_4 = <<<CANDIDATE_1>>>mul16u_exact(c_4,i_4.read());
 	ir_5 = <<<CANDIDATE_1>>>mul16u_exact(c_5,i_5.read());
 	ir_6 = <<<CANDIDATE_1>>>mul16u_exact(c_6,i_6.read());
 	ir_7 = <<<CANDIDATE_1>>>mul16u_exact(c_7,i_7.read());
 	ir_8 = <<<CANDIDATE_1>>>mul16u_exact(c_8,i_8.read());

 	//For sampling
 	// int_0.write(ir_0);
 	// int_1.write(ir_1);
 	// int_2.write(ir_2);
 	// int_3.write(ir_3);
 	// int_4.write(ir_4);
 	// int_5.write(ir_5);
 	// int_6.write(ir_6);
 	// int_7.write(ir_7);
 	// int_8.write(ir_8);

 	// Sum level -1
 	sum_0 = <<<CANDIDATE_0>>>adder_16bit(ir_0,ir_1);
 	sum_1 = <<<CANDIDATE_0>>>adder_16bit(ir_2,ir_3);
 	sum_2 = <<<CANDIDATE_0>>>adder_16bit(ir_4,ir_5);
 	sum_3 = <<<CANDIDATE_0>>>adder_16bit(ir_6,ir_7);

	// Sum level-2
 	sum_4 = <<<CANDIDATE_0>>>adder_16bit(sum_0,sum_1);
 	sum_5 = <<<CANDIDATE_0>>>adder_16bit(sum_2,sum_3); 
 	sum_6 = <<<CANDIDATE_0>>>adder_16bit(sum_4,sum_5); 
 	sum_7 = <<<CANDIDATE_0>>>adder_16bit(sum_6,ir_8); 


 	
 	// Final result
 	result.write(sum_7);

 	 	
 	//For sampling
 	// s_0.write(sum_0);
 	// s_1.write(sum_1);
 	// s_2.write(sum_2);
 	// s_3.write(sum_3);
 	// s_4.write(sum_4);
 	// s_5.write(sum_5);
 	// s_6.write(sum_6);
 	// s_7.write(sum_7);

 	// Input shift
 	i_1.write(sample.read());
 	i_2.write(i_1.read());
 	i_3.write(i_2.read());
 	i_4.write(i_3.read());
 	i_5.write(i_4.read());
 	i_6.write(i_5.read());
 	i_7.write(i_6.read());
 	i_8.write(i_7.read()); 	
	}
};
