#include <stdint.h>
#include <stdlib.h>

uint64_t mult8_cgp14ep_ep65536_wc16384_2_csamcsa(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_225, dout_267, dout_268, dout_299, dout_300, dout_302, dout_328, dout_331;   int avg=0;

   dout_225=((B >> 7)&1)&((A >> 6)&1);
   dout_267=((B >> 6)&1)&((A >> 7)&1);
   dout_268=((B >> 7)&1)&((A >> 7)&1);
   dout_299=dout_225|dout_267;
   dout_300=dout_225&dout_267;
   dout_302=dout_299|dout_268;
   dout_328=dout_268^dout_300;
   dout_331=dout_328^dout_302;

   O = 0;
   O |= (0&1) << 0;
   O |= (0&1) << 1;
   O |= (0&1) << 2;
   O |= (0&1) << 3;
   O |= (0&1) << 4;
   O |= (0&1) << 5;
   O |= (0&1) << 6;
   O |= (0&1) << 7;
   O |= (0&1) << 8;
   O |= (0&1) << 9;
   O |= (0&1) << 10;
   O |= (0&1) << 11;
   O |= (0&1) << 12;
   O |= (0&1) << 13;
   O |= (dout_331&1) << 14;
   O |= (dout_268&1) << 15;
   return O;
}


uint16_t trun8_tam00b(uint8_t A, uint8_t B)
{
  uint16_t P, P_;
  uint8_t tmp, C_1_0,C_1_1,C_1_2,C_1_3,C_1_4,C_1_5,C_1_6,C_2_0,C_2_1,C_2_2,C_2_3,C_2_4,C_2_5,C_2_6,C_3_0,C_3_1,C_3_2,C_3_3,C_3_4,C_3_5,C_3_6,C_4_0,C_4_1,C_4_2,C_4_3,C_4_4,C_4_5,C_4_6,C_5_0,C_5_1,C_5_2,C_5_3,C_5_4,C_5_5,C_5_6,C_6_0,C_6_1,C_6_2,C_6_3,C_6_4,C_6_5,C_6_6,C_7_0,C_7_1,C_7_2,C_7_3,C_7_4,C_7_5,C_7_6,S_0_0,S_0_1,S_0_2,S_0_3,S_0_4,S_0_5,S_0_6,S_0_7,S_1_0,S_1_1,S_1_2,S_1_3,S_1_4,S_1_5,S_1_6,S_1_7,S_2_0,S_2_1,S_2_2,S_2_3,S_2_4,S_2_5,S_2_6,S_2_7,S_3_0,S_3_1,S_3_2,S_3_3,S_3_4,S_3_5,S_3_6,S_3_7,S_4_0,S_4_1,S_4_2,S_4_3,S_4_4,S_4_5,S_4_6,S_4_7,S_5_0,S_5_1,S_5_2,S_5_3,S_5_4,S_5_5,S_5_6,S_5_7,S_6_0,S_6_1,S_6_2,S_6_3,S_6_4,S_6_5,S_6_6,S_6_7,S_7_0,S_7_1,S_7_2,S_7_3,S_7_4,S_7_5,S_7_6,S_7_7,S_8_0,S_8_1,S_8_2,S_8_3,S_8_4,S_8_5,S_8_6,S_8_7;
  S_0_0 = (((A>>0)&1) & ((B>>0)&1));
  S_0_1 = (((A>>0)&1) & ((B>>1)&1));
  S_0_2 = (((A>>0)&1) & ((B>>2)&1));
  S_0_3 = (((A>>0)&1) & ((B>>3)&1));
  S_0_4 = (((A>>0)&1) & ((B>>4)&1));
  S_0_5 = (((A>>0)&1) & ((B>>5)&1));
  S_0_6 = (((A>>0)&1) & ((B>>6)&1));
  S_0_7 = (((A>>0)&1) & ((B>>7)&1));
  S_1_0 = S_0_1^(((A>>1)&1) & ((B>>0)&1));
  C_1_0 = S_0_1&(((A>>1)&1) & ((B>>0)&1));
  S_1_1 = S_0_2^(((A>>1)&1) & ((B>>1)&1));
  C_1_1 = S_0_2&(((A>>1)&1) & ((B>>1)&1));
  S_1_2 = S_0_3^(((A>>1)&1) & ((B>>2)&1));
  C_1_2 = S_0_3&(((A>>1)&1) & ((B>>2)&1));
  S_1_3 = S_0_4^(((A>>1)&1) & ((B>>3)&1));
  C_1_3 = S_0_4&(((A>>1)&1) & ((B>>3)&1));
  S_1_4 = S_0_5^(((A>>1)&1) & ((B>>4)&1));
  C_1_4 = S_0_5&(((A>>1)&1) & ((B>>4)&1));
  S_1_5 = S_0_6^(((A>>1)&1) & ((B>>5)&1));
  C_1_5 = S_0_6&(((A>>1)&1) & ((B>>5)&1));
  S_1_6 = S_0_7^(((A>>1)&1) & ((B>>6)&1));
  C_1_6 = S_0_7&(((A>>1)&1) & ((B>>6)&1));
  S_1_7 = (((A>>1)&1) & ((B>>7)&1));
  tmp = S_1_1^C_1_0;
  S_2_0 = tmp^(((A>>2)&1) & ((B>>0)&1));
  C_2_0 = (tmp&(((A>>2)&1) & ((B>>0)&1)))|(S_1_1&C_1_0);
  tmp = S_1_2^C_1_1;
  S_2_1 = tmp^(((A>>2)&1) & ((B>>1)&1));
  C_2_1 = (tmp&(((A>>2)&1) & ((B>>1)&1)))|(S_1_2&C_1_1);
  tmp = S_1_3^C_1_2;
  S_2_2 = tmp^(((A>>2)&1) & ((B>>2)&1));
  C_2_2 = (tmp&(((A>>2)&1) & ((B>>2)&1)))|(S_1_3&C_1_2);
  tmp = S_1_4^C_1_3;
  S_2_3 = tmp^(((A>>2)&1) & ((B>>3)&1));
  C_2_3 = (tmp&(((A>>2)&1) & ((B>>3)&1)))|(S_1_4&C_1_3);
  tmp = S_1_5^C_1_4;
  S_2_4 = tmp^(((A>>2)&1) & ((B>>4)&1));
  C_2_4 = (tmp&(((A>>2)&1) & ((B>>4)&1)))|(S_1_5&C_1_4);
  tmp = S_1_6^C_1_5;
  S_2_5 = tmp^(((A>>2)&1) & ((B>>5)&1));
  C_2_5 = (tmp&(((A>>2)&1) & ((B>>5)&1)))|(S_1_6&C_1_5);
  tmp = S_1_7^C_1_6;
  S_2_6 = tmp^(((A>>2)&1) & ((B>>6)&1));
  C_2_6 = (tmp&(((A>>2)&1) & ((B>>6)&1)))|(S_1_7&C_1_6);
  S_2_7 = (((A>>2)&1) & ((B>>7)&1));
  tmp = S_2_1^C_2_0;
  S_3_0 = tmp^(((A>>3)&1) & ((B>>0)&1));
  C_3_0 = (tmp&(((A>>3)&1) & ((B>>0)&1)))|(S_2_1&C_2_0);
  tmp = S_2_2^C_2_1;
  S_3_1 = tmp^(((A>>3)&1) & ((B>>1)&1));
  C_3_1 = (tmp&(((A>>3)&1) & ((B>>1)&1)))|(S_2_2&C_2_1);
  tmp = S_2_3^C_2_2;
  S_3_2 = tmp^(((A>>3)&1) & ((B>>2)&1));
  C_3_2 = (tmp&(((A>>3)&1) & ((B>>2)&1)))|(S_2_3&C_2_2);
  tmp = S_2_4^C_2_3;
  S_3_3 = tmp^(((A>>3)&1) & ((B>>3)&1));
  C_3_3 = (tmp&(((A>>3)&1) & ((B>>3)&1)))|(S_2_4&C_2_3);
  tmp = S_2_5^C_2_4;
  S_3_4 = tmp^(((A>>3)&1) & ((B>>4)&1));
  C_3_4 = (tmp&(((A>>3)&1) & ((B>>4)&1)))|(S_2_5&C_2_4);
  tmp = S_2_6^C_2_5;
  S_3_5 = tmp^(((A>>3)&1) & ((B>>5)&1));
  C_3_5 = (tmp&(((A>>3)&1) & ((B>>5)&1)))|(S_2_6&C_2_5);
  tmp = S_2_7^C_2_6;
  S_3_6 = tmp^(((A>>3)&1) & ((B>>6)&1));
  C_3_6 = (tmp&(((A>>3)&1) & ((B>>6)&1)))|(S_2_7&C_2_6);
  S_3_7 = (((A>>3)&1) & ((B>>7)&1));
  tmp = S_3_1^C_3_0;
  S_4_0 = tmp^(((A>>4)&1) & ((B>>0)&1));
  C_4_0 = (tmp&(((A>>4)&1) & ((B>>0)&1)))|(S_3_1&C_3_0);
  tmp = S_3_2^C_3_1;
  S_4_1 = tmp^(((A>>4)&1) & ((B>>1)&1));
  C_4_1 = (tmp&(((A>>4)&1) & ((B>>1)&1)))|(S_3_2&C_3_1);
  tmp = S_3_3^C_3_2;
  S_4_2 = tmp^(((A>>4)&1) & ((B>>2)&1));
  C_4_2 = (tmp&(((A>>4)&1) & ((B>>2)&1)))|(S_3_3&C_3_2);
  tmp = S_3_4^C_3_3;
  S_4_3 = tmp^(((A>>4)&1) & ((B>>3)&1));
  C_4_3 = (tmp&(((A>>4)&1) & ((B>>3)&1)))|(S_3_4&C_3_3);
  tmp = S_3_5^C_3_4;
  S_4_4 = tmp^(((A>>4)&1) & ((B>>4)&1));
  C_4_4 = (tmp&(((A>>4)&1) & ((B>>4)&1)))|(S_3_5&C_3_4);
  tmp = S_3_6^C_3_5;
  S_4_5 = tmp^(((A>>4)&1) & ((B>>5)&1));
  C_4_5 = (tmp&(((A>>4)&1) & ((B>>5)&1)))|(S_3_6&C_3_5);
  tmp = S_3_7^C_3_6;
  S_4_6 = tmp^(((A>>4)&1) & ((B>>6)&1));
  C_4_6 = (tmp&(((A>>4)&1) & ((B>>6)&1)))|(S_3_7&C_3_6);
  S_4_7 = (((A>>4)&1) & ((B>>7)&1));
  tmp = S_4_1^C_4_0;
  S_5_0 = tmp^(((A>>5)&1) & ((B>>0)&1));
  C_5_0 = (tmp&(((A>>5)&1) & ((B>>0)&1)))|(S_4_1&C_4_0);
  tmp = S_4_2^C_4_1;
  S_5_1 = tmp^(((A>>5)&1) & ((B>>1)&1));
  C_5_1 = (tmp&(((A>>5)&1) & ((B>>1)&1)))|(S_4_2&C_4_1);
  tmp = S_4_3^C_4_2;
  S_5_2 = tmp^(((A>>5)&1) & ((B>>2)&1));
  C_5_2 = (tmp&(((A>>5)&1) & ((B>>2)&1)))|(S_4_3&C_4_2);
  tmp = S_4_4^C_4_3;
  S_5_3 = tmp^(((A>>5)&1) & ((B>>3)&1));
  C_5_3 = (tmp&(((A>>5)&1) & ((B>>3)&1)))|(S_4_4&C_4_3);
  tmp = S_4_5^C_4_4;
  S_5_4 = tmp^(((A>>5)&1) & ((B>>4)&1));
  C_5_4 = (tmp&(((A>>5)&1) & ((B>>4)&1)))|(S_4_5&C_4_4);
  tmp = S_4_6^C_4_5;
  S_5_5 = tmp^(((A>>5)&1) & ((B>>5)&1));
  C_5_5 = (tmp&(((A>>5)&1) & ((B>>5)&1)))|(S_4_6&C_4_5);
  tmp = S_4_7^C_4_6;
  S_5_6 = tmp^(((A>>5)&1) & ((B>>6)&1));
  C_5_6 = (tmp&(((A>>5)&1) & ((B>>6)&1)))|(S_4_7&C_4_6);
  S_5_7 = (((A>>5)&1) & ((B>>7)&1));
  tmp = S_5_1^C_5_0;
  S_6_0 = tmp^(((A>>6)&1) & ((B>>0)&1));
  C_6_0 = (tmp&(((A>>6)&1) & ((B>>0)&1)))|(S_5_1&C_5_0);
  tmp = S_5_2^C_5_1;
  S_6_1 = tmp^(((A>>6)&1) & ((B>>1)&1));
  C_6_1 = (tmp&(((A>>6)&1) & ((B>>1)&1)))|(S_5_2&C_5_1);
  tmp = S_5_3^C_5_2;
  S_6_2 = tmp^(((A>>6)&1) & ((B>>2)&1));
  C_6_2 = (tmp&(((A>>6)&1) & ((B>>2)&1)))|(S_5_3&C_5_2);
  tmp = S_5_4^C_5_3;
  S_6_3 = tmp^(((A>>6)&1) & ((B>>3)&1));
  C_6_3 = (tmp&(((A>>6)&1) & ((B>>3)&1)))|(S_5_4&C_5_3);
  tmp = S_5_5^C_5_4;
  S_6_4 = tmp^(((A>>6)&1) & ((B>>4)&1));
  C_6_4 = (tmp&(((A>>6)&1) & ((B>>4)&1)))|(S_5_5&C_5_4);
  tmp = S_5_6^C_5_5;
  S_6_5 = tmp^(((A>>6)&1) & ((B>>5)&1));
  C_6_5 = (tmp&(((A>>6)&1) & ((B>>5)&1)))|(S_5_6&C_5_5);
  tmp = S_5_7^C_5_6;
  S_6_6 = tmp^(((A>>6)&1) & ((B>>6)&1));
  C_6_6 = (tmp&(((A>>6)&1) & ((B>>6)&1)))|(S_5_7&C_5_6);
  S_6_7 = (((A>>6)&1) & ((B>>7)&1));
  tmp = S_6_1^C_6_0;
  S_7_0 = tmp^(((A>>7)&1) & ((B>>0)&1));
  C_7_0 = (tmp&(((A>>7)&1) & ((B>>0)&1)))|(S_6_1&C_6_0);
  tmp = S_6_2^C_6_1;
  S_7_1 = tmp^(((A>>7)&1) & ((B>>1)&1));
  C_7_1 = (tmp&(((A>>7)&1) & ((B>>1)&1)))|(S_6_2&C_6_1);
  tmp = S_6_3^C_6_2;
  S_7_2 = tmp^(((A>>7)&1) & ((B>>2)&1));
  C_7_2 = (tmp&(((A>>7)&1) & ((B>>2)&1)))|(S_6_3&C_6_2);
  tmp = S_6_4^C_6_3;
  S_7_3 = tmp^(((A>>7)&1) & ((B>>3)&1));
  C_7_3 = (tmp&(((A>>7)&1) & ((B>>3)&1)))|(S_6_4&C_6_3);
  tmp = S_6_5^C_6_4;
  S_7_4 = tmp^(((A>>7)&1) & ((B>>4)&1));
  C_7_4 = (tmp&(((A>>7)&1) & ((B>>4)&1)))|(S_6_5&C_6_4);
  tmp = S_6_6^C_6_5;
  S_7_5 = tmp^(((A>>7)&1) & ((B>>5)&1));
  C_7_5 = (tmp&(((A>>7)&1) & ((B>>5)&1)))|(S_6_6&C_6_5);
  tmp = S_6_7^C_6_6;
  S_7_6 = tmp^(((A>>7)&1) & ((B>>6)&1));
  C_7_6 = (tmp&(((A>>7)&1) & ((B>>6)&1)))|(S_6_7&C_6_6);
  S_7_7 = (((A>>7)&1) & ((B>>7)&1));
  P_ = (((C_7_0 & 1)<<0)|((C_7_1 & 1)<<1)|((C_7_2 & 1)<<2)|((C_7_3 & 1)<<3)|((C_7_4 & 1)<<4)|((C_7_5 & 1)<<5)|((C_7_6 & 1)<<6)) + (((S_7_1 & 1)<<0)|((S_7_2 & 1)<<1)|((S_7_3 & 1)<<2)|((S_7_4 & 1)<<3)|((S_7_5 & 1)<<4)|((S_7_6 & 1)<<5)|((S_7_7 & 1)<<6));
  S_8_0 = (P_ >> 0) & 1;
  S_8_1 = (P_ >> 1) & 1;
  S_8_2 = (P_ >> 2) & 1;
  S_8_3 = (P_ >> 3) & 1;
  S_8_4 = (P_ >> 4) & 1;
  S_8_5 = (P_ >> 5) & 1;
  S_8_6 = (P_ >> 6) & 1;
  S_8_7 = (P_ >> 7) & 1;
  P = 0;
  P |= (S_0_0 & 1) << 0;
  P |= (S_1_0 & 1) << 1;
  P |= (S_2_0 & 1) << 2;
  P |= (S_3_0 & 1) << 3;
  P |= (S_4_0 & 1) << 4;
  P |= (S_5_0 & 1) << 5;
  P |= (S_6_0 & 1) << 6;
  P |= (S_7_0 & 1) << 7;
  P |= (S_8_0 & 1) << 8;
  P |= (S_8_1 & 1) << 9;
  P |= (S_8_2 & 1) << 10;
  P |= (S_8_3 & 1) << 11;
  P |= (S_8_4 & 1) << 12;
  P |= (S_8_5 & 1) << 13;
  P |= (S_8_6 & 1) << 14;
  P |= (S_8_7 & 1) << 15;
  return P;
}


uint16_t mul8_364(uint8_t a, uint8_t b)
{
  uint16_t c = 0;
  uint8_t n0 = (a >> 0) & 0x1;
  uint8_t n2 = (a >> 1) & 0x1;
  uint8_t n4 = (a >> 2) & 0x1;
  uint8_t n6 = (a >> 3) & 0x1;
  uint8_t n8 = (a >> 4) & 0x1;
  uint8_t n10 = (a >> 5) & 0x1;
  uint8_t n12 = (a >> 6) & 0x1;
  uint8_t n14 = (a >> 7) & 0x1;
  uint8_t n16 = (b >> 0) & 0x1;
  uint8_t n18 = (b >> 1) & 0x1;
  uint8_t n20 = (b >> 2) & 0x1;
  uint8_t n22 = (b >> 3) & 0x1;
  uint8_t n24 = (b >> 4) & 0x1;
  uint8_t n26 = (b >> 5) & 0x1;
  uint8_t n28 = (b >> 6) & 0x1;
  uint8_t n30 = (b >> 7) & 0x1;
  uint8_t n32;
  uint8_t n48;
  uint8_t n64;
  uint8_t n82;
  uint8_t n98;
  uint8_t n114;
  uint8_t n132;
  uint8_t n149;
  uint8_t n164;
  uint8_t n167;
  uint8_t n182;
  uint8_t n198;
  uint8_t n214;
  uint8_t n232;
  uint8_t n248;
  uint8_t n264;
  uint8_t n282;
  uint8_t n298;
  uint8_t n299;
  uint8_t n314;
  uint8_t n315;
  uint8_t n332;
  uint8_t n333;
  uint8_t n348;
  uint8_t n349;
  uint8_t n364;
  uint8_t n365;
  uint8_t n382;
  uint8_t n383;
  uint8_t n398;
  uint8_t n399;
  uint8_t n414;
  uint8_t n432;
  uint8_t n448;
  uint8_t n464;
  uint8_t n482;
  uint8_t n498;
  uint8_t n514;
  uint8_t n532;
  uint8_t n548;
  uint8_t n549;
  uint8_t n564;
  uint8_t n565;
  uint8_t n582;
  uint8_t n583;
  uint8_t n598;
  uint8_t n599;
  uint8_t n614;
  uint8_t n615;
  uint8_t n632;
  uint8_t n633;
  uint8_t n648;
  uint8_t n649;
  uint8_t n664;
  uint8_t n682;
  uint8_t n698;
  uint8_t n714;
  uint8_t n732;
  uint8_t n748;
  uint8_t n764;
  uint8_t n782;
  uint8_t n798;
  uint8_t n799;
  uint8_t n814;
  uint8_t n815;
  uint8_t n832;
  uint8_t n833;
  uint8_t n848;
  uint8_t n849;
  uint8_t n864;
  uint8_t n865;
  uint8_t n882;
  uint8_t n883;
  uint8_t n898;
  uint8_t n899;
  uint8_t n914;
  uint8_t n932;
  uint8_t n948;
  uint8_t n964;
  uint8_t n982;
  uint8_t n998;
  uint8_t n1014;
  uint8_t n1032;
  uint8_t n1048;
  uint8_t n1049;
  uint8_t n1064;
  uint8_t n1065;
  uint8_t n1082;
  uint8_t n1083;
  uint8_t n1098;
  uint8_t n1099;
  uint8_t n1114;
  uint8_t n1115;
  uint8_t n1132;
  uint8_t n1133;
  uint8_t n1148;
  uint8_t n1149;
  uint8_t n1164;
  uint8_t n1182;
  uint8_t n1198;
  uint8_t n1214;
  uint8_t n1232;
  uint8_t n1248;
  uint8_t n1264;
  uint8_t n1282;
  uint8_t n1298;
  uint8_t n1299;
  uint8_t n1314;
  uint8_t n1315;
  uint8_t n1332;
  uint8_t n1333;
  uint8_t n1348;
  uint8_t n1349;
  uint8_t n1364;
  uint8_t n1365;
  uint8_t n1382;
  uint8_t n1383;
  uint8_t n1398;
  uint8_t n1399;
  uint8_t n1414;
  uint8_t n1432;
  uint8_t n1448;
  uint8_t n1464;
  uint8_t n1482;
  uint8_t n1498;
  uint8_t n1514;
  uint8_t n1532;
  uint8_t n1548;
  uint8_t n1549;
  uint8_t n1564;
  uint8_t n1565;
  uint8_t n1582;
  uint8_t n1583;
  uint8_t n1598;
  uint8_t n1599;
  uint8_t n1614;
  uint8_t n1615;
  uint8_t n1632;
  uint8_t n1633;
  uint8_t n1648;
  uint8_t n1649;
  uint8_t n1664;
  uint8_t n1682;
  uint8_t n1698;
  uint8_t n1714;
  uint8_t n1732;
  uint8_t n1748;
  uint8_t n1764;
  uint8_t n1782;
  uint8_t n1798;
  uint8_t n1799;
  uint8_t n1814;
  uint8_t n1815;
  uint8_t n1832;
  uint8_t n1833;
  uint8_t n1848;
  uint8_t n1849;
  uint8_t n1864;
  uint8_t n1865;
  uint8_t n1882;
  uint8_t n1883;
  uint8_t n1898;
  uint8_t n1899;
  uint8_t n1914;
  uint8_t n1915;
  uint8_t n1932;
  uint8_t n1933;
  uint8_t n1948;
  uint8_t n1949;
  uint8_t n1964;
  uint8_t n1965;
  uint8_t n1982;
  uint8_t n1983;
  uint8_t n1998;
  uint8_t n1999;
  uint8_t n2014;
  uint8_t n2015;

  n32 = n0 & n16;
  n48 = n2 & n16;
  n64 = n4 & n16;
  n82 = n6 & n16;
  n98 = n8 & n16;
  n114 = n10 & n16;
  n132 = n12 & n16;
  n149 = n14 & n16;
  n164 = n0 & n18;
  n167 = n149;
  n182 = n2 & n18;
  n198 = n4 & n18;
  n214 = n6 & n18;
  n232 = n8 & n18;
  n248 = n10 & n18;
  n264 = n12 & n18;
  n282 = n14 & n18;
  n298 = n48 ^ n164;
  n299 = n48 & n164;
  n314 = n64 ^ n182;
  n315 = n64 & n182;
  n332 = n82 ^ n198;
  n333 = n82 & n198;
  n348 = n98 ^ n214;
  n349 = n98 & n214;
  n364 = n114 ^ n232;
  n365 = n114 & n232;
  n382 = n132 ^ n248;
  n383 = n132 & n248;
  n398 = n167 ^ n264;
  n399 = n167 & n264;
  n414 = n0 & n20;
  n432 = n2 & n20;
  n448 = n4 & n20;
  n464 = n6 & n20;
  n482 = n8 & n20;
  n498 = n10 & n20;
  n514 = n12 & n20;
  n532 = n14 & n20;
  n548 = (n314 ^ n414) ^ n299;
  n549 = (n314 & n414) | (n414 & n299) | (n314 & n299);
  n564 = (n332 ^ n432) ^ n315;
  n565 = (n332 & n432) | (n432 & n315) | (n332 & n315);
  n582 = (n348 ^ n448) ^ n333;
  n583 = (n348 & n448) | (n448 & n333) | (n348 & n333);
  n598 = (n364 ^ n464) ^ n349;
  n599 = (n364 & n464) | (n464 & n349) | (n364 & n349);
  n614 = (n382 ^ n482) ^ n365;
  n615 = (n382 & n482) | (n482 & n365) | (n382 & n365);
  n632 = (n398 ^ n498) ^ n383;
  n633 = (n398 & n498) | (n498 & n383) | (n398 & n383);
  n648 = (n282 ^ n514) ^ n399;
  n649 = (n282 & n514) | (n514 & n399) | (n282 & n399);
  n664 = n0 & n22;
  n682 = n2 & n22;
  n698 = n4 & n22;
  n714 = n6 & n22;
  n732 = n8 & n22;
  n748 = n10 & n22;
  n764 = n12 & n22;
  n782 = n14 & n22;
  n798 = (n564 ^ n664) ^ n549;
  n799 = (n564 & n664) | (n664 & n549) | (n564 & n549);
  n814 = (n582 ^ n682) ^ n565;
  n815 = (n582 & n682) | (n682 & n565) | (n582 & n565);
  n832 = (n598 ^ n698) ^ n583;
  n833 = (n598 & n698) | (n698 & n583) | (n598 & n583);
  n848 = (n614 ^ n714) ^ n599;
  n849 = (n614 & n714) | (n714 & n599) | (n614 & n599);
  n864 = (n632 ^ n732) ^ n615;
  n865 = (n632 & n732) | (n732 & n615) | (n632 & n615);
  n882 = (n648 ^ n748) ^ n633;
  n883 = (n648 & n748) | (n748 & n633) | (n648 & n633);
  n898 = (n532 ^ n764) ^ n649;
  n899 = (n532 & n764) | (n764 & n649) | (n532 & n649);
  n914 = n0 & n24;
  n932 = n2 & n24;
  n948 = n4 & n24;
  n964 = n6 & n24;
  n982 = n8 & n24;
  n998 = n10 & n24;
  n1014 = n12 & n24;
  n1032 = n14 & n24;
  n1048 = (n814 ^ n914) ^ n799;
  n1049 = (n814 & n914) | (n914 & n799) | (n814 & n799);
  n1064 = (n832 ^ n932) ^ n815;
  n1065 = (n832 & n932) | (n932 & n815) | (n832 & n815);
  n1082 = (n848 ^ n948) ^ n833;
  n1083 = (n848 & n948) | (n948 & n833) | (n848 & n833);
  n1098 = (n864 ^ n964) ^ n849;
  n1099 = (n864 & n964) | (n964 & n849) | (n864 & n849);
  n1114 = (n882 ^ n982) ^ n865;
  n1115 = (n882 & n982) | (n982 & n865) | (n882 & n865);
  n1132 = (n898 ^ n998) ^ n883;
  n1133 = (n898 & n998) | (n998 & n883) | (n898 & n883);
  n1148 = (n782 ^ n1014) ^ n899;
  n1149 = (n782 & n1014) | (n1014 & n899) | (n782 & n899);
  n1164 = n0 & n26;
  n1182 = n2 & n26;
  n1198 = n4 & n26;
  n1214 = n6 & n26;
  n1232 = n8 & n26;
  n1248 = n10 & n26;
  n1264 = n12 & n26;
  n1282 = n14 & n26;
  n1298 = (n1064 ^ n1164) ^ n1049;
  n1299 = (n1064 & n1164) | (n1164 & n1049) | (n1064 & n1049);
  n1314 = (n1082 ^ n1182) ^ n1065;
  n1315 = (n1082 & n1182) | (n1182 & n1065) | (n1082 & n1065);
  n1332 = (n1098 ^ n1198) ^ n1083;
  n1333 = (n1098 & n1198) | (n1198 & n1083) | (n1098 & n1083);
  n1348 = (n1114 ^ n1214) ^ n1099;
  n1349 = (n1114 & n1214) | (n1214 & n1099) | (n1114 & n1099);
  n1364 = (n1132 ^ n1232) ^ n1115;
  n1365 = (n1132 & n1232) | (n1232 & n1115) | (n1132 & n1115);
  n1382 = (n1148 ^ n1248) ^ n1133;
  n1383 = (n1148 & n1248) | (n1248 & n1133) | (n1148 & n1133);
  n1398 = (n1032 ^ n1264) ^ n1149;
  n1399 = (n1032 & n1264) | (n1264 & n1149) | (n1032 & n1149);
  n1414 = n0 & n28;
  n1432 = n2 & n28;
  n1448 = n4 & n28;
  n1464 = n6 & n28;
  n1482 = n8 & n28;
  n1498 = n10 & n28;
  n1514 = n12 & n28;
  n1532 = n14 & n28;
  n1548 = (n1314 ^ n1414) ^ n1299;
  n1549 = (n1314 & n1414) | (n1414 & n1299) | (n1314 & n1299);
  n1564 = (n1332 ^ n1432) ^ n1315;
  n1565 = (n1332 & n1432) | (n1432 & n1315) | (n1332 & n1315);
  n1582 = (n1348 ^ n1448) ^ n1333;
  n1583 = (n1348 & n1448) | (n1448 & n1333) | (n1348 & n1333);
  n1598 = (n1364 ^ n1464) ^ n1349;
  n1599 = (n1364 & n1464) | (n1464 & n1349) | (n1364 & n1349);
  n1614 = (n1382 ^ n1482) ^ n1365;
  n1615 = (n1382 & n1482) | (n1482 & n1365) | (n1382 & n1365);
  n1632 = (n1398 ^ n1498) ^ n1383;
  n1633 = (n1398 & n1498) | (n1498 & n1383) | (n1398 & n1383);
  n1648 = (n1282 ^ n1514) ^ n1399;
  n1649 = (n1282 & n1514) | (n1514 & n1399) | (n1282 & n1399);
  n1664 = n0 & n30;
  n1682 = n2 & n30;
  n1698 = n4 & n30;
  n1714 = n6 & n30;
  n1732 = n8 & n30;
  n1748 = n10 & n30;
  n1764 = n12 & n30;
  n1782 = n14 & n30;
  n1798 = (n1564 ^ n1664) ^ n1549;
  n1799 = (n1564 & n1664) | (n1664 & n1549) | (n1564 & n1549);
  n1814 = (n1582 ^ n1682) ^ n1565;
  n1815 = (n1582 & n1682) | (n1682 & n1565) | (n1582 & n1565);
  n1832 = (n1598 ^ n1698) ^ n1583;
  n1833 = (n1598 & n1698) | (n1698 & n1583) | (n1598 & n1583);
  n1848 = (n1614 ^ n1714) ^ n1599;
  n1849 = (n1614 & n1714) | (n1714 & n1599) | (n1614 & n1599);
  n1864 = (n1632 ^ n1732) ^ n1615;
  n1865 = (n1632 & n1732) | (n1732 & n1615) | (n1632 & n1615);
  n1882 = (n1648 ^ n1748) ^ n1633;
  n1883 = (n1648 & n1748) | (n1748 & n1633) | (n1648 & n1633);
  n1898 = (n1532 ^ n1764) ^ n1649;
  n1899 = (n1532 & n1764) | (n1764 & n1649) | (n1532 & n1649);
  n1914 = n1814 ^ n1799;
  n1915 = n1814 & n1799;
  n1932 = (n1832 ^ n1815) ^ n1915;
  n1933 = (n1832 & n1815) | (n1815 & n1915) | (n1832 & n1915);
  n1948 = (n1848 ^ n1833) ^ n1933;
  n1949 = (n1848 & n1833) | (n1833 & n1933) | (n1848 & n1933);
  n1964 = (n1864 ^ n1849) ^ n1949;
  n1965 = (n1864 & n1849) | (n1849 & n1949) | (n1864 & n1949);
  n1982 = (n1882 ^ n1865) ^ n1965;
  n1983 = (n1882 & n1865) | (n1865 & n1965) | (n1882 & n1965);
  n1998 = (n1898 ^ n1883) ^ n1983;
  n1999 = (n1898 & n1883) | (n1883 & n1983) | (n1898 & n1983);
  n2014 = (n1782 ^ n1899) ^ n1999;
  n2015 = (n1782 & n1899) | (n1899 & n1999) | (n1782 & n1999);

  c |= (n32 & 0x1) << 0;
  c |= (n298 & 0x1) << 1;
  c |= (n548 & 0x1) << 2;
  c |= (n798 & 0x1) << 3;
  c |= (n1048 & 0x1) << 4;
  c |= (n1298 & 0x1) << 5;
  c |= (n1548 & 0x1) << 6;
  c |= (n1798 & 0x1) << 7;
  c |= (n1914 & 0x1) << 8;
  c |= (n1932 & 0x1) << 9;
  c |= (n1948 & 0x1) << 10;
  c |= (n1964 & 0x1) << 11;
  c |= (n1982 & 0x1) << 12;
  c |= (n1998 & 0x1) << 13;
  c |= (n2014 & 0x1) << 14;
  c |= (n2015 & 0x1) << 15;

  return c;
}

uint64_t mult8_cgp14ep_ep63897_wc377_rcam(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_23, dout_30, dout_31, dout_59, dout_60, dout_63, dout_64, dout_72, dout_73, dout_90, dout_97, dout_101, dout_102, dout_103, dout_104, dout_105, dout_106, dout_107, dout_108, dout_109, dout_110, dout_114, dout_115, dout_117, dout_118, dout_141, dout_142, dout_143, dout_144, dout_145, dout_146, dout_147, dout_148, dout_149, dout_150, dout_151, dout_152, dout_153, dout_154, dout_155, dout_160, dout_161, dout_162, dout_163, dout_165, dout_172, dout_173, dout_176, dout_177, dout_181, dout_182, dout_183, dout_184, dout_185, dout_186, dout_187, dout_188, dout_189, dout_190, dout_191, dout_193, dout_194, dout_195, dout_196, dout_198, dout_199, dout_200, dout_204, dout_205, dout_206, dout_207, dout_208, dout_216, dout_221, dout_222, dout_223, dout_224, dout_225, dout_226, dout_227, dout_228, dout_229, dout_230, dout_231, dout_232, dout_233, dout_234, dout_235, dout_236, dout_237, dout_238, dout_239, dout_240, dout_241, dout_242, dout_243, dout_244, dout_245, dout_247, dout_248, dout_249, dout_250, dout_251, dout_252, dout_253, dout_256, dout_259, dout_261, dout_262, dout_263, dout_264, dout_265, dout_266, dout_267, dout_268, dout_269, dout_270, dout_271, dout_272, dout_273, dout_274, dout_275, dout_276, dout_277, dout_278, dout_279, dout_280, dout_281, dout_282, dout_283, dout_284, dout_285, dout_286, dout_287, dout_288, dout_289, dout_290, dout_291, dout_292, dout_293, dout_294, dout_295, dout_296, dout_297, dout_298, dout_299, dout_300, dout_301, dout_302, dout_303, dout_304, dout_305, dout_306, dout_307, dout_308, dout_309, dout_310, dout_311, dout_312, dout_313, dout_314, dout_315, dout_316, dout_317, dout_318, dout_319, dout_320, dout_321, dout_322, dout_323, dout_324, dout_325, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_334, dout_335;   int avg=0;

   dout_23=((B >> 7)&1)&((A >> 0)&1);
   dout_30=((B >> 6)&1)&((A >> 1)&1);
   dout_31=((B >> 7)&1)&((A >> 1)&1);
   dout_59=dout_23|dout_30;
   dout_60=dout_23&dout_30;
   dout_63=dout_31&((B >> 6)&1);
   dout_64=dout_60^dout_31;
   dout_72=((B >> 6)&1)&((A >> 2)&1);
   dout_73=((B >> 7)&1)&((A >> 2)&1);
   dout_90=dout_63&((A >> 0)&1);
   dout_97=((A >> 2)&1)&((B >> 5)&1);
   dout_101=dout_64^dout_72;
   dout_102=dout_64&dout_72;
   dout_103=dout_101&dout_97;
   dout_104=dout_101^dout_97;
   dout_105=dout_102|dout_103;
   dout_106=dout_90^dout_73;
   dout_107=dout_63&((A >> 2)&1);
   dout_108=dout_73&dout_105;
   dout_109=dout_106^dout_105;
   dout_110=dout_107|dout_108;
   dout_114=((B >> 5)&1)&((A >> 3)&1);
   dout_115=((B >> 4)&1)&((A >> 3)&1);
   dout_117=((B >> 6)&1)&((A >> 3)&1);
   dout_118=((B >> 7)&1)&((A >> 3)&1);
   dout_141=dout_104^dout_114;
   dout_142=dout_104&dout_114;
   dout_143=dout_141&dout_115;
   dout_144=dout_141^dout_115;
   dout_145=dout_142|dout_143;
   dout_146=dout_109^dout_117;
   dout_147=dout_109&dout_117;
   dout_148=dout_146&dout_145;
   dout_149=dout_146^dout_145;
   dout_150=dout_147|dout_148;
   dout_151=dout_110^dout_118;
   dout_152=dout_110&((A >> 3)&1);
   dout_153=((B >> 7)&1)&dout_150;
   dout_154=dout_151^dout_150;
   dout_155=dout_152|dout_153;
   dout_160=((B >> 4)&1)&((A >> 4)&1);
   dout_161=((B >> 5)&1)&((A >> 4)&1);
   dout_162=((B >> 6)&1)&((A >> 4)&1);
   dout_163=((B >> 7)&1)&((A >> 4)&1);
   dout_165=dout_162&dout_154;
   dout_172=((B >> 2)&1)&((A >> 5)&1);
   dout_173=dout_155&((A >> 4)&1);
   dout_176=dout_59|dout_172;
   dout_177=((A >> 4)&1)&((B >> 3)&1);
   dout_181=dout_144^dout_160;
   dout_182=dout_144&dout_160;
   dout_183=dout_181&dout_177;
   dout_184=dout_181^dout_177;
   dout_185=dout_182|dout_183;
   dout_186=dout_149^dout_161;
   dout_187=dout_149&dout_161;
   dout_188=dout_186&dout_185;
   dout_189=dout_186^dout_185;
   dout_190=dout_187|dout_188;
   dout_191=dout_154^dout_162;
   dout_193=dout_191&dout_190;
   dout_194=dout_191^dout_190;
   dout_195=dout_165|dout_193;
   dout_196=dout_155^dout_163;
   dout_198=dout_163&dout_195;
   dout_199=dout_196^dout_195;
   dout_200=dout_173|dout_198;
   dout_204=((B >> 3)&1)&((A >> 5)&1);
   dout_205=((B >> 4)&1)&((A >> 5)&1);
   dout_206=((B >> 5)&1)&((A >> 5)&1);
   dout_207=((B >> 6)&1)&((A >> 5)&1);
   dout_208=((B >> 7)&1)&((A >> 5)&1);
   dout_216=dout_176^dout_172;
   dout_221=dout_184^dout_204;
   dout_222=dout_184&dout_204;
   dout_223=dout_221&dout_172;
   dout_224=dout_221^dout_172;
   dout_225=dout_222|dout_223;
   dout_226=dout_189^dout_205;
   dout_227=dout_189&dout_205;
   dout_228=dout_226&dout_225;
   dout_229=dout_226^dout_225;
   dout_230=dout_227|dout_228;
   dout_231=dout_194^dout_206;
   dout_232=dout_194&dout_206;
   dout_233=dout_231&dout_230;
   dout_234=dout_231^dout_230;
   dout_235=dout_232|dout_233;
   dout_236=dout_199^dout_207;
   dout_237=dout_199&dout_207;
   dout_238=dout_236&dout_235;
   dout_239=dout_236^dout_235;
   dout_240=dout_237|dout_238;
   dout_241=dout_200^dout_208;
   dout_242=dout_200&((A >> 5)&1);
   dout_243=dout_208&dout_240;
   dout_244=dout_241^dout_240;
   dout_245=dout_242|dout_243;
   dout_247=((B >> 1)&1)&((A >> 6)&1);
   dout_248=((B >> 2)&1)&((A >> 6)&1);
   dout_249=((B >> 3)&1)&((A >> 6)&1);
   dout_250=((B >> 4)&1)&((A >> 6)&1);
   dout_251=((B >> 5)&1)&((A >> 6)&1);
   dout_252=((B >> 6)&1)&((A >> 6)&1);
   dout_253=((B >> 7)&1)&((A >> 6)&1);
   dout_256=dout_216|dout_247;
   dout_259=dout_256^dout_247;
   dout_261=dout_224^dout_248;
   dout_262=dout_224&dout_248;
   dout_263=dout_261&dout_247;
   dout_264=dout_261^dout_247;
   dout_265=dout_262|dout_263;
   dout_266=dout_229^dout_249;
   dout_267=dout_229&dout_249;
   dout_268=dout_266&dout_265;
   dout_269=dout_266^dout_265;
   dout_270=dout_267|dout_268;
   dout_271=dout_234^dout_250;
   dout_272=dout_234&dout_250;
   dout_273=dout_271&dout_270;
   dout_274=dout_271^dout_270;
   dout_275=dout_272|dout_273;
   dout_276=dout_239^dout_251;
   dout_277=dout_239&dout_251;
   dout_278=dout_276&dout_275;
   dout_279=dout_276^dout_275;
   dout_280=dout_277|dout_278;
   dout_281=dout_244^dout_252;
   dout_282=dout_244&dout_252;
   dout_283=dout_281&dout_280;
   dout_284=dout_281^dout_280;
   dout_285=dout_282|dout_283;
   dout_286=dout_245^dout_253;
   dout_287=dout_245&((A >> 6)&1);
   dout_288=dout_253&dout_285;
   dout_289=dout_286^dout_285;
   dout_290=dout_287|dout_288;
   dout_291=((B >> 0)&1)&((A >> 7)&1);
   dout_292=((B >> 1)&1)&((A >> 7)&1);
   dout_293=((B >> 2)&1)&((A >> 7)&1);
   dout_294=((B >> 3)&1)&((A >> 7)&1);
   dout_295=((B >> 4)&1)&((A >> 7)&1);
   dout_296=((B >> 5)&1)&((A >> 7)&1);
   dout_297=((B >> 6)&1)&((A >> 7)&1);
   dout_298=((B >> 7)&1)&((A >> 7)&1);
   dout_299=dout_259|dout_291;
   dout_300=dout_259&dout_291;
   dout_301=dout_264^dout_292;
   dout_302=dout_264&dout_292;
   dout_303=dout_301&dout_300;
   dout_304=dout_301^dout_300;
   dout_305=dout_302|dout_303;
   dout_306=dout_269^dout_293;
   dout_307=dout_269&dout_293;
   dout_308=dout_306&dout_305;
   dout_309=dout_306^dout_305;
   dout_310=dout_307|dout_308;
   dout_311=dout_274^dout_294;
   dout_312=dout_274&dout_294;
   dout_313=dout_311&dout_310;
   dout_314=dout_311^dout_310;
   dout_315=dout_312|dout_313;
   dout_316=dout_279^dout_295;
   dout_317=dout_279&dout_295;
   dout_318=dout_316&dout_315;
   dout_319=dout_316^dout_315;
   dout_320=dout_317|dout_318;
   dout_321=dout_284^dout_296;
   dout_322=dout_284&dout_296;
   dout_323=dout_321&dout_320;
   dout_324=dout_321^dout_320;
   dout_325=dout_322|dout_323;
   dout_326=dout_289^dout_297;
   dout_327=dout_289&dout_297;
   dout_328=dout_326&dout_325;
   dout_329=dout_326^dout_325;
   dout_330=dout_327|dout_328;
   dout_331=dout_290^dout_298;
   dout_332=dout_290&((A >> 7)&1);
   dout_333=((B >> 7)&1)&dout_330;
   dout_334=dout_331^dout_330;
   dout_335=dout_332|dout_333;

   O = 0;
   O |= (0&1) << 0;
   O |= (dout_73&1) << 1;
   O |= (dout_172&1) << 2;
   O |= (dout_115&1) << 3;
   O |= (0&1) << 4;
   O |= (dout_222&1) << 5;
   O |= (dout_59&1) << 6;
   O |= (dout_299&1) << 7;
   O |= (dout_304&1) << 8;
   O |= (dout_309&1) << 9;
   O |= (dout_314&1) << 10;
   O |= (dout_319&1) << 11;
   O |= (dout_324&1) << 12;
   O |= (dout_329&1) << 13;
   O |= (dout_334&1) << 14;
   O |= (dout_335&1) << 15;
   return O;
}



uint64_t adder_16bit(uint64_t a,uint64_t b){
  return a+b;
}
uint64_t add16u_0AV(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_32 = wa[0] ^ wb[0];
  int sig_33 = wa[0] & wb[0];
  int sig_34 = wa[1] ^ wb[1];
  int sig_35 = wa[1] & wb[1];
  int sig_36 = wa[0] & wb[0];
  int sig_37 = sig_34 ^ sig_33;
  int sig_38 = sig_35 | sig_36;
  int sig_39 = wa[2] ^ wb[2];
  int sig_40 = wa[2] & wb[2];
  int sig_41 = sig_39 & sig_38;
  int sig_42 = sig_39 ^ sig_38;
  int sig_43 = sig_40 | sig_41;
  int sig_44 = wa[3] ^ wb[3];
  int sig_45 = wa[3] & wb[3];
  int sig_46 = sig_44 & sig_43;
  int sig_47 = sig_44 ^ sig_43;
  int sig_48 = sig_45 | sig_46;
  int sig_49 = wa[4] ^ wb[4];
  int sig_50 = wa[4] & wb[4];
  int sig_51 = sig_49 & sig_48;
  int sig_52 = sig_49 ^ sig_48;
  int sig_53 = sig_50 | sig_51;
  int sig_54 = wa[5] ^ wb[5];
  int sig_55 = wa[5] & wb[5];
  int sig_56 = sig_54 & sig_53;
  int sig_57 = sig_54 ^ sig_53;
  int sig_58 = sig_55 | sig_56;
  int sig_59 = wa[6] ^ wb[6];
  int sig_60 = wa[6] & wb[6];
  int sig_61 = sig_59 & sig_58;
  int sig_62 = sig_59 ^ sig_58;
  int sig_63 = sig_60 | sig_61;
  int sig_64 = wa[7] ^ wb[7];
  int sig_65 = wa[7] & wb[7];
  int sig_66 = sig_64 & sig_63;
  int sig_67 = sig_64 ^ sig_63;
  int sig_68 = sig_65 | sig_66;
  int sig_69 = wa[8] ^ wb[8];
  int sig_70 = wa[8] & wb[8];
  int sig_71 = sig_69 & sig_68;
  int sig_72 = sig_69 ^ sig_68;
  int sig_73 = sig_70 | sig_71;
  int sig_74 = wa[9] ^ wb[9];
  int sig_75 = wa[9] & wb[9];
  int sig_76 = sig_74 & sig_73;
  int sig_77 = sig_74 ^ sig_73;
  int sig_78 = sig_75 | sig_76;
  int sig_79 = wa[10] ^ wb[10];
  int sig_80 = wa[10] & wb[10];
  int sig_81 = sig_79 & sig_78;
  int sig_82 = sig_79 ^ sig_78;
  int sig_83 = sig_80 | sig_81;
  int sig_84 = wa[11] ^ wb[11];
  int sig_85 = wa[11] & wb[11];
  int sig_86 = sig_84 & sig_83;
  int sig_87 = sig_84 ^ sig_83;
  int sig_88 = sig_85 | sig_86;
  int sig_89 = wa[12] ^ wb[12];
  int sig_90 = wa[12] & wb[12];
  int sig_91 = sig_89 & sig_88;
  int sig_92 = sig_89 ^ sig_88;
  int sig_93 = sig_90 | sig_91;
  int sig_94 = wa[13] ^ wb[13];
  int sig_95 = wa[13] & wb[13];
  int sig_96 = sig_94 & sig_93;
  int sig_97 = sig_94 ^ sig_93;
  int sig_98 = sig_95 | sig_96;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (sig_32 & 0x01) << 0; // default output
  y |=  (sig_37 & 0x01) << 1; // default output
  y |=  (sig_42 & 0x01) << 2; // default output
  y |=  (sig_47 & 0x01) << 3; // default output
  y |=  (sig_52 & 0x01) << 4; // default output
  y |=  (sig_57 & 0x01) << 5; // default output
  y |=  (sig_62 & 0x01) << 6; // default output
  y |=  (sig_67 & 0x01) << 7; // default output
  y |=  (sig_72 & 0x01) << 8; // default output
  y |=  (sig_77 & 0x01) << 9; // default output
  y |=  (sig_82 & 0x01) << 10; // default output
  y |=  (sig_87 & 0x01) << 11; // default output
  y |=  (sig_92 & 0x01) << 12; // default output
  y |=  (sig_97 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

uint64_t add16u_0EM(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_32 = ~((int)0);
  int sig_37 = ~((int)0);
  int sig_41 = wb[3] ^ wa[3];
  int sig_45 = wa[3] & wb[3];
  int sig_48 = sig_45;
  int sig_49 = wa[4] ^ wb[4];
  int sig_50 = wa[4] & wb[4];
  int sig_51 = sig_49 & sig_48;
  int sig_52 = sig_49 ^ sig_48;
  int sig_53 = sig_50 | sig_51;
  int sig_54 = wa[5] ^ wb[5];
  int sig_55 = wa[5] & wb[5];
  int sig_56 = sig_54 & sig_53;
  int sig_57 = sig_54 ^ sig_53;
  int sig_58 = sig_55 | sig_56;
  int sig_59 = wa[6] ^ wb[6];
  int sig_60 = wa[6] & wb[6];
  int sig_61 = sig_59 & sig_58;
  int sig_62 = sig_59 ^ sig_58;
  int sig_63 = sig_60 | sig_61;
  int sig_64 = wa[7] ^ wb[7];
  int sig_65 = wa[7] & wb[7];
  int sig_66 = sig_64 & sig_63;
  int sig_67 = sig_64 ^ sig_63;
  int sig_68 = sig_65 | sig_66;
  int sig_69 = wa[8] ^ wb[8];
  int sig_70 = wa[8] & wb[8];
  int sig_71 = sig_69 & sig_68;
  int sig_72 = sig_69 ^ sig_68;
  int sig_73 = sig_70 | sig_71;
  int sig_74 = wa[9] ^ wb[9];
  int sig_75 = wa[9] & wb[9];
  int sig_76 = sig_74 & sig_73;
  int sig_77 = sig_74 ^ sig_73;
  int sig_78 = sig_75 | sig_76;
  int sig_79 = wa[10] ^ wb[10];
  int sig_80 = wa[10] & wb[10];
  int sig_81 = sig_79 & sig_78;
  int sig_82 = sig_79 ^ sig_78;
  int sig_83 = sig_80 | sig_81;
  int sig_84 = wa[11] ^ wb[11];
  int sig_85 = wa[11] & wb[11];
  int sig_86 = sig_84 & sig_83;
  int sig_87 = sig_84 ^ sig_83;
  int sig_88 = sig_85 | sig_86;
  int sig_89 = wa[12] ^ wb[12];
  int sig_90 = wa[12] & wb[12];
  int sig_91 = sig_89 & sig_88;
  int sig_92 = sig_89 ^ sig_88;
  int sig_93 = sig_90 | sig_91;
  int sig_94 = wa[13] ^ wb[13];
  int sig_95 = wa[13] & wb[13];
  int sig_96 = sig_94 & sig_93;
  int sig_97 = sig_94 ^ sig_93;
  int sig_98 = sig_95 | sig_96;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (sig_32 & 0x01) << 0; // default output
  y |=  (wa[2] & 0x01) << 1; // default output
  y |=  (sig_37 & 0x01) << 2; // default output
  y |=  (sig_41 & 0x01) << 3; // default output
  y |=  (sig_52 & 0x01) << 4; // default output
  y |=  (sig_57 & 0x01) << 5; // default output
  y |=  (sig_62 & 0x01) << 6; // default output
  y |=  (sig_67 & 0x01) << 7; // default output
  y |=  (sig_72 & 0x01) << 8; // default output
  y |=  (sig_77 & 0x01) << 9; // default output
  y |=  (sig_82 & 0x01) << 10; // default output
  y |=  (sig_87 & 0x01) << 11; // default output
  y |=  (sig_92 & 0x01) << 12; // default output
  y |=  (sig_97 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}




uint64_t add16u_0Q7(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_50 = wa[4] | wb[4];
  int sig_52 = 0;
  int sig_53 = sig_50;
  int sig_54 = wa[5] ^ wb[5];
  int sig_55 = wa[5] & wb[5];
  int sig_56 = sig_54 & sig_53;
  int sig_57 = sig_54 ^ sig_53;
  int sig_58 = sig_55 | sig_56;
  int sig_59 = wa[6] ^ wb[6];
  int sig_60 = wa[6] & wb[6];
  int sig_61 = sig_59 & sig_58;
  int sig_62 = sig_59 ^ sig_58;
  int sig_63 = sig_60 | sig_61;
  int sig_64 = wa[7] ^ wb[7];
  int sig_65 = wa[7] & wb[7];
  int sig_66 = sig_64 & sig_63;
  int sig_67 = sig_64 ^ sig_63;
  int sig_68 = sig_65 | sig_66;
  int sig_69 = wa[8] ^ wb[8];
  int sig_70 = wa[8] & wb[8];
  int sig_71 = sig_69 & sig_68;
  int sig_72 = sig_69 ^ sig_68;
  int sig_73 = sig_70 | sig_71;
  int sig_74 = wa[9] ^ wb[9];
  int sig_75 = wa[9] & wb[9];
  int sig_76 = sig_74 & sig_73;
  int sig_77 = sig_74 ^ sig_73;
  int sig_78 = sig_75 | sig_76;
  int sig_79 = wa[10] ^ wb[10];
  int sig_80 = wa[10] & wb[10];
  int sig_81 = sig_79 & sig_78;
  int sig_82 = sig_79 ^ sig_78;
  int sig_83 = sig_80 | sig_81;
  int sig_84 = wa[11] ^ wb[11];
  int sig_85 = wa[11] & wb[11];
  int sig_86 = sig_84 & sig_83;
  int sig_87 = sig_84 ^ sig_83;
  int sig_88 = sig_85 | sig_86;
  int sig_89 = wa[12] ^ wb[12];
  int sig_90 = wa[12] & wb[12];
  int sig_91 = sig_89 & sig_88;
  int sig_92 = sig_89 ^ sig_88;
  int sig_93 = sig_90 | sig_91;
  int sig_94 = wa[13] ^ wb[13];
  int sig_95 = wa[13] & wb[13];
  int sig_96 = sig_94 & sig_93;
  int sig_97 = sig_94 ^ sig_93;
  int sig_98 = sig_95 | sig_96;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (wb[14] & 0x01) << 0; // default output
  y |=  (sig_83 & 0x01) << 1; // default output
  y |=  (sig_72 & 0x01) << 2; // default output
  y |=  (wa[3] & 0x01) << 3; // default output
  y |=  (sig_52 & 0x01) << 4; // default output
  y |=  (sig_57 & 0x01) << 5; // default output
  y |=  (sig_62 & 0x01) << 6; // default output
  y |=  (sig_67 & 0x01) << 7; // default output
  y |=  (sig_72 & 0x01) << 8; // default output
  y |=  (sig_77 & 0x01) << 9; // default output
  y |=  (sig_82 & 0x01) << 10; // default output
  y |=  (sig_87 & 0x01) << 11; // default output
  y |=  (sig_92 & 0x01) << 12; // default output
  y |=  (sig_97 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

/***
* This code is a part of EvoApproxLib library (ehw.fit.vutbr.cz/approxlib) distributed under The MIT License.
* When used, please cite the following article(s):  
* This file contains a circuit from a sub-set of pareto optimal circuits with respect to the pwr and mae parameters
***/
// MAE% = 0.021 %
// MAE = 27 
// WCE% = 0.079 %
// WCE = 103 
// WCRE% = 6300.00 %
// EP% = 98.74 %
// MRE% = 0.056 %
// MSE = 1136 
// PDK45_PWR = 0.043 mW
// PDK45_AREA = 90.6 um2
// PDK45_DELAY = 0.80 ns
#include <stdint.h>
#include <stdlib.h>

uint64_t add16u_073(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_35 = 0;
  int sig_57 = wa[11];
  int sig_59 = wa[6] ^ wb[6];
  int sig_60 = wa[6] | wb[6];
  int sig_62 = !sig_59;
  int sig_63 = sig_60;
  int sig_64 = wa[7] ^ wb[7];
  int sig_65 = wa[7] & wb[7];
  int sig_66 = sig_64 & sig_63;
  int sig_67 = sig_64 ^ sig_63;
  int sig_68 = sig_65 | sig_66;
  int sig_69 = wa[8] ^ wb[8];
  int sig_70 = wa[8] & wb[8];
  int sig_71 = sig_69 & sig_68;
  int sig_72 = sig_69 ^ sig_68;
  int sig_73 = sig_70 | sig_71;
  int sig_74 = wa[9] ^ wb[9];
  int sig_75 = wa[9] & wb[9];
  int sig_76 = sig_74 & sig_73;
  int sig_77 = sig_74 ^ sig_73;
  int sig_78 = sig_75 | sig_76;
  int sig_79 = wa[10] ^ wb[10];
  int sig_80 = wa[10] & wb[10];
  int sig_81 = sig_79 & sig_78;
  int sig_82 = sig_79 ^ sig_78;
  int sig_83 = sig_80 | sig_81;
  int sig_84 = wa[11] ^ wb[11];
  int sig_85 = wa[11] & wb[11];
  int sig_86 = sig_84 & sig_83;
  int sig_87 = sig_84 ^ sig_83;
  int sig_88 = sig_85 | sig_86;
  int sig_89 = wa[12] ^ wb[12];
  int sig_90 = wa[12] & wb[12];
  int sig_91 = sig_89 & sig_88;
  int sig_92 = sig_89 ^ sig_88;
  int sig_93 = sig_90 | sig_91;
  int sig_94 = wa[13] ^ wb[13];
  int sig_95 = wa[13] & wb[13];
  int sig_96 = sig_94 & sig_93;
  int sig_97 = sig_94 ^ sig_93;
  int sig_98 = sig_95 | sig_96;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (sig_102 & 0x01) << 0; // default output
  y |=  (wb[9] & 0x01) << 1; // default output
  y |=  (sig_57 & 0x01) << 2; // default output
  y |=  (wa[4] & 0x01) << 3; // default output
  y |=  (sig_35 & 0x01) << 4; // default output
  y |=  (sig_85 & 0x01) << 5; // default output
  y |=  (sig_62 & 0x01) << 6; // default output
  y |=  (sig_67 & 0x01) << 7; // default output
  y |=  (sig_72 & 0x01) << 8; // default output
  y |=  (sig_77 & 0x01) << 9; // default output
  y |=  (sig_82 & 0x01) << 10; // default output
  y |=  (sig_87 & 0x01) << 11; // default output
  y |=  (sig_92 & 0x01) << 12; // default output
  y |=  (sig_97 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

/***
* This code is a part of EvoApproxLib library (ehw.fit.vutbr.cz/approxlib) distributed under The MIT License.
* When used, please cite the following article(s):  
* This file contains a circuit from a sub-set of pareto optimal circuits with respect to the pwr and mae parameters
***/
// MAE% = 0.057 %
// MAE = 75 
// WCE% = 0.19 %
// WCE = 251 
// WCRE% = 6300.00 %
// EP% = 99.61 %
// MRE% = 0.16 %
// MSE = 8209 
// PDK45_PWR = 0.036 mW
// PDK45_AREA = 71.3 um2
// PDK45_DELAY = 0.68 ns
#include <stdint.h>
#include <stdlib.h>

uint64_t add16u_0M0(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_40 = ~((int)0);
  int sig_69 = wa[8] ^ wb[8];
  int sig_70 = wa[8] & wb[8];
  int sig_71 = sig_69 & wb[7];
  int sig_72 = sig_69 ^ wb[7];
  int sig_73 = sig_70 | sig_71;
  int sig_74 = wa[9] ^ wb[9];
  int sig_75 = wa[9] & wb[9];
  int sig_76 = sig_74 & sig_73;
  int sig_77 = sig_74 ^ sig_73;
  int sig_78 = sig_75 | sig_76;
  int sig_79 = wa[10] ^ wb[10];
  int sig_80 = wa[10] & wb[10];
  int sig_81 = sig_79 & sig_78;
  int sig_82 = sig_79 ^ sig_78;
  int sig_83 = sig_80 | sig_81;
  int sig_84 = wa[11] ^ wb[11];
  int sig_85 = wa[11] & wb[11];
  int sig_86 = sig_84 & sig_83;
  int sig_87 = sig_84 ^ sig_83;
  int sig_88 = sig_85 | sig_86;
  int sig_89 = wa[12] ^ wb[12];
  int sig_90 = wa[12] & wb[12];
  int sig_91 = sig_89 & sig_88;
  int sig_92 = sig_89 ^ sig_88;
  int sig_93 = sig_90 | sig_91;
  int sig_94 = wa[13] ^ wb[13];
  int sig_95 = wa[13] & wb[13];
  int sig_96 = sig_94 & sig_93;
  int sig_97 = sig_94 ^ sig_93;
  int sig_98 = sig_95 | sig_96;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (wb[11] & 0x01) << 0; // default output
  y |=  (wa[13] & 0x01) << 1; // default output
  y |=  (sig_88 & 0x01) << 2; // default output
  y |=  (wb[13] & 0x01) << 3; // default output
  y |=  (wb[2] & 0x01) << 4; // default output
  y |=  (sig_107 & 0x01) << 5; // default output
  y |=  (sig_40 & 0x01) << 6; // default output
  y |=  (wa[7] & 0x01) << 7; // default output
  y |=  (sig_72 & 0x01) << 8; // default output
  y |=  (sig_77 & 0x01) << 9; // default output
  y |=  (sig_82 & 0x01) << 10; // default output
  y |=  (sig_87 & 0x01) << 11; // default output
  y |=  (sig_92 & 0x01) << 12; // default output
  y |=  (sig_97 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

/***
* This code is a part of EvoApproxLib library (ehw.fit.vutbr.cz/approxlib) distributed under The MIT License.
* When used, please cite the following article(s):  
* This file contains a circuit from a sub-set of pareto optimal circuits with respect to the pwr and mae parameters
***/
// MAE% = 0.20 %
// MAE = 262 
// WCE% = 0.52 %
// WCE = 684 
// WCRE% = 6300.00 %
// EP% = 99.91 %
// MRE% = 0.55 %
// MSE = 92039 
// PDK45_PWR = 0.026 mW
// PDK45_AREA = 55.8 um2
// PDK45_DELAY = 0.51 ns
#include <stdint.h>
#include <stdlib.h>

uint64_t add16u_0DL(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_35 = wb[10];
  int sig_65 = 0;
  int sig_79 = wa[10] ^ wb[10];
  int sig_80 = wa[10] & wb[10];
  int sig_81 = sig_79 & wb[9];
  int sig_82 = sig_79 ^ wb[9];
  int sig_83 = sig_80 | sig_81;
  int sig_84 = wa[11] ^ wb[11];
  int sig_85 = wa[11] & wb[11];
  int sig_86 = sig_84 & sig_83;
  int sig_87 = sig_84 ^ sig_83;
  int sig_88 = sig_85 | sig_86;
  int sig_89 = wa[12] ^ wb[12];
  int sig_90 = wa[12] & wb[12];
  int sig_91 = sig_89 & sig_88;
  int sig_92 = sig_89 ^ sig_88;
  int sig_93 = sig_90 | sig_91;
  int sig_94 = wa[13] ^ wb[13];
  int sig_95 = wa[13] & wb[13];
  int sig_96 = sig_94 & sig_93;
  int sig_97 = sig_94 ^ sig_93;
  int sig_98 = sig_95 | sig_96;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (sig_35 & 0x01) << 0; // default output
  y |=  (sig_65 & 0x01) << 1; // default output
  y |=  (wb[14] & 0x01) << 2; // default output
  y |=  (sig_92 & 0x01) << 3; // default output
  y |=  (wa[8] & 0x01) << 4; // default output
  y |=  (sig_95 & 0x01) << 5; // default output
  y |=  (wa[7] & 0x01) << 6; // default output
  y |=  (wb[1] & 0x01) << 7; // default output
  y |=  (wb[8] & 0x01) << 8; // default output
  y |=  (wa[9] & 0x01) << 9; // default output
  y |=  (sig_82 & 0x01) << 10; // default output
  y |=  (sig_87 & 0x01) << 11; // default output
  y |=  (sig_92 & 0x01) << 12; // default output
  y |=  (sig_97 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

/***
* This code is a part of EvoApproxLib library (ehw.fit.vutbr.cz/approxlib) distributed under The MIT License.
* When used, please cite the following article(s):  
* This file contains a circuit from a sub-set of pareto optimal circuits with respect to the pwr and mae parameters
***/
// MAE% = 0.91 %
// MAE = 1187 
// WCE% = 2.90 %
// WCE = 3803 
// WCRE% = 1503.12 %
// EP% = 99.98 %
// MRE% = 2.49 %
// MSE = 20515.545e2 
// PDK45_PWR = 0.018 mW
// PDK45_AREA = 45.5 um2
// PDK45_DELAY = 0.35 ns
#include <stdint.h>
#include <stdlib.h>

uint64_t add16u_0GK(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_89 = wa[12] ^ wb[12];
  int sig_90 = wa[12] & wb[12];
  int sig_91 = sig_89 & wb[11];
  int sig_92 = sig_89 ^ wb[11];
  int sig_93 = sig_90 | sig_91;
  int sig_94 = wa[13] ^ wb[13];
  int sig_95 = wa[13] & wb[13];
  int sig_96 = sig_94 & sig_93;
  int sig_97 = sig_94 ^ sig_93;
  int sig_98 = sig_95 | sig_96;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (wb[13] & 0x01) << 0; // default output
  y |=  (wa[6] & 0x01) << 1; // default output
  y |=  (wb[7] & 0x01) << 2; // default output
  y |=  (sig_106 & 0x01) << 3; // default output
  y |=  (wb[12] & 0x01) << 4; // default output
  y |=  (wa[10] & 0x01) << 5; // default output
  y |=  (sig_104 & 0x01) << 6; // default output
  y |=  (wb[9] & 0x01) << 7; // default output
  y |=  (wb[6] & 0x01) << 8; // default output
  y |=  (sig_106 & 0x01) << 9; // default output
  y |=  (wa[6] & 0x01) << 10; // default output
  y |=  (wa[11] & 0x01) << 11; // default output
  y |=  (sig_92 & 0x01) << 12; // default output
  y |=  (sig_97 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

/***
* This code is a part of EvoApproxLib library (ehw.fit.vutbr.cz/approxlib) distributed under The MIT License.
* When used, please cite the following article(s):  
* This file contains a circuit from a sub-set of pareto optimal circuits with respect to the pwr and mae parameters
***/
// MAE% = 3.52 %
// MAE = 4619 
// WCE% = 9.74 %
// WCE = 12763 
// WCRE% = 700.20 %
// EP% = 99.99 %
// MRE% = 9.54 %
// MSE = 30582.328e3 
// PDK45_PWR = 0.011 mW
// PDK45_AREA = 31.9 um2
// PDK45_DELAY = 0.27 ns
#include <stdint.h>
#include <stdlib.h>

uint64_t add16u_02E(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_54 = wa[3];
  int sig_74 = 0;
  int sig_95 = wa[13] | wb[13];
  int sig_98 = sig_95;
  int sig_99 = wa[14] ^ wb[14];
  int sig_100 = wa[14] & wb[14];
  int sig_101 = sig_99 & sig_98;
  int sig_102 = sig_99 ^ sig_98;
  int sig_103 = sig_100 | sig_101;
  int sig_104 = wa[15] ^ wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (wa[9] & 0x01) << 0; // default output
  y |=  (sig_107 & 0x01) << 1; // default output
  y |=  (wa[7] & 0x01) << 2; // default output
  y |=  (wa[6] & 0x01) << 3; // default output
  y |=  (wb[12] & 0x01) << 4; // default output
  y |=  (sig_98 & 0x01) << 5; // default output
  y |=  (sig_54 & 0x01) << 6; // default output
  y |=  (sig_101 & 0x01) << 7; // default output
  y |=  (sig_98 & 0x01) << 8; // default output
  y |=  (wa[15] & 0x01) << 9; // default output
  y |=  (wb[10] & 0x01) << 10; // default output
  y |=  (wb[11] & 0x01) << 11; // default output
  y |=  (wa[9] & 0x01) << 12; // default output
  y |=  (sig_74 & 0x01) << 13; // default output
  y |=  (sig_102 & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

/***
* This code is a part of EvoApproxLib library (ehw.fit.vutbr.cz/approxlib) distributed under The MIT License.
* When used, please cite the following article(s):  
* This file contains a circuit from a sub-set of pareto optimal circuits with respect to the pwr and mae parameters
***/
// MAE% = 9.90 %
// MAE = 12976 
// WCE% = 34.18 %
// WCE = 44805 
// WCRE% = 6300.00 %
// EP% = 100.00 %
// MRE% = 22.35 %
// MSE = 25358.103e4 
// PDK45_PWR = 0.0041 mW
// PDK45_AREA = 18.3 um2
// PDK45_DELAY = 0.20 ns
#include <stdint.h>
#include <stdlib.h>

uint64_t add16u_0MH(uint64_t a, uint64_t b) {
  int wa[16];
  int wb[16];
  uint64_t y = 0;
  wa[0] = (a >> 0) & 0x01;
  wb[0] = (b >> 0) & 0x01;
  wa[1] = (a >> 1) & 0x01;
  wb[1] = (b >> 1) & 0x01;
  wa[2] = (a >> 2) & 0x01;
  wb[2] = (b >> 2) & 0x01;
  wa[3] = (a >> 3) & 0x01;
  wb[3] = (b >> 3) & 0x01;
  wa[4] = (a >> 4) & 0x01;
  wb[4] = (b >> 4) & 0x01;
  wa[5] = (a >> 5) & 0x01;
  wb[5] = (b >> 5) & 0x01;
  wa[6] = (a >> 6) & 0x01;
  wb[6] = (b >> 6) & 0x01;
  wa[7] = (a >> 7) & 0x01;
  wb[7] = (b >> 7) & 0x01;
  wa[8] = (a >> 8) & 0x01;
  wb[8] = (b >> 8) & 0x01;
  wa[9] = (a >> 9) & 0x01;
  wb[9] = (b >> 9) & 0x01;
  wa[10] = (a >> 10) & 0x01;
  wb[10] = (b >> 10) & 0x01;
  wa[11] = (a >> 11) & 0x01;
  wb[11] = (b >> 11) & 0x01;
  wa[12] = (a >> 12) & 0x01;
  wb[12] = (b >> 12) & 0x01;
  wa[13] = (a >> 13) & 0x01;
  wb[13] = (b >> 13) & 0x01;
  wa[14] = (a >> 14) & 0x01;
  wb[14] = (b >> 14) & 0x01;
  wa[15] = (a >> 15) & 0x01;
  wb[15] = (b >> 15) & 0x01;
  int sig_52 = wb[11];
  int sig_72 = 0;
  int sig_92 = ~((int)0);
  int sig_93 = 0;
  int sig_103 = wa[14];
  int sig_104 = wa[15] | wb[15];
  int sig_105 = wa[15] & wb[15];
  int sig_106 = sig_104 & sig_103;
  int sig_107 = sig_104 ^ sig_103;
  int sig_108 = sig_105 | sig_106;
  y |=  (wb[2] & 0x01) << 0; // default output
  y |=  (sig_93 & 0x01) << 1; // default output
  y |=  (wa[8] & 0x01) << 2; // default output
  y |=  (wa[7] & 0x01) << 3; // default output
  y |=  (sig_52 & 0x01) << 4; // default output
  y |=  (sig_106 & 0x01) << 5; // default output
  y |=  (sig_92 & 0x01) << 6; // default output
  y |=  (sig_52 & 0x01) << 7; // default output
  y |=  (wb[11] & 0x01) << 8; // default output
  y |=  (wb[13] & 0x01) << 9; // default output
  y |=  (wa[8] & 0x01) << 10; // default output
  y |=  (wb[13] & 0x01) << 11; // default output
  y |=  (sig_72 & 0x01) << 12; // default output
  y |=  (wb[10] & 0x01) << 13; // default output
  y |=  (wb[14] & 0x01) << 14; // default output
  y |=  (sig_107 & 0x01) << 15; // default output
  y |=  (sig_108 & 0x01) << 16; // default output
   return y;
}

uint64_t mul16u_exact(uint64_t a, uint64_t b){
  return a*b;
}

uint32_t mul16u_BMC (uint16_t a, uint16_t b) {
    static uint16_t * cacheLL = NULL;
    static uint16_t * cacheLH = NULL;
    static uint16_t * cacheHL = NULL;
    static uint16_t * cacheHH = NULL;
    int fillData = cacheLL == NULL || cacheLH == NULL || cacheHL == NULL || cacheHH == NULL;

    if(!cacheLL) cacheLL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheLH) cacheLH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHL) cacheHL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHH) cacheHH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    
    if(fillData) {
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                cacheLL[i * 256 + j] = mul8_364(i, j);
                cacheLH[i * 256 + j] = mul8_364(i, j);
                cacheHL[i * 256 + j] = trun8_tam00b(i, j);
                cacheHH[i * 256 + j] = mul8_364(i, j);
            }
        }
    }

    uint32_t opt = 0;

    opt += (uint32_t)cacheLL[(a & 0xFF       ) * 256 + (b & 0xFF             )];
    opt += (uint32_t)cacheLH[(a & 0xFF       ) * 256 + ((b >> 8) & 0xFF      )] << 8;
    opt += (uint32_t)cacheHL[((a >> 8) & 0xFF) * 256 + (b & 0xFF             )] << 8;
    opt += (uint32_t)cacheHH[((a >> 8) & 0xFF) * 256 + ((b >> 8) & 0xFF      )] << 16;

    return opt;
}


uint64_t mult8_cgp14ep_ep55705_wc4_csamrca(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_17, dout_19, dout_20, dout_21, dout_22, dout_23, dout_26, dout_27, dout_28, dout_29, dout_30, dout_31, dout_35, dout_36, dout_37, dout_38, dout_39, dout_40, dout_41, dout_42, dout_43, dout_44, dout_45, dout_46, dout_47, dout_48, dout_49, dout_50, dout_51, dout_52, dout_53, dout_56, dout_57, dout_59, dout_60, dout_61, dout_62, dout_63, dout_64, dout_65, dout_66, dout_67, dout_68, dout_69, dout_70, dout_71, dout_72, dout_73, dout_74, dout_75, dout_76, dout_77, dout_78, dout_79, dout_80, dout_81, dout_82, dout_83, dout_84, dout_85, dout_86, dout_87, dout_88, dout_89, dout_90, dout_91, dout_92, dout_93, dout_94, dout_95, dout_96, dout_97, dout_98, dout_99, dout_100, dout_101, dout_102, dout_103, dout_104, dout_105, dout_106, dout_107, dout_108, dout_109, dout_110, dout_111, dout_112, dout_113, dout_114, dout_115, dout_116, dout_117, dout_118, dout_119, dout_120, dout_121, dout_122, dout_123, dout_124, dout_125, dout_126, dout_127, dout_128, dout_129, dout_130, dout_131, dout_132, dout_133, dout_134, dout_135, dout_136, dout_137, dout_138, dout_139, dout_140, dout_141, dout_142, dout_143, dout_144, dout_145, dout_146, dout_147, dout_148, dout_149, dout_150, dout_151, dout_152, dout_153, dout_154, dout_155, dout_156, dout_157, dout_158, dout_159, dout_160, dout_161, dout_162, dout_163, dout_164, dout_165, dout_166, dout_167, dout_168, dout_169, dout_170, dout_171, dout_172, dout_173, dout_174, dout_175, dout_176, dout_177, dout_178, dout_179, dout_180, dout_181, dout_182, dout_183, dout_184, dout_185, dout_186, dout_187, dout_188, dout_189, dout_190, dout_191, dout_192, dout_193, dout_194, dout_195, dout_196, dout_197, dout_198, dout_199, dout_200, dout_201, dout_202, dout_203, dout_204, dout_205, dout_206, dout_207, dout_208, dout_209, dout_210, dout_211, dout_212, dout_213, dout_214, dout_215, dout_216, dout_217, dout_218, dout_219, dout_220, dout_221, dout_222, dout_223, dout_224, dout_225, dout_226, dout_227, dout_228, dout_229, dout_230, dout_231, dout_232, dout_233, dout_234, dout_235, dout_236, dout_237, dout_238, dout_239, dout_240, dout_241, dout_242, dout_243, dout_244, dout_245, dout_246, dout_247, dout_248, dout_249, dout_250, dout_251, dout_252, dout_253, dout_254, dout_255, dout_256, dout_257, dout_258, dout_259, dout_260, dout_261, dout_262, dout_263, dout_264, dout_265, dout_266, dout_267, dout_268, dout_269, dout_270, dout_271, dout_272, dout_273, dout_274, dout_275, dout_276, dout_277, dout_278, dout_279, dout_280, dout_281, dout_282, dout_283, dout_284, dout_285, dout_286, dout_287, dout_288, dout_289, dout_290, dout_291, dout_292, dout_293, dout_294, dout_295, dout_296, dout_297, dout_298, dout_299, dout_300, dout_301, dout_302, dout_303, dout_304, dout_305, dout_306, dout_307, dout_308, dout_309, dout_310, dout_311, dout_312, dout_313, dout_314, dout_315, dout_316, dout_317, dout_318, dout_319, dout_320, dout_321, dout_322, dout_323, dout_324, dout_325, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_334, dout_335;   int avg=0;

   dout_17=((B >> 1)&1)&((A >> 1)&1);
   dout_19=((B >> 3)&1)&((A >> 0)&1);
   dout_20=((B >> 4)&1)&((A >> 0)&1);
   dout_21=((B >> 5)&1)&((A >> 0)&1);
   dout_22=((B >> 6)&1)&((A >> 0)&1);
   dout_23=((B >> 7)&1)&((A >> 0)&1);
   dout_26=((B >> 2)&1)&((A >> 1)&1);
   dout_27=((B >> 3)&1)&((A >> 1)&1);
   dout_28=((B >> 4)&1)&((A >> 1)&1);
   dout_29=((B >> 5)&1)&((A >> 1)&1);
   dout_30=((B >> 6)&1)&((A >> 1)&1);
   dout_31=((B >> 7)&1)&((A >> 1)&1);
   dout_35=((B >> 2)&1)&((A >> 0)&1);
   dout_36=dout_19^dout_26;
   dout_37=dout_19&dout_26;
   dout_38=dout_20^dout_27;
   dout_39=dout_20&dout_27;
   dout_40=dout_21^dout_28;
   dout_41=dout_21&dout_28;
   dout_42=dout_22^dout_29;
   dout_43=dout_22&dout_29;
   dout_44=dout_23^dout_30;
   dout_45=((B >> 6)&1)&dout_31;
   dout_46=((B >> 0)&1)&((A >> 2)&1);
   dout_47=((B >> 1)&1)&((A >> 2)&1);
   dout_48=((B >> 2)&1)&((A >> 2)&1);
   dout_49=((B >> 3)&1)&((A >> 2)&1);
   dout_50=((B >> 4)&1)&((A >> 2)&1);
   dout_51=((B >> 5)&1)&((A >> 2)&1);
   dout_52=((B >> 6)&1)&((A >> 2)&1);
   dout_53=((B >> 7)&1)&((A >> 2)&1);
   dout_56=dout_46&dout_17;
   dout_57=dout_46|dout_17;
   dout_59=dout_36^dout_47;
   dout_60=dout_36&dout_47;
   dout_61=dout_59&dout_35;
   dout_62=dout_59^dout_35;
   dout_63=dout_60|dout_61;
   dout_64=dout_38^dout_48;
   dout_65=dout_38&dout_48;
   dout_66=dout_64&dout_37;
   dout_67=dout_64^dout_37;
   dout_68=dout_65|dout_66;
   dout_69=dout_40^dout_49;
   dout_70=dout_40&dout_49;
   dout_71=dout_69&dout_39;
   dout_72=dout_69^dout_39;
   dout_73=dout_70|dout_71;
   dout_74=dout_42^dout_50;
   dout_75=dout_42&dout_50;
   dout_76=dout_74&dout_41;
   dout_77=dout_74^dout_41;
   dout_78=dout_75|dout_76;
   dout_79=dout_44^dout_51;
   dout_80=dout_44&dout_51;
   dout_81=dout_79&dout_43;
   dout_82=dout_79^dout_43;
   dout_83=dout_80|dout_81;
   dout_84=dout_31^dout_52;
   dout_85=dout_45&((A >> 2)&1);
   dout_86=((A >> 0)&1)&dout_45;
   dout_87=dout_84^dout_86;
   dout_88=dout_85|dout_86;
   dout_89=((B >> 0)&1)&((A >> 3)&1);
   dout_90=((B >> 1)&1)&((A >> 3)&1);
   dout_91=((B >> 2)&1)&((A >> 3)&1);
   dout_92=((B >> 3)&1)&((A >> 3)&1);
   dout_93=((B >> 4)&1)&((A >> 3)&1);
   dout_94=((B >> 5)&1)&((A >> 3)&1);
   dout_95=((B >> 6)&1)&((A >> 3)&1);
   dout_96=((B >> 7)&1)&((A >> 3)&1);
   dout_97=dout_62^dout_89;
   dout_98=dout_62&dout_89;
   dout_99=dout_97&dout_56;
   dout_100=dout_97^dout_56;
   dout_101=dout_98|dout_99;
   dout_102=dout_67^dout_90;
   dout_103=dout_67&dout_90;
   dout_104=dout_102&dout_63;
   dout_105=dout_102^dout_63;
   dout_106=dout_103|dout_104;
   dout_107=dout_72^dout_91;
   dout_108=dout_72&dout_91;
   dout_109=dout_107&dout_68;
   dout_110=dout_107^dout_68;
   dout_111=dout_108|dout_109;
   dout_112=dout_77^dout_92;
   dout_113=dout_77&dout_92;
   dout_114=dout_112&dout_73;
   dout_115=dout_112^dout_73;
   dout_116=dout_113|dout_114;
   dout_117=dout_82^dout_93;
   dout_118=dout_82&dout_93;
   dout_119=dout_117&dout_78;
   dout_120=dout_117^dout_78;
   dout_121=dout_118|dout_119;
   dout_122=dout_87^dout_94;
   dout_123=dout_87&dout_94;
   dout_124=dout_122&dout_83;
   dout_125=dout_122^dout_83;
   dout_126=dout_123|dout_124;
   dout_127=dout_53^dout_95;
   dout_128=dout_53&dout_95;
   dout_129=dout_127&dout_88;
   dout_130=dout_127^dout_88;
   dout_131=dout_128|dout_129;
   dout_132=((B >> 0)&1)&((A >> 4)&1);
   dout_133=((B >> 1)&1)&((A >> 4)&1);
   dout_134=((B >> 2)&1)&((A >> 4)&1);
   dout_135=((B >> 3)&1)&((A >> 4)&1);
   dout_136=((B >> 4)&1)&((A >> 4)&1);
   dout_137=((B >> 5)&1)&((A >> 4)&1);
   dout_138=((B >> 6)&1)&((A >> 4)&1);
   dout_139=((B >> 7)&1)&((A >> 4)&1);
   dout_140=dout_105^dout_132;
 

   dout_141=dout_105&dout_132;
   dout_142=dout_140&dout_101;
   dout_143=dout_140^dout_101;
   dout_144=dout_141|dout_142;
   dout_145=dout_110^dout_133;
   dout_146=dout_110&dout_133;
   dout_147=dout_145&dout_106;
   dout_148=dout_145^dout_106;
   dout_149=dout_146|dout_147;
   dout_150=dout_115^dout_134;
   dout_151=dout_115&dout_134;
   dout_152=dout_150&dout_111;
   dout_153=dout_150^dout_111;
   dout_154=dout_151|dout_152;
   dout_155=dout_120^dout_135;
   dout_156=dout_120&dout_135;
   dout_157=dout_155&dout_116;
   dout_158=dout_155^dout_116;
   dout_159=dout_156|dout_157;
   dout_160=dout_125^dout_136;
   dout_161=dout_125&dout_136;
   dout_162=dout_160&dout_121;
   dout_163=dout_160^dout_121;
   dout_164=dout_161|dout_162;
   dout_165=dout_130^dout_137;
   dout_166=dout_130&dout_137;
   dout_167=dout_165&dout_126;
   dout_168=dout_165^dout_126;
   dout_169=dout_166|dout_167;
   dout_170=dout_96^dout_138;
   dout_171=dout_96&dout_138;
   dout_172=dout_170&dout_131;
   dout_173=dout_170^dout_131;
   dout_174=dout_171|dout_172;
   dout_175=((B >> 0)&1)&((A >> 5)&1);
   dout_176=((B >> 1)&1)&((A >> 5)&1);
   dout_177=((B >> 2)&1)&((A >> 5)&1);
   dout_178=((B >> 3)&1)&((A >> 5)&1);
   dout_179=((B >> 4)&1)&((A >> 5)&1);
   dout_180=((B >> 5)&1)&((A >> 5)&1);
   dout_181=((B >> 6)&1)&((A >> 5)&1);
   dout_182=((B >> 7)&1)&((A >> 5)&1);
   dout_183=dout_148^dout_175;
   dout_184=dout_148&dout_175;
   dout_185=dout_183&dout_144;
   dout_186=dout_183^dout_144;
   dout_187=dout_184|dout_185;
   dout_188=dout_153^dout_176;
   dout_189=dout_153&dout_176;
   dout_190=dout_188&dout_149;
   dout_191=dout_188^dout_149;
   dout_192=dout_189|dout_190;
   dout_193=dout_158^dout_177;
   dout_194=dout_158&dout_177;
   dout_195=dout_193&dout_154;
   dout_196=dout_193^dout_154;
   dout_197=dout_194|dout_195;
   dout_198=dout_163^dout_178;
   dout_199=dout_163&dout_178;
   dout_200=dout_198&dout_159;
   dout_201=dout_198^dout_159;
   dout_202=dout_199|dout_200;
   dout_203=dout_168^dout_179;
   dout_204=dout_168&dout_179;
   dout_205=dout_203&dout_164;
   dout_206=dout_203^dout_164;
   dout_207=dout_204|dout_205;
   dout_208=dout_173^dout_180;
   dout_209=dout_173&dout_180;
   dout_210=dout_208&dout_169;
   dout_211=dout_208^dout_169;
   dout_212=dout_209|dout_210;
   dout_213=dout_139^dout_181;
   dout_214=dout_139&dout_181;
   dout_215=dout_213&dout_174;
   dout_216=dout_213^dout_174;
   dout_217=dout_214|dout_215;
   dout_218=((B >> 0)&1)&((A >> 6)&1);
   dout_219=((B >> 1)&1)&((A >> 6)&1);
   dout_220=((B >> 2)&1)&((A >> 6)&1);
   dout_221=((B >> 3)&1)&((A >> 6)&1);
   dout_222=((B >> 4)&1)&((A >> 6)&1);
   dout_223=((B >> 5)&1)&((A >> 6)&1);
   dout_224=((B >> 6)&1)&((A >> 6)&1);
   dout_225=((B >> 7)&1)&((A >> 6)&1);
   dout_226=dout_191^dout_218;
   dout_227=dout_191&dout_218;
   dout_228=dout_226&dout_187;
   dout_229=dout_226^dout_187;
   dout_230=dout_227|dout_228;
   dout_231=dout_196^dout_219;
   dout_232=dout_196&dout_219;
   dout_233=dout_231&dout_192;
   dout_234=dout_231^dout_192;
   dout_235=dout_232|dout_233;
   dout_236=dout_201^dout_220;
   dout_237=dout_201&dout_220;
   dout_238=dout_236&dout_197;
   dout_239=dout_236^dout_197;
   dout_240=dout_237|dout_238;
   dout_241=dout_206^dout_221;
   dout_242=dout_206&dout_221;
   dout_243=dout_241&dout_202;
   dout_244=dout_241^dout_202;
   dout_245=dout_242|dout_243;
   dout_246=dout_211^dout_222;
   dout_247=dout_211&dout_222;
   dout_248=dout_246&dout_207;
   dout_249=dout_246^dout_207;
   dout_250=dout_247|dout_248;
   dout_251=dout_216^dout_223;
   dout_252=dout_216&dout_223;
   dout_253=dout_251&dout_212;
   dout_254=dout_251^dout_212;
   dout_255=dout_252|dout_253;
   dout_256=dout_182^dout_224;
   dout_257=dout_182&dout_224;
   dout_258=dout_256&dout_217;
   dout_259=dout_256^dout_217;
   dout_260=dout_257|dout_258;
   dout_261=((B >> 0)&1)&((A >> 7)&1);
   dout_262=((B >> 1)&1)&((A >> 7)&1);
   dout_263=((B >> 2)&1)&((A >> 7)&1);
   dout_264=((B >> 3)&1)&((A >> 7)&1);
   dout_265=((B >> 4)&1)&((A >> 7)&1);
   dout_266=((B >> 5)&1)&((A >> 7)&1);
   dout_267=((B >> 6)&1)&((A >> 7)&1);
   dout_268=((B >> 7)&1)&((A >> 7)&1);
   dout_269=dout_234^dout_261;
   dout_270=dout_234&dout_261;
   dout_271=dout_269&dout_230;
   dout_272=dout_269^dout_230;
   dout_273=dout_270|dout_271;
   dout_274=dout_239^dout_262;
   dout_275=dout_239&dout_262;
   dout_276=dout_274&dout_235;
   dout_277=dout_274^dout_235;
   dout_278=dout_275|dout_276;
   dout_279=dout_244^dout_263;
   dout_280=dout_244&dout_263;
   dout_281=dout_279&dout_240;
   dout_282=dout_279^dout_240;
   dout_283=dout_280|dout_281;
   dout_284=dout_249^dout_264;
   dout_285=dout_249&dout_264;
   dout_286=dout_284&dout_245;
   dout_287=dout_284^dout_245;
   dout_288=dout_285|dout_286;
   dout_289=dout_254^dout_265;
   dout_290=dout_254&dout_265;
   dout_291=dout_289&dout_250;
   dout_292=dout_289^dout_250;
   dout_293=dout_290|dout_291;
   dout_294=dout_259^dout_266;
   dout_295=dout_259&dout_266;
   dout_296=dout_294&dout_255;
   dout_297=dout_294^dout_255;
   dout_298=dout_295|dout_296;
   dout_299=dout_225^dout_267;
   dout_300=dout_225&dout_267;
   dout_301=dout_299&dout_260;
   dout_302=dout_299^dout_260;
   dout_303=dout_300|dout_301;
   dout_304=dout_277^dout_273;
   dout_305=dout_277&dout_273;
   dout_306=dout_282^dout_278;
   dout_307=dout_282&dout_278;
   dout_308=dout_306&dout_305;
   dout_309=dout_306^dout_305;
   dout_310=dout_307|dout_308;
   dout_311=dout_287^dout_283;
   dout_312=dout_287&dout_283;
   dout_313=dout_311&dout_310;
   dout_314=dout_311^dout_310;
   dout_315=dout_312|dout_313;
   dout_316=dout_292^dout_288;
   dout_317=dout_292&dout_288;
   dout_318=dout_316&dout_315;
   dout_319=dout_316^dout_315;
   dout_320=dout_317|dout_318;
   dout_321=dout_297^dout_293;
   dout_322=dout_297&dout_293;
   dout_323=dout_321&dout_320;
   dout_324=dout_321^dout_320;
   dout_325=dout_322|dout_323;
   dout_326=dout_302^dout_298;
   dout_327=dout_302&dout_298;
   dout_328=dout_326&dout_325;
   dout_329=dout_326^dout_325;
   dout_330=dout_327|dout_328;
   dout_331=dout_268^dout_303;
   dout_332=((A >> 7)&1)&dout_303;
   dout_333=dout_331&dout_330;
   dout_334=dout_331^dout_330;
   dout_335=dout_332|dout_333;

   O = 0;
   O |= (dout_57&1) << 0;
   O |= (0&1) << 1;
   O |= (dout_57&1) << 2;
   O |= (dout_100&1) << 3;
   O |= (dout_143&1) << 4;
   O |= (dout_186&1) << 5;
   O |= (dout_229&1) << 6;
   O |= (dout_272&1) << 7;
   O |= (dout_304&1) << 8;
   O |= (dout_309&1) << 9;
   O |= (dout_314&1) << 10;
   O |= (dout_319&1) << 11;
   O |= (dout_324&1) << 12;
   O |= (dout_329&1) << 13;
   O |= (dout_334&1) << 14;
   O |= (dout_335&1) << 15;
   return O;
}

uint32_t mul16u_AQ1 (uint16_t a, uint16_t b) {
    static uint16_t * cacheLL = NULL;
    static uint16_t * cacheLH = NULL;
    static uint16_t * cacheHL = NULL;
    static uint16_t * cacheHH = NULL;
    int fillData = cacheLL == NULL || cacheLH == NULL || cacheHL == NULL || cacheHH == NULL;

    if(!cacheLL) cacheLL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheLH) cacheLH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHL) cacheHL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHH) cacheHH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    
    if(fillData) {
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                cacheLL[i * 256 + j] = mult8_cgp14ep_ep55705_wc4_csamrca(j, i);
                cacheLH[i * 256 + j] = mul8_364(i, j);
                cacheHL[i * 256 + j] = mul8_364(i, j);
                cacheHH[i * 256 + j] = mul8_364(i, j);
            }
        }
    }

    uint32_t opt = 0;

    opt += (uint32_t)cacheLL[(a & 0xFF       ) * 256 + (b & 0xFF             )];
    opt += (uint32_t)cacheLH[(a & 0xFF       ) * 256 + ((b >> 8) & 0xFF      )] << 8;
    opt += (uint32_t)cacheHL[((a >> 8) & 0xFF) * 256 + (b & 0xFF             )] << 8;
    opt += (uint32_t)cacheHH[((a >> 8) & 0xFF) * 256 + ((b >> 8) & 0xFF      )] << 16;

    return opt;
}



uint64_t mult8_cgp14_wc79_csamrca(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_20, dout_21, dout_22, dout_23, dout_29, dout_31, dout_40, dout_41, dout_42, dout_43, dout_44, dout_47, dout_49, dout_50, dout_51, dout_52, dout_53, dout_56, dout_70, dout_74, dout_75, dout_76, dout_77, dout_78, dout_79, dout_80, dout_81, dout_82, dout_83, dout_84, dout_86, dout_87, dout_88, dout_92, dout_93, dout_94, dout_95, dout_96, dout_106, dout_112, dout_113, dout_114, dout_115, dout_116, dout_117, dout_118, dout_119, dout_120, dout_121, dout_122, dout_123, dout_124, dout_125, dout_126, dout_127, dout_128, dout_129, dout_130, dout_131, dout_133, dout_134, dout_135, dout_136, dout_137, dout_138, dout_139, dout_150, dout_151, dout_155, dout_156, dout_157, dout_158, dout_159, dout_160, dout_161, dout_162, dout_163, dout_164, dout_165, dout_166, dout_167, dout_168, dout_169, dout_170, dout_171, dout_172, dout_173, dout_174, dout_176, dout_177, dout_178, dout_179, dout_180, dout_181, dout_182, dout_186, dout_188, dout_189, dout_190, dout_191, dout_192, dout_193, dout_194, dout_195, dout_196, dout_197, dout_198, dout_199, dout_200, dout_201, dout_202, dout_203, dout_204, dout_205, dout_206, dout_207, dout_208, dout_209, dout_210, dout_211, dout_212, dout_213, dout_214, dout_215, dout_216, dout_217, dout_218, dout_219, dout_220, dout_221, dout_222, dout_223, dout_224, dout_225, dout_226, dout_227, dout_228, dout_229, dout_230, dout_231, dout_232, dout_233, dout_234, dout_235, dout_236, dout_237, dout_238, dout_239, dout_240, dout_241, dout_242, dout_243, dout_244, dout_245, dout_246, dout_247, dout_248, dout_249, dout_250, dout_251, dout_252, dout_253, dout_254, dout_255, dout_256, dout_257, dout_258, dout_259, dout_260, dout_261, dout_262, dout_263, dout_264, dout_265, dout_266, dout_267, dout_268, dout_269, dout_270, dout_271, dout_272, dout_273, dout_274, dout_275, dout_276, dout_277, dout_278, dout_279, dout_280, dout_281, dout_282, dout_283, dout_284, dout_285, dout_286, dout_287, dout_288, dout_289, dout_290, dout_291, dout_292, dout_293, dout_294, dout_295, dout_296, dout_297, dout_298, dout_299, dout_300, dout_301, dout_302, dout_303, dout_304, dout_305, dout_306, dout_307, dout_308, dout_309, dout_310, dout_311, dout_312, dout_313, dout_314, dout_315, dout_316, dout_317, dout_318, dout_319, dout_320, dout_321, dout_322, dout_323, dout_324, dout_325, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_334, dout_335;   int avg=0;

   dout_20=((B >> 6)&1)&((A >> 1)&1);
   dout_21=((A >> 1)&1)&((A >> 0)&1);
   dout_22=((B >> 6)&1)&((A >> 0)&1);
   dout_23=((B >> 7)&1)&((A >> 0)&1);
   dout_29=((B >> 5)&1)&((A >> 1)&1);
   dout_31=((B >> 7)&1)&((A >> 1)&1);
   dout_40=((A >> 1)&1)|((A >> 0)&1);
   dout_41=dout_21&((B >> 4)&1);
   dout_42=dout_22^dout_29;
   dout_43=dout_22&dout_29;
   dout_44=dout_23^dout_20;
   dout_47=((B >> 7)&1)&dout_21;
   dout_49=((B >> 3)&1)&((A >> 2)&1);
   dout_50=((B >> 4)&1)&((A >> 2)&1);
   dout_51=((B >> 5)&1)&((A >> 2)&1);
   dout_52=((B >> 6)&1)&((A >> 2)&1);
   dout_53=((B >> 7)&1)&((A >> 2)&1);
   dout_56=dout_52&dout_31;
   dout_70=dout_40&dout_49;
   dout_74=dout_42^dout_50;
   dout_75=dout_42&dout_50;
   dout_76=dout_74&dout_41;
   dout_77=dout_74^dout_41;
   dout_78=dout_75|dout_76;
   dout_79=dout_44^dout_51;
   dout_80=dout_44&dout_51;
   dout_81=dout_79&dout_43;
   dout_82=dout_79^dout_43;
   dout_83=dout_80|dout_81;
   dout_84=dout_31^dout_52;
   dout_86=((B >> 6)&1)&dout_47;
   dout_87=dout_84^dout_86;
   dout_88=dout_56|dout_86;
   dout_92=((B >> 3)&1)&((A >> 3)&1);
   dout_93=((B >> 4)&1)&((A >> 3)&1);
   dout_94=((B >> 5)&1)&((A >> 3)&1);
   dout_95=((B >> 6)&1)&((A >> 3)&1);
   dout_96=((B >> 7)&1)&((A >> 3)&1);
   dout_106=((B >> 2)&1)&((A >> 3)&1);
   dout_112=dout_77^dout_92;
   dout_113=dout_77&dout_92;
   dout_114=dout_112&dout_70;
   dout_115=dout_112^dout_70;
   dout_116=dout_113|dout_114;
   dout_117=dout_82^dout_93;
   dout_118=dout_82&dout_93;
   dout_119=dout_117&dout_78;
   dout_120=dout_117^dout_78;
   dout_121=dout_118|dout_119;
   dout_122=dout_87^dout_94;
   dout_123=dout_87&dout_94;
   dout_124=dout_122&dout_83;
   dout_125=dout_122^dout_83;
   dout_126=dout_123|dout_124;
   dout_127=dout_53^dout_95;
   dout_128=dout_53&dout_95;
   dout_129=dout_127&dout_88;
   dout_130=dout_127^dout_88;
   dout_131=dout_128|dout_129;
   dout_133=((B >> 1)&1)&((A >> 4)&1);
   dout_134=((B >> 2)&1)&((A >> 4)&1);
   dout_135=((B >> 3)&1)&((A >> 4)&1);
   dout_136=((B >> 4)&1)&((A >> 4)&1);
   dout_137=((B >> 5)&1)&((A >> 4)&1);
   dout_138=((B >> 6)&1)&((A >> 4)&1);
   dout_139=((B >> 7)&1)&((A >> 4)&1);
   dout_150=dout_115^dout_134;
   dout_151=dout_115&dout_134;
   dout_155=dout_120^dout_135;
   dout_156=dout_120&dout_135;
   dout_157=dout_155&dout_116;
   dout_158=dout_155^dout_116;
   dout_159=dout_156|dout_157;
   dout_160=dout_125^dout_136;
   dout_161=dout_125&dout_136;
   dout_162=dout_160&dout_121;
   dout_163=dout_160^dout_121;
   dout_164=dout_161|dout_162;
   dout_165=dout_130^dout_137;
   dout_166=dout_130&dout_137;
   dout_167=dout_165&dout_126;
   dout_168=dout_165^dout_126;
   dout_169=dout_166|dout_167;
   dout_170=dout_96^dout_138;
   dout_171=dout_96&dout_138;
   dout_172=dout_170&dout_131;
   dout_173=dout_170^dout_131;
   dout_174=dout_171|dout_172;
   dout_176=((B >> 1)&1)&((A >> 5)&1);
   dout_177=((B >> 2)&1)&((A >> 5)&1);
   dout_178=((B >> 3)&1)&((A >> 5)&1);
   dout_179=((B >> 4)&1)&((A >> 5)&1);
   dout_180=((B >> 5)&1)&((A >> 5)&1);
   dout_181=((B >> 6)&1)&((A >> 5)&1);
   dout_182=((B >> 7)&1)&((A >> 5)&1);
   dout_186=dout_133^0xFFFFFFFFFFFFFFFFU;
   dout_188=dout_150^dout_176;
   dout_189=dout_150&dout_176;
   dout_190=dout_188&dout_106;
   dout_191=dout_188^dout_106;
   dout_192=dout_189|dout_190;
   dout_193=dout_158^dout_177;
   dout_194=dout_158&dout_177;
   dout_195=dout_193&dout_151;
   dout_196=dout_193^dout_151;
   dout_197=dout_194|dout_195;
   dout_198=dout_163^dout_178;
   dout_199=dout_163&dout_178;
   dout_200=dout_198&dout_159;
   dout_201=dout_198^dout_159;
   dout_202=dout_199|dout_200;
   dout_203=dout_168^dout_179;
   dout_204=dout_168&dout_179;
   dout_205=dout_203&dout_164;
   dout_206=dout_203^dout_164;
   dout_207=dout_204|dout_205;
   dout_208=dout_173^dout_180;
   dout_209=dout_173&dout_180;
   dout_210=dout_208&dout_169;
   dout_211=dout_208^dout_169;
   dout_212=dout_209|dout_210;
   dout_213=dout_139^dout_181;
   dout_214=dout_139&dout_181;
   dout_215=dout_213&dout_174;
   dout_216=dout_213^dout_174;
   dout_217=dout_214|dout_215;
   dout_218=((B >> 0)&1)&((A >> 6)&1);
   dout_219=((B >> 1)&1)&((A >> 6)&1);
   dout_220=((B >> 2)&1)&((A >> 6)&1);
   dout_221=((B >> 3)&1)&((A >> 6)&1);
   dout_222=((B >> 4)&1)&((A >> 6)&1);
   dout_223=((B >> 5)&1)&((A >> 6)&1);
   dout_224=((B >> 6)&1)&((A >> 6)&1);
   dout_225=((B >> 7)&1)&((A >> 6)&1);
   dout_226=dout_191^dout_218;
   dout_227=dout_191&dout_218;
   dout_228=dout_226&dout_133;
   dout_229=dout_226^dout_133;
   dout_230=dout_227|dout_228;
   dout_231=dout_196^dout_219;
   dout_232=dout_196&dout_219;
   dout_233=dout_231&dout_192;
   dout_234=dout_231^dout_192;
   dout_235=dout_232|dout_233;
   dout_236=dout_201^dout_220;
   dout_237=dout_201&dout_220;
   dout_238=dout_236&dout_197;
   dout_239=dout_236^dout_197;
   dout_240=dout_237|dout_238;
   dout_241=dout_206^dout_221;
 

   dout_242=dout_206&dout_221;
   dout_243=dout_241&dout_202;
   dout_244=dout_241^dout_202;
   dout_245=dout_242|dout_243;
   dout_246=dout_211^dout_222;
   dout_247=dout_211&dout_222;
   dout_248=dout_246&dout_207;
   dout_249=dout_246^dout_207;
   dout_250=dout_247|dout_248;
   dout_251=dout_216^dout_223;
   dout_252=dout_216&dout_223;
   dout_253=dout_251&dout_212;
   dout_254=dout_251^dout_212;
   dout_255=dout_252|dout_253;
   dout_256=dout_182^dout_224;
   dout_257=dout_182&dout_224;
   dout_258=dout_256&dout_217;
   dout_259=dout_256^dout_217;
   dout_260=dout_257|dout_258;
   dout_261=((B >> 0)&1)&((A >> 7)&1);
   dout_262=((B >> 1)&1)&((A >> 7)&1);
   dout_263=((B >> 2)&1)&((A >> 7)&1);
   dout_264=((B >> 3)&1)&((A >> 7)&1);
   dout_265=((B >> 4)&1)&((A >> 7)&1);
   dout_266=((B >> 5)&1)&((A >> 7)&1);
   dout_267=((B >> 6)&1)&((A >> 7)&1);
   dout_268=((B >> 7)&1)&((A >> 7)&1);
   dout_269=dout_234^dout_261;
   dout_270=dout_234&dout_261;
   dout_271=dout_269&dout_230;
   dout_272=dout_269^dout_230;
   dout_273=dout_270|dout_271;
   dout_274=dout_239^dout_262;
   dout_275=dout_239&dout_262;
   dout_276=dout_274&dout_235;
   dout_277=dout_274^dout_235;
   dout_278=dout_275|dout_276;
   dout_279=dout_244^dout_263;
   dout_280=dout_244&dout_263;
   dout_281=dout_279&dout_240;
   dout_282=dout_279^dout_240;
   dout_283=dout_280|dout_281;
   dout_284=dout_249^dout_264;
   dout_285=dout_249&dout_264;
   dout_286=dout_284&dout_245;
   dout_287=dout_284^dout_245;
   dout_288=dout_285|dout_286;
   dout_289=dout_254^dout_265;
   dout_290=dout_254&dout_265;
   dout_291=dout_289&dout_250;
   dout_292=dout_289^dout_250;
   dout_293=dout_290|dout_291;
   dout_294=dout_259^dout_266;
   dout_295=dout_259&dout_266;
   dout_296=dout_294&dout_255;
   dout_297=dout_294^dout_255;
   dout_298=dout_295|dout_296;
   dout_299=dout_225^dout_267;
   dout_300=dout_225&dout_267;
   dout_301=dout_299&dout_260;
   dout_302=dout_299^dout_260;
   dout_303=dout_300|dout_301;
   dout_304=dout_277^dout_273;
   dout_305=dout_277&dout_273;
   dout_306=dout_282^dout_278;
   dout_307=dout_282&dout_278;
   dout_308=dout_306&dout_305;
   dout_309=dout_306^dout_305;
   dout_310=dout_307|dout_308;
   dout_311=dout_287^dout_283;
   dout_312=dout_287&dout_283;
   dout_313=dout_311&dout_310;
   dout_314=dout_311^dout_310;
   dout_315=dout_312|dout_313;
   dout_316=dout_292^dout_288;
   dout_317=dout_292&dout_288;
   dout_318=dout_316&dout_315;
   dout_319=dout_316^dout_315;
   dout_320=dout_317|dout_318;
   dout_321=dout_297^dout_293;
   dout_322=dout_297&dout_293;
   dout_323=dout_321&dout_320;
   dout_324=dout_321^dout_320;
   dout_325=dout_322|dout_323;
   dout_326=dout_302^dout_298;
   dout_327=dout_302&dout_298;
   dout_328=dout_326&dout_325;
   dout_329=dout_326^dout_325;
   dout_330=dout_327|dout_328;
   dout_331=dout_268^dout_303;
   dout_332=((A >> 7)&1)&dout_303;
   dout_333=dout_331&dout_330;
   dout_334=dout_331^dout_330;
   dout_335=dout_332|dout_333;

   O = 0;
   O |= (dout_134&1) << 0;
   O |= (dout_252&1) << 1;
   O |= (((A >> 5)&1)&1) << 2;
   O |= (dout_180&1) << 3;
   O |= (dout_133&1) << 4;
   O |= (dout_186&1) << 5;
   O |= (dout_229&1) << 6;
   O |= (dout_272&1) << 7;
   O |= (dout_304&1) << 8;
   O |= (dout_309&1) << 9;
   O |= (dout_314&1) << 10;
   O |= (dout_319&1) << 11;
   O |= (dout_324&1) << 12;
   O |= (dout_329&1) << 13;
   O |= (dout_334&1) << 14;
   O |= (dout_335&1) << 15;
   return O;
}

uint32_t mul16u_5FA (uint16_t a, uint16_t b) {
    static uint16_t * cacheLL = NULL;
    static uint16_t * cacheLH = NULL;
    static uint16_t * cacheHL = NULL;
    static uint16_t * cacheHH = NULL;
    int fillData = cacheLL == NULL || cacheLH == NULL || cacheHL == NULL || cacheHH == NULL;

    if(!cacheLL) cacheLL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheLH) cacheLH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHL) cacheHL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHH) cacheHH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    
    if(fillData) {
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                cacheLL[i * 256 + j] = mult8_cgp14_wc79_csamrca(j, i);
                cacheLH[i * 256 + j] = trun8_tam00b(i, j);
                cacheHL[i * 256 + j] = mul8_364(i, j);
                cacheHH[i * 256 + j] = trun8_tam00b(i, j);
            }
        }
    }

    uint32_t opt = 0;

    opt += (uint32_t)cacheLL[(a & 0xFF       ) * 256 + (b & 0xFF             )];
    opt += (uint32_t)cacheLH[(a & 0xFF       ) * 256 + ((b >> 8) & 0xFF      )] << 8;
    opt += (uint32_t)cacheHL[((a >> 8) & 0xFF) * 256 + (b & 0xFF             )] << 8;
    opt += (uint32_t)cacheHH[((a >> 8) & 0xFF) * 256 + ((b >> 8) & 0xFF      )] << 16;

    return opt;
}




uint64_t mult8_cgp14zr_wc907_rcam(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_65, dout_72, dout_73, dout_101, dout_102, dout_106, dout_107, dout_108, dout_109, dout_110, dout_116, dout_117, dout_118, dout_146, dout_148, dout_149, dout_150, dout_151, dout_152, dout_153, dout_154, dout_155, dout_160, dout_161, dout_162, dout_163, dout_186, dout_187, dout_188, dout_189, dout_190, dout_191, dout_192, dout_193, dout_194, dout_195, dout_196, dout_197, dout_198, dout_199, dout_200, dout_203, dout_205, dout_206, dout_207, dout_208, dout_222, dout_226, dout_227, dout_228, dout_229, dout_230, dout_231, dout_232, dout_233, dout_234, dout_235, dout_236, dout_237, dout_238, dout_239, dout_240, dout_241, dout_242, dout_243, dout_244, dout_245, dout_249, dout_250, dout_251, dout_252, dout_253, dout_266, dout_267, dout_268, dout_269, dout_270, dout_271, dout_272, dout_273, dout_274, dout_275, dout_276, dout_277, dout_278, dout_279, dout_280, dout_281, dout_282, dout_283, dout_284, dout_285, dout_286, dout_287, dout_288, dout_289, dout_290, dout_292, dout_293, dout_294, dout_295, dout_296, dout_297, dout_298, dout_306, dout_307, dout_311, dout_312, dout_313, dout_314, dout_315, dout_316, dout_317, dout_318, dout_319, dout_320, dout_321, dout_322, dout_323, dout_324, dout_325, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_334, dout_335;   int avg=0;

   dout_65=((B >> 7)&1)&((A >> 1)&1);
   dout_72=((B >> 6)&1)&((A >> 2)&1);
   dout_73=((B >> 7)&1)&((A >> 2)&1);
   dout_101=((B >> 5)&1)|((B >> 6)&1);
   dout_102=((B >> 5)&1)&dout_72;
   dout_106=dout_65^dout_73;
   dout_107=dout_65&((A >> 2)&1);
   dout_108=((B >> 7)&1)&dout_102;
   dout_109=dout_106^dout_102;
   dout_110=dout_107|dout_108;
   dout_116=dout_101&((A >> 3)&1);
   dout_117=((B >> 6)&1)&((A >> 3)&1);
   dout_118=((B >> 7)&1)&((A >> 3)&1);
   dout_146=dout_109^dout_117;
   dout_148=dout_106&dout_116;
   dout_149=dout_146^dout_116;
   dout_150=dout_117|dout_148;
   dout_151=dout_110^dout_118;
   dout_152=dout_107&((A >> 3)&1);
   dout_153=dout_118&dout_150;
   dout_154=dout_151^dout_150;
   dout_155=dout_152|dout_153;
   dout_160=((B >> 4)&1)&((A >> 4)&1);
   dout_161=((B >> 5)&1)&((A >> 4)&1);
   dout_162=((B >> 6)&1)&((A >> 4)&1);
   dout_163=((B >> 7)&1)&((A >> 4)&1);
   dout_186=dout_149^dout_161;
   dout_187=dout_149&dout_161;
   dout_188=dout_186&dout_160;
   dout_189=dout_186^dout_160;
   dout_190=dout_187|dout_188;
   dout_191=dout_154^dout_162;
   dout_192=dout_154&dout_162;
   dout_193=dout_191&dout_190;
   dout_194=dout_191^dout_190;
   dout_195=dout_192|dout_193;
   dout_196=dout_155^dout_163;
   dout_197=dout_155&((A >> 4)&1);
   dout_198=((B >> 7)&1)&dout_195;
   dout_199=dout_196^dout_195;
   dout_200=dout_197|dout_198;
   dout_203=((B >> 2)&1)&((A >> 6)&1);
   dout_205=((B >> 4)&1)&((A >> 5)&1);
   dout_206=((B >> 5)&1)&((A >> 5)&1);
   dout_207=((B >> 6)&1)&((A >> 5)&1);
   dout_208=((B >> 7)&1)&((A >> 5)&1);
   dout_222=((B >> 3)&1)&((A >> 5)&1);
   dout_226=dout_189^dout_205;
   dout_227=dout_189&dout_205;
   dout_228=dout_226&dout_222;
   dout_229=dout_226^dout_222;
   dout_230=dout_227|dout_228;
   dout_231=dout_194^dout_206;
   dout_232=dout_194&dout_206;
   dout_233=dout_231&dout_230;
   dout_234=dout_231^dout_230;
   dout_235=dout_232|dout_233;
   dout_236=dout_199^dout_207;
   dout_237=dout_199&dout_207;
   dout_238=dout_236&dout_235;
   dout_239=dout_236^dout_235;
   dout_240=dout_237|dout_238;
   dout_241=dout_200^dout_208;
   dout_242=dout_200&((A >> 5)&1);
   dout_243=dout_208&dout_240;
   dout_244=dout_241^dout_240;
   dout_245=dout_242|dout_243;
   dout_249=((B >> 3)&1)&((A >> 6)&1);
   dout_250=((B >> 4)&1)&((A >> 6)&1);
   dout_251=((B >> 5)&1)&((A >> 6)&1);
   dout_252=((B >> 6)&1)&((A >> 6)&1);
   dout_253=((B >> 7)&1)&((A >> 6)&1);
   dout_266=dout_229^dout_249;
   dout_267=dout_229&dout_249;
   dout_268=dout_266&dout_203;
   dout_269=dout_266^dout_203;
   dout_270=dout_267|dout_268;
   dout_271=dout_234^dout_250;
   dout_272=dout_234&dout_250;
   dout_273=dout_271&dout_270;
   dout_274=dout_271^dout_270;
   dout_275=dout_272|dout_273;
   dout_276=dout_239^dout_251;
   dout_277=dout_239&dout_251;
   dout_278=dout_276&dout_275;
   dout_279=dout_276^dout_275;
   dout_280=dout_277|dout_278;
   dout_281=dout_244^dout_252;
   dout_282=dout_244&dout_252;
   dout_283=dout_281&dout_280;
   dout_284=dout_281^dout_280;
   dout_285=dout_282|dout_283;
   dout_286=dout_245^dout_253;
   dout_287=dout_245&((A >> 6)&1);
   dout_288=((B >> 7)&1)&dout_285;
   dout_289=dout_286^dout_285;
   dout_290=dout_287|dout_288;
   dout_292=((B >> 1)&1)&((A >> 7)&1);
   dout_293=((B >> 2)&1)&((A >> 7)&1);
   dout_294=((B >> 3)&1)&((A >> 7)&1);
   dout_295=((B >> 4)&1)&((A >> 7)&1);
   dout_296=((B >> 5)&1)&((A >> 7)&1);
   dout_297=((B >> 6)&1)&((A >> 7)&1);
   dout_298=((B >> 7)&1)&((A >> 7)&1);
   dout_306=dout_269^dout_293;
   dout_307=dout_269&dout_293;
   dout_311=dout_274^dout_294;
   dout_312=dout_274&dout_294;
   dout_313=dout_311&dout_307;
   dout_314=dout_311^dout_307;
   dout_315=dout_312|dout_313;
   dout_316=dout_279^dout_295;
   dout_317=dout_279&dout_295;
   dout_318=dout_316&dout_315;
   dout_319=dout_316^dout_315;
   dout_320=dout_317|dout_318;
   dout_321=dout_284^dout_296;
   dout_322=dout_284&dout_296;
   dout_323=dout_321&dout_320;
   dout_324=dout_321^dout_320;
   dout_325=dout_322|dout_323;
   dout_326=dout_289^dout_297;
   dout_327=dout_289&dout_297;
   dout_328=dout_326&dout_325;
   dout_329=dout_326^dout_325;
   dout_330=dout_327|dout_328;
   dout_331=dout_290^dout_298;
   dout_332=dout_290&((A >> 7)&1);
   dout_333=((B >> 7)&1)&dout_330;
   dout_334=dout_331^dout_330;
   dout_335=dout_332|dout_333;

   O = 0;
   O |= (dout_249&1) << 0;
   O |= (dout_293&1) << 1;
   O |= (dout_312&1) << 2;
   O |= (0&1) << 3;
   O |= (dout_317&1) << 4;
   O |= (0&1) << 5;
   O |= (dout_72&1) << 6;
   O |= (dout_292&1) << 7;
   O |= (dout_292&1) << 8;
   O |= (dout_306&1) << 9;
   O |= (dout_314&1) << 10;
   O |= (dout_319&1) << 11;
   O |= (dout_324&1) << 12;
   O |= (dout_329&1) << 13;
   O |= (dout_334&1) << 14;
   O |= (dout_335&1) << 15;
   return O;
}



uint32_t mul16u_DAE (uint16_t a, uint16_t b) {
    static uint16_t * cacheLL = NULL;
    static uint16_t * cacheLH = NULL;
    static uint16_t * cacheHL = NULL;
    static uint16_t * cacheHH = NULL;
    int fillData = cacheLL == NULL || cacheLH == NULL || cacheHL == NULL || cacheHH == NULL;

    if(!cacheLL) cacheLL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheLH) cacheLH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHL) cacheHL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHH) cacheHH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    
    if(fillData) {
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                cacheLL[i * 256 + j] = mult8_cgp14zr_wc907_rcam(j, i);
                cacheLH[i * 256 + j] = trun8_tam00b(i, j);
                cacheHL[i * 256 + j] = mul8_364(i, j);
                cacheHH[i * 256 + j] = trun8_tam00b(i, j);
            }
        }
    }

    uint32_t opt = 0;

    opt += (uint32_t)cacheLL[(a & 0xFF       ) * 256 + (b & 0xFF             )];
    opt += (uint32_t)cacheLH[(a & 0xFF       ) * 256 + ((b >> 8) & 0xFF      )] << 8;
    opt += (uint32_t)cacheHL[((a >> 8) & 0xFF) * 256 + (b & 0xFF             )] << 8;
    opt += (uint32_t)cacheHH[((a >> 8) & 0xFF) * 256 + ((b >> 8) & 0xFF      )] << 16;

    return opt;
}

uint64_t mult8_cgp14ep_ep64716_wc26_csamrca(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_20, dout_21, dout_22, dout_23, dout_27, dout_28, dout_29, dout_30, dout_31, dout_38, dout_39, dout_40, dout_41, dout_42, dout_43, dout_44, dout_45, dout_48, dout_49, dout_50, dout_51, dout_52, dout_53, dout_63, dout_64, dout_65, dout_69, dout_70, dout_71, dout_72, dout_73, dout_74, dout_75, dout_76, dout_77, dout_78, dout_79, dout_80, dout_81, dout_82, dout_83, dout_84, dout_85, dout_87, dout_88, dout_91, dout_92, dout_93, dout_94, dout_95, dout_96, dout_103, dout_105, dout_107, dout_108, dout_109, dout_110, dout_111, dout_112, dout_113, dout_114, dout_115, dout_116, dout_117, dout_118, dout_119, dout_120, dout_121, dout_122, dout_123, dout_124, dout_125, dout_126, dout_127, dout_128, dout_129, dout_130, dout_131, dout_132, dout_133, dout_134, dout_135, dout_136, dout_137, dout_138, dout_139, dout_140, dout_141, dout_145, dout_146, dout_147, dout_148, dout_149, dout_150, dout_151, dout_152, dout_153, dout_154, dout_155, dout_156, dout_157, dout_158, dout_159, dout_160, dout_161, dout_162, dout_163, dout_164, dout_165, dout_166, dout_167, dout_168, dout_169, dout_170, dout_171, dout_172, dout_173, dout_174, dout_175, dout_176, dout_177, dout_178, dout_179, dout_180, dout_181, dout_182, dout_183, dout_184, dout_185, dout_186, dout_187, dout_188, dout_189, dout_190, dout_191, dout_192, dout_193, dout_194, dout_195, dout_196, dout_197, dout_198, dout_199, dout_200, dout_201, dout_202, dout_203, dout_204, dout_205, dout_206, dout_207, dout_208, dout_209, dout_210, dout_211, dout_212, dout_213, dout_214, dout_215, dout_216, dout_217, dout_218, dout_219, dout_220, dout_221, dout_222, dout_223, dout_224, dout_225, dout_226, dout_227, dout_228, dout_229, dout_230, dout_231, dout_232, dout_233, dout_234, dout_235, dout_236, dout_237, dout_238, dout_239, dout_240, dout_241, dout_242, dout_243, dout_244, dout_245, dout_246, dout_247, dout_248, dout_249, dout_250, dout_251, dout_252, dout_253, dout_254, dout_255, dout_256, dout_257, dout_258, dout_259, dout_260, dout_261, dout_262, dout_263, dout_264, dout_265, dout_266, dout_267, dout_268, dout_269, dout_270, dout_271, dout_272, dout_273, dout_274, dout_275, dout_276, dout_277, dout_278, dout_279, dout_280, dout_281, dout_282, dout_283, dout_284, dout_285, dout_286, dout_287, dout_288, dout_289, dout_290, dout_291, dout_292, dout_293, dout_294, dout_295, dout_296, dout_297, dout_298, dout_299, dout_300, dout_301, dout_302, dout_303, dout_304, dout_305, dout_306, dout_307, dout_308, dout_309, dout_310, dout_311, dout_312, dout_313, dout_314, dout_315, dout_316, dout_317, dout_318, dout_319, dout_320, dout_321, dout_322, dout_323, dout_324, dout_325, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_334, dout_335;   int avg=0;

   dout_20=((B >> 4)&1)&((A >> 0)&1);
   dout_21=((B >> 5)&1)&((A >> 0)&1);
   dout_22=((B >> 6)&1)&((A >> 0)&1);
   dout_23=((B >> 7)&1)&((A >> 0)&1);
   dout_27=((B >> 3)&1)&((A >> 1)&1);
   dout_28=((B >> 4)&1)&((A >> 1)&1);
   dout_29=((B >> 5)&1)&((A >> 1)&1);
   dout_30=((B >> 6)&1)&((A >> 1)&1);
   dout_31=((B >> 7)&1)&((A >> 1)&1);
   dout_38=dout_20|dout_27;
   dout_39=dout_20&dout_27;
   dout_40=dout_21^dout_28;
   dout_41=dout_21&dout_28;
   dout_42=dout_22^dout_29;
   dout_43=dout_22&dout_29;
   dout_44=dout_23^dout_30;
   dout_45=dout_23&dout_30;
   dout_48=((B >> 2)&1)&((A >> 2)&1);
   dout_49=((B >> 3)&1)&((A >> 2)&1);
   dout_50=((B >> 4)&1)&((A >> 2)&1);
   dout_51=((B >> 5)&1)&((A >> 2)&1);
   dout_52=((B >> 6)&1)&((A >> 2)&1);
   dout_53=((B >> 7)&1)&((A >> 2)&1);
   dout_63=((A >> 3)&1)&((B >> 1)&1);
   dout_64=dout_38|dout_48;
   dout_65=dout_38&dout_48;
   dout_69=dout_40^dout_49;
   dout_70=dout_40&dout_49;
   dout_71=dout_69&dout_39;
   dout_72=dout_69^dout_39;
   dout_73=dout_70|dout_71;
   dout_74=dout_42^dout_50;
   dout_75=dout_42&dout_50;
   dout_76=dout_74&dout_41;
   dout_77=dout_74^dout_41;
   dout_78=dout_75|dout_76;
   dout_79=dout_44^dout_51;
   dout_80=dout_44&dout_51;
   dout_81=dout_79&dout_43;
   dout_82=dout_79^dout_43;
   dout_83=dout_80|dout_81;
   dout_84=dout_31^dout_52;
   dout_85=dout_31&dout_52;
   dout_87=dout_84^dout_45;
   dout_88=dout_85|dout_45;
   dout_91=((B >> 2)&1)&((A >> 3)&1);
   dout_92=((B >> 3)&1)&((A >> 3)&1);
   dout_93=((B >> 4)&1)&((A >> 3)&1);
   dout_94=((B >> 5)&1)&((A >> 3)&1);
   dout_95=((B >> 6)&1)&((A >> 3)&1);
   dout_96=((B >> 7)&1)&((A >> 3)&1);
   dout_103=dout_64&dout_63;
   dout_105=dout_64|dout_63;
   dout_107=dout_72^dout_91;
   dout_108=dout_72&dout_91;
   dout_109=dout_107&dout_65;
   dout_110=dout_107^dout_65;
   dout_111=dout_108|dout_109;
   dout_112=dout_77^dout_92;
   dout_113=dout_77&dout_92;
   dout_114=dout_112&dout_73;
   dout_115=dout_112^dout_73;
   dout_116=dout_113|dout_114;
   dout_117=dout_82^dout_93;
   dout_118=dout_82&dout_93;
   dout_119=dout_117&dout_78;
   dout_120=dout_117^dout_78;
   dout_121=dout_118|dout_119;
   dout_122=dout_87^dout_94;
   dout_123=dout_87&dout_94;
   dout_124=dout_122&dout_83;
   dout_125=dout_122^dout_83;
   dout_126=dout_123|dout_124;
   dout_127=dout_53^dout_95;
   dout_128=dout_53&dout_95;
   dout_129=dout_127&dout_88;
   dout_130=dout_127^dout_88;
   dout_131=dout_128|dout_129;
   dout_132=((B >> 0)&1)&((A >> 4)&1);
   dout_133=((B >> 1)&1)&((A >> 4)&1);
   dout_134=((B >> 2)&1)&((A >> 4)&1);
   dout_135=((B >> 3)&1)&((A >> 4)&1);
   dout_136=((B >> 4)&1)&((A >> 4)&1);
   dout_137=((B >> 5)&1)&((A >> 4)&1);
   dout_138=((B >> 6)&1)&((A >> 4)&1);
   dout_139=((B >> 7)&1)&((A >> 4)&1);
   dout_140=dout_105^dout_132;
   dout_141=dout_105&dout_132;
   dout_145=dout_110^dout_133;
   dout_146=dout_110&dout_133;
   dout_147=dout_145&dout_103;
   dout_148=dout_145^dout_103;
   dout_149=dout_146|dout_147;
   dout_150=dout_115^dout_134;
   dout_151=dout_115&dout_134;
   dout_152=dout_150&dout_111;
   dout_153=dout_150^dout_111;
   dout_154=dout_151|dout_152;
   dout_155=dout_120^dout_135;
   dout_156=dout_120&dout_135;
   dout_157=dout_155&dout_116;
   dout_158=dout_155^dout_116;
   dout_159=dout_156|dout_157;
   dout_160=dout_125^dout_136;
   dout_161=dout_125&dout_136;
   dout_162=dout_160&dout_121;
   dout_163=dout_160^dout_121;
   dout_164=dout_161|dout_162;
   dout_165=dout_130^dout_137;
   dout_166=dout_130&dout_137;
   dout_167=dout_165&dout_126;
   dout_168=dout_165^dout_126;
   dout_169=dout_166|dout_167;
   dout_170=dout_96^dout_138;
   dout_171=dout_96&dout_138;
   dout_172=dout_170&dout_131;
   dout_173=dout_170^dout_131;
   dout_174=dout_171|dout_172;
   dout_175=((B >> 0)&1)&((A >> 5)&1);
   dout_176=((B >> 1)&1)&((A >> 5)&1);
   dout_177=((B >> 2)&1)&((A >> 5)&1);
   dout_178=((B >> 3)&1)&((A >> 5)&1);
   dout_179=((B >> 4)&1)&((A >> 5)&1);
   dout_180=((B >> 5)&1)&((A >> 5)&1);
   dout_181=((B >> 6)&1)&((A >> 5)&1);
   dout_182=((B >> 7)&1)&((A >> 5)&1);
   dout_183=dout_148^dout_175;
   dout_184=dout_148&dout_175;
   dout_185=dout_183&dout_141;
   dout_186=dout_183^dout_141;
   dout_187=dout_184|dout_185;
   dout_188=dout_153^dout_176;
   dout_189=dout_153&dout_176;
   dout_190=dout_188&dout_149;
   dout_191=dout_188^dout_149;
   dout_192=dout_189|dout_190;
   dout_193=dout_158^dout_177;
   dout_194=dout_158&dout_177;
   dout_195=dout_193&dout_154;
   dout_196=dout_193^dout_154;
   dout_197=dout_194|dout_195;
   dout_198=dout_163^dout_178;
   dout_199=dout_163&dout_178;
   dout_200=dout_198&dout_159;
   dout_201=dout_198^dout_159;
   dout_202=dout_199|dout_200;
   dout_203=dout_168^dout_179;
   dout_204=dout_168&dout_179;
   dout_205=dout_203&dout_164;
   dout_206=dout_203^dout_164;
   dout_207=dout_204|dout_205;
   dout_208=dout_173^dout_180;
   dout_209=dout_173&dout_180;
   dout_210=dout_208&dout_169;
   dout_211=dout_208^dout_169;
   dout_212=dout_209|dout_210;
   dout_213=dout_139^dout_181;
   dout_214=dout_139&dout_181;
   dout_215=dout_213&dout_174;
   dout_216=dout_213^dout_174;
   dout_217=dout_214|dout_215;
   dout_218=((B >> 0)&1)&((A >> 6)&1);
   dout_219=((B >> 1)&1)&((A >> 6)&1);
   dout_220=((B >> 2)&1)&((A >> 6)&1);
   dout_221=((B >> 3)&1)&((A >> 6)&1);
   dout_222=((B >> 4)&1)&((A >> 6)&1);
   dout_223=((B >> 5)&1)&((A >> 6)&1);
   dout_224=((B >> 6)&1)&((A >> 6)&1);
   dout_225=((B >> 7)&1)&((A >> 6)&1);
   dout_226=dout_191^dout_218;
   dout_227=dout_191&dout_218;
   dout_228=dout_226&dout_187;
   dout_229=dout_226^dout_187;
   dout_230=dout_227|dout_228;
   dout_231=dout_196^dout_219;
   dout_232=dout_196&dout_219;
   dout_233=dout_231&dout_192;
   dout_234=dout_231^dout_192;
   dout_235=dout_232|dout_233;
   dout_236=dout_201^dout_220;
   dout_237=dout_201&dout_220;
   dout_238=dout_236&dout_197;
   dout_239=dout_236^dout_197;
   dout_240=dout_237|dout_238;
   dout_241=dout_206^dout_221;
   dout_242=dout_206&dout_221;
   dout_243=dout_241&dout_202;
   dout_244=dout_241^dout_202;
   dout_245=dout_242|dout_243;
   dout_246=dout_211^dout_222;
   dout_247=dout_211&dout_222;
   dout_248=dout_246&dout_207;
   dout_249=dout_246^dout_207;
   dout_250=dout_247|dout_248;
   dout_251=dout_216^dout_223;
   dout_252=dout_216&dout_223;
   dout_253=dout_251&dout_212;
   dout_254=dout_251^dout_212;
   dout_255=dout_252|dout_253;
   dout_256=dout_182^dout_224;
   dout_257=dout_182&dout_224;
   dout_258=dout_256&dout_217;
   dout_259=dout_256^dout_217;
   dout_260=dout_257|dout_258;
   dout_261=((B >> 0)&1)&((A >> 7)&1);
   dout_262=((B >> 1)&1)&((A >> 7)&1);
   dout_263=((B >> 2)&1)&((A >> 7)&1);
   dout_264=((B >> 3)&1)&((A >> 7)&1);
   dout_265=((B >> 4)&1)&((A >> 7)&1);
   dout_266=((B >> 5)&1)&((A >> 7)&1);
   dout_267=((B >> 6)&1)&((A >> 7)&1);
   dout_268=((B >> 7)&1)&((A >> 7)&1);
   dout_269=dout_234^dout_261;
   dout_270=dout_234&dout_261;
   dout_271=dout_269&dout_230;
   dout_272=dout_269^dout_230;
   dout_273=dout_270|dout_271;
   dout_274=dout_239^dout_262;
   dout_275=dout_239&dout_262;
   dout_276=dout_274&dout_235;
   dout_277=dout_274^dout_235;
   dout_278=dout_275|dout_276;
   dout_279=dout_244^dout_263;
   dout_280=dout_244&dout_263;
   dout_281=dout_279&dout_240;
   dout_282=dout_279^dout_240;
   dout_283=dout_280|dout_281;
   dout_284=dout_249^dout_264;
   dout_285=dout_249&dout_264;
   dout_286=dout_284&dout_245;
   dout_287=dout_284^dout_245;
   dout_288=dout_285|dout_286;
   dout_289=dout_254^dout_265;
   dout_290=dout_254&dout_265;
   dout_291=dout_289&dout_250;
   dout_292=dout_289^dout_250;
   dout_293=dout_290|dout_291;
   dout_294=dout_259^dout_266;
   dout_295=dout_259&dout_266;
   dout_296=dout_294&dout_255;
   dout_297=dout_294^dout_255;
   dout_298=dout_295|dout_296;
   dout_299=dout_225^dout_267;
   dout_300=dout_225&dout_267;
   dout_301=dout_299&dout_260;
   dout_302=dout_299^dout_260;
   dout_303=dout_300|dout_301;
   dout_304=dout_277^dout_273;
   dout_305=dout_277&dout_273;
   dout_306=dout_282^dout_278;
   dout_307=dout_282&dout_278;
   dout_308=dout_306&dout_305;
   dout_309=dout_306^dout_305;
   dout_310=dout_307|dout_308;
   dout_311=dout_287^dout_283;
   dout_312=dout_287&dout_283;
   dout_313=dout_311&dout_310;
   dout_314=dout_311^dout_310;
   dout_315=dout_312|dout_313;
   dout_316=dout_292^dout_288;
   dout_317=dout_292&dout_288;
   dout_318=dout_316&dout_315;
   dout_319=dout_316^dout_315;
   dout_320=dout_317|dout_318;
   dout_321=dout_297^dout_293;
   dout_322=dout_297&dout_293;
   dout_323=dout_321&dout_320;
   dout_324=dout_321^dout_320;
   dout_325=dout_322|dout_323;
   dout_326=dout_302^dout_298;
   dout_327=dout_302&dout_298;
   dout_328=dout_326&dout_325;
   dout_329=dout_326^dout_325;
   dout_330=dout_327|dout_328;
   dout_331=dout_268^dout_303;
   dout_332=((A >> 7)&1)&dout_303;
   dout_333=dout_331&dout_330;
   dout_334=dout_331^dout_330;
   dout_335=dout_332|dout_333;

   O = 0;
   O |= (0&1) << 0;
   O |= (dout_218&1) << 1;
   O |= (dout_132&1) << 2;
   O |= (dout_105&1) << 3;
   O |= (dout_140&1) << 4;
   O |= (dout_186&1) << 5;
   O |= (dout_229&1) << 6;
   O |= (dout_272&1) << 7;
   O |= (dout_304&1) << 8;
   O |= (dout_309&1) << 9;
   O |= (dout_314&1) << 10;
   O |= (dout_319&1) << 11;
   O |= (dout_324&1) << 12;
   O |= (dout_329&1) << 13;
   O |= (dout_334&1) << 14;
   O |= (dout_335&1) << 15;
   return O;
}

uint64_t mult8_cgp14zr_wc7391_rcam(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_242, dout_246, dout_251, dout_252, dout_253, dout_286, dout_288, dout_289, dout_290, dout_295, dout_296, dout_297, dout_298, dout_323, dout_324, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_334, dout_335;   int avg=0;

   dout_242=((A >> 5)&1)&((B >> 7)&1);
   dout_246=dout_242&((A >> 6)&1);
   dout_251=((B >> 5)&1)&((A >> 4)&1);
   dout_252=((B >> 6)&1)&((A >> 6)&1);
   dout_253=((B >> 7)&1)&((A >> 6)&1);
   dout_286=dout_242^dout_253;
   dout_288=((B >> 7)&1)&dout_252;
   dout_289=dout_286^dout_252;
   dout_290=dout_246|dout_288;
   dout_295=((B >> 4)&1)&((A >> 7)&1);
   dout_296=((B >> 5)&1)&((A >> 7)&1);
   dout_297=((B >> 6)&1)&((A >> 7)&1);
   dout_298=((B >> 7)&1)&((A >> 7)&1);
   dout_323=dout_296&((B >> 4)&1);
   dout_324=dout_296^dout_295;
   dout_326=dout_289^dout_297;
   dout_327=dout_289&dout_297;
   dout_328=dout_326&dout_323;
   dout_329=dout_326^dout_323;
   dout_330=dout_327|dout_328;
   dout_331=dout_290^dout_298;
   dout_332=dout_290&dout_298;
   dout_333=((B >> 7)&1)&dout_330;
   dout_334=dout_331^dout_330;
   dout_335=dout_332|dout_333;

   O = 0;
   O |= (0&1) << 0;
   O |= (dout_251&1) << 1;
   O |= (0&1) << 2;
   O |= (0&1) << 3;
   O |= (0&1) << 4;
   O |= (0&1) << 5;
   O |= (dout_332&1) << 6;
   O |= (dout_335&1) << 7;
   O |= (dout_298&1) << 8;
   O |= (0&1) << 9;
   O |= (dout_329&1) << 10;
   O |= (dout_251&1) << 11;
   O |= (dout_324&1) << 12;
   O |= (dout_329&1) << 13;
   O |= (dout_334&1) << 14;
   O |= (dout_335&1) << 15;
   return O;
}

uint32_t mul16u_F6B (uint16_t a, uint16_t b) {
    static uint16_t * cacheLL = NULL;
    static uint16_t * cacheLH = NULL;
    static uint16_t * cacheHL = NULL;
    static uint16_t * cacheHH = NULL;
    int fillData = cacheLL == NULL || cacheLH == NULL || cacheHL == NULL || cacheHH == NULL;

    if(!cacheLL) cacheLL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheLH) cacheLH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHL) cacheHL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHH) cacheHH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    
    if(fillData) {
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                cacheLL[i * 256 + j] = mult8_cgp14zr_wc7391_rcam(j, i);
                cacheLH[i * 256 + j] = mult8_cgp14ep_ep64716_wc26_csamrca(j, i);
                cacheHL[i * 256 + j] = mult8_cgp14ep_ep64716_wc26_csamrca(j, i);
                cacheHH[i * 256 + j] = mul8_364(i, j);
            }
        }
    }

    uint32_t opt = 0;

    opt += (uint32_t)cacheLL[(a & 0xFF       ) * 256 + (b & 0xFF             )];
    opt += (uint32_t)cacheLH[(a & 0xFF       ) * 256 + ((b >> 8) & 0xFF      )] << 8;
    opt += (uint32_t)cacheHL[((a >> 8) & 0xFF) * 256 + (b & 0xFF             )] << 8;
    opt += (uint32_t)cacheHH[((a >> 8) & 0xFF) * 256 + ((b >> 8) & 0xFF      )] << 16;

    return opt;
}

uint32_t mul16u_CK3 (uint16_t a, uint16_t b) {
    static uint16_t * cacheLL = NULL;
    static uint16_t * cacheLH = NULL;
    static uint16_t * cacheHL = NULL;
    static uint16_t * cacheHH = NULL;
    int fillData = cacheLL == NULL || cacheLH == NULL || cacheHL == NULL || cacheHH == NULL;

    if(!cacheLL) cacheLL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheLH) cacheLH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHL) cacheHL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHH) cacheHH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    
    if(fillData) {
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                cacheLL[i * 256 + j] = mult8_cgp14ep_ep65536_wc16384_2_csamcsa(j, i);
                cacheLH[i * 256 + j] = mult8_cgp14ep_ep63897_wc377_rcam(j, i);
                cacheHL[i * 256 + j] = mult8_cgp14ep_ep63897_wc377_rcam(j, i);
                cacheHH[i * 256 + j] = mul8_364(i, j);
            }
        }
    }

    uint32_t opt = 0;

    opt += (uint32_t)cacheLL[(a & 0xFF       ) * 256 + (b & 0xFF             )];
    opt += (uint32_t)cacheLH[(a & 0xFF       ) * 256 + ((b >> 8) & 0xFF      )] << 8;
    opt += (uint32_t)cacheHL[((a >> 8) & 0xFF) * 256 + (b & 0xFF             )] << 8;
    opt += (uint32_t)cacheHH[((a >> 8) & 0xFF) * 256 + ((b >> 8) & 0xFF      )] << 16;

    return opt;
}
uint64_t mult8_cgp14_wc3_csamcsa(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_18, dout_19, dout_20, dout_21, dout_22, dout_23, dout_25, dout_26, dout_27, dout_28, dout_29, dout_30, dout_31, dout_32, dout_34, dout_35, dout_36, dout_37, dout_38, dout_39, dout_40, dout_41, dout_42, dout_43, dout_44, dout_45, dout_46, dout_47, dout_48, dout_49, dout_50, dout_51, dout_52, dout_53, dout_54, dout_55, dout_59, dout_60, dout_61, dout_62, dout_63, dout_64, dout_65, dout_66, dout_67, dout_68, dout_69, dout_70, dout_71, dout_72, dout_73, dout_74, dout_75, dout_76, dout_77, dout_78, dout_79, dout_80, dout_81, dout_82, dout_83, dout_84, dout_85, dout_87, dout_88, dout_89, dout_90, dout_91, dout_92, dout_93, dout_94, dout_95, dout_96, dout_97, dout_98, dout_99, dout_100, dout_101, dout_102, dout_103, dout_104, dout_105, dout_106, dout_107, dout_108, dout_109, dout_110, dout_111, dout_112, dout_113, dout_114, dout_115, dout_116, dout_117, dout_118, dout_119, dout_120, dout_121, dout_122, dout_123, dout_124, dout_125, dout_126, dout_127, dout_128, dout_129, dout_130, dout_131, dout_132, dout_133, dout_134, dout_135, dout_136, dout_137, dout_138, dout_139, dout_140, dout_141, dout_142, dout_143, dout_144, dout_145, dout_146, dout_147, dout_148, dout_149, dout_150, dout_151, dout_152, dout_153, dout_154, dout_155, dout_156, dout_157, dout_158, dout_159, dout_160, dout_161, dout_162, dout_163, dout_164, dout_165, dout_166, dout_167, dout_168, dout_169, dout_170, dout_171, dout_172, dout_173, dout_174, dout_175, dout_176, dout_177, dout_178, dout_179, dout_180, dout_181, dout_182, dout_183, dout_184, dout_185, dout_186, dout_187, dout_188, dout_189, dout_190, dout_191, dout_192, dout_193, dout_194, dout_195, dout_196, dout_197, dout_198, dout_199, dout_200, dout_201, dout_202, dout_203, dout_204, dout_205, dout_206, dout_207, dout_208, dout_209, dout_210, dout_211, dout_212, dout_213, dout_214, dout_215, dout_216, dout_217, dout_218, dout_219, dout_220, dout_221, dout_222, dout_223, dout_224, dout_225, dout_226, dout_227, dout_228, dout_229, dout_230, dout_231, dout_232, dout_233, dout_234, dout_235, dout_236, dout_237, dout_238, dout_239, dout_240, dout_241, dout_242, dout_243, dout_244, dout_245, dout_246, dout_247, dout_248, dout_249, dout_250, dout_251, dout_252, dout_253, dout_254, dout_255, dout_256, dout_257, dout_258, dout_259, dout_260, dout_261, dout_262, dout_263, dout_264, dout_265, dout_266, dout_267, dout_268, dout_269, dout_270, dout_271, dout_272, dout_273, dout_274, dout_275, dout_276, dout_277, dout_278, dout_279, dout_280, dout_281, dout_282, dout_283, dout_284, dout_285, dout_286, dout_287, dout_288, dout_289, dout_290, dout_291, dout_292, dout_293, dout_294, dout_295, dout_296, dout_297, dout_298, dout_299, dout_300, dout_301, dout_302, dout_303, dout_304, dout_305, dout_306, dout_307, dout_308, dout_309, dout_310, dout_311, dout_312, dout_313, dout_314, dout_315, dout_316, dout_317, dout_318, dout_319, dout_320, dout_321, dout_322, dout_323, dout_324, dout_325, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_335, dout_336, dout_338, dout_339, dout_340, dout_341, dout_344, dout_345, dout_348, dout_349, dout_350, dout_352, dout_353, dout_354, dout_355, dout_356, dout_357, dout_358, dout_361, dout_362;   int avg=0;

   dout_18=((B >> 2)&1)&((A >> 0)&1);
   dout_19=((B >> 3)&1)&((A >> 0)&1);
   dout_20=((B >> 4)&1)&((A >> 0)&1);
   dout_21=((B >> 5)&1)&((A >> 0)&1);
   dout_22=((B >> 6)&1)&dout_21;
   dout_23=((B >> 7)&1)&((A >> 0)&1);
   dout_25=((B >> 1)&1)&((A >> 1)&1);
   dout_26=((B >> 2)&1)&((A >> 1)&1);
   dout_27=((B >> 3)&1)&((A >> 1)&1);
   dout_28=((B >> 4)&1)&((A >> 1)&1);
   dout_29=((B >> 5)&1)&((A >> 1)&1);
   dout_30=((B >> 6)&1)&((A >> 1)&1);
   dout_31=((B >> 7)&1)&((A >> 1)&1);
   dout_32=((A >> 0)&1)&((B >> 6)&1);
   dout_34=dout_18^dout_25;
   dout_35=dout_18&dout_25;
   dout_36=dout_19^dout_26;
   dout_37=dout_19&dout_26;
   dout_38=dout_20^dout_27;
   dout_39=dout_20&dout_27;
   dout_40=dout_21^dout_28;
   dout_41=dout_21&dout_28;
   dout_42=dout_32^dout_29;
   dout_43=dout_22&((A >> 1)&1);
   dout_44=dout_23^dout_30;
   dout_45=dout_31&dout_32;
   dout_46=((B >> 0)&1)&((A >> 2)&1);
   dout_47=((B >> 1)&1)&((A >> 2)&1);
   dout_48=((B >> 2)&1)&((A >> 2)&1);
   dout_49=((B >> 3)&1)&((A >> 2)&1);
   dout_50=((B >> 4)&1)&((A >> 2)&1);
   dout_51=((B >> 5)&1)&((A >> 2)&1);
   dout_52=((B >> 6)&1)&((A >> 2)&1);
   dout_53=((B >> 7)&1)&((A >> 2)&1);
   dout_54=dout_34^dout_46;
   dout_55=dout_34&dout_46;
   dout_59=dout_36^dout_47;
   dout_60=dout_36&dout_47;
   dout_61=dout_59&dout_35;
   dout_62=dout_59^dout_35;
   dout_63=dout_60|dout_61;
   dout_64=dout_38^dout_48;
   dout_65=dout_38&dout_48;
   dout_66=dout_64&dout_37;
   dout_67=dout_64^dout_37;
   dout_68=dout_65|dout_66;
   dout_69=dout_40^dout_49;
   dout_70=dout_40&dout_49;
   dout_71=dout_69&dout_39;
   dout_72=dout_69^dout_39;
   dout_73=dout_70|dout_71;
   dout_74=dout_42^dout_50;
   dout_75=dout_42&dout_50;
   dout_76=dout_74&dout_41;
   dout_77=dout_74^dout_41;
   dout_78=dout_75|dout_76;
   dout_79=dout_44^dout_51;
   dout_80=dout_44&dout_51;
   dout_81=dout_79&dout_43;
   dout_82=dout_79^dout_43;
   dout_83=dout_80|dout_81;
   dout_84=dout_31^dout_52;
   dout_85=dout_31&dout_52;
   dout_87=dout_84^dout_45;
   dout_88=dout_85|dout_45;
   dout_89=((B >> 0)&1)&((A >> 3)&1);
   dout_90=((B >> 1)&1)&((A >> 3)&1);
   dout_91=((B >> 2)&1)&((A >> 3)&1);
   dout_92=((B >> 3)&1)&((A >> 3)&1);
   dout_93=((B >> 4)&1)&((A >> 3)&1);
   dout_94=((B >> 5)&1)&((A >> 3)&1);
   dout_95=((B >> 6)&1)&((A >> 3)&1);
   dout_96=((B >> 7)&1)&((A >> 3)&1);
   dout_97=dout_62^dout_89;
   dout_98=dout_62&dout_89;
   dout_99=dout_97&dout_55;
   dout_100=dout_97^dout_55;
   dout_101=dout_98|dout_99;
   dout_102=dout_67^dout_90;
   dout_103=dout_67&dout_90;
   dout_104=dout_102&dout_63;
   dout_105=dout_102^dout_63;
   dout_106=dout_103|dout_104;
   dout_107=dout_72^dout_91;
   dout_108=dout_72&dout_91;
   dout_109=dout_107&dout_68;
   dout_110=dout_107^dout_68;
   dout_111=dout_108|dout_109;
   dout_112=dout_77^dout_92;
   dout_113=dout_77&dout_92;
   dout_114=dout_112&dout_73;
   dout_115=dout_112^dout_73;
   dout_116=dout_113|dout_114;
   dout_117=dout_82^dout_93;
   dout_118=dout_82&dout_93;
   dout_119=dout_117&dout_78;
   dout_120=dout_117^dout_78;
   dout_121=dout_118|dout_119;
   dout_122=dout_87^dout_94;
   dout_123=dout_87&dout_94;
   dout_124=dout_122&dout_83;
   dout_125=dout_122^dout_83;
   dout_126=dout_123|dout_124;
   dout_127=dout_53^dout_95;
   dout_128=dout_53&dout_95;
   dout_129=dout_127&dout_88;
   dout_130=dout_127^dout_88;
   dout_131=dout_128|dout_129;
   dout_132=((B >> 0)&1)&((A >> 4)&1);
   dout_133=((B >> 1)&1)&((A >> 4)&1);
   dout_134=((B >> 2)&1)&((A >> 4)&1);
   dout_135=((B >> 3)&1)&((A >> 4)&1);
   dout_136=((B >> 4)&1)&((A >> 4)&1);
   dout_137=((B >> 5)&1)&((A >> 4)&1);
   dout_138=((B >> 6)&1)&((A >> 4)&1);
   dout_139=((B >> 7)&1)&((A >> 4)&1);
   dout_140=dout_105^dout_132;
   dout_141=dout_105&dout_132;
   dout_142=dout_140&dout_101;
   dout_143=dout_140^dout_101;
   dout_144=dout_141|dout_142;
   dout_145=dout_110^dout_133;
   dout_146=dout_110&dout_133;
   dout_147=dout_145&dout_106;
   dout_148=dout_145^dout_106;
   dout_149=dout_146|dout_147;
   dout_150=dout_115^dout_134;
   dout_151=dout_115&dout_134;
   dout_152=dout_150&dout_111;
   dout_153=dout_150^dout_111;
   dout_154=dout_151|dout_152;
   dout_155=dout_120^dout_135;
   dout_156=dout_120&dout_135;
   dout_157=dout_155&dout_116;
   dout_158=dout_155^dout_116;
   dout_159=dout_156|dout_157;
   dout_160=dout_125^dout_136;
   dout_161=dout_125&dout_136;
   dout_162=dout_160&dout_121;
   dout_163=dout_160^dout_121;
   dout_164=dout_161|dout_162;
   dout_165=dout_130^dout_137;
   dout_166=dout_130&dout_137;
   dout_167=dout_165&dout_126;
   dout_168=dout_165^dout_126;
   dout_169=dout_166|dout_167;
   dout_170=dout_96^dout_138;
   dout_171=dout_96&dout_138;
   dout_172=dout_170&dout_131;
   dout_173=dout_170^dout_131;
   dout_174=dout_171|dout_172;
   dout_175=((B >> 0)&1)&((A >> 5)&1);
   dout_176=((B >> 1)&1)&((A >> 5)&1);
   dout_177=((B >> 2)&1)&((A >> 5)&1);
   dout_178=((B >> 3)&1)&((A >> 5)&1);
   dout_179=((B >> 4)&1)&((A >> 5)&1);
   dout_180=((B >> 5)&1)&((A >> 5)&1);
   dout_181=((B >> 6)&1)&((A >> 5)&1);
   dout_182=((B >> 7)&1)&((A >> 5)&1);
   dout_183=dout_148^dout_175;
   dout_184=dout_148&dout_175;
   dout_185=dout_183&dout_144;
   dout_186=dout_183^dout_144;
   dout_187=dout_184|dout_185;
   dout_188=dout_153^dout_176;
   dout_189=dout_153&dout_176;
   dout_190=dout_188&dout_149;
   dout_191=dout_188^dout_149;
   dout_192=dout_189|dout_190;
   dout_193=dout_158^dout_177;
   dout_194=dout_158&dout_177;
   dout_195=dout_193&dout_154;
   dout_196=dout_193^dout_154;
   dout_197=dout_194|dout_195;
   dout_198=dout_163^dout_178;
   dout_199=dout_163&dout_178;
   dout_200=dout_198&dout_159;
   dout_201=dout_198^dout_159;
   dout_202=dout_199|dout_200;
   dout_203=dout_168^dout_179;
   dout_204=dout_168&dout_179;
   dout_205=dout_203&dout_164;
   dout_206=dout_203^dout_164;
   dout_207=dout_204|dout_205;
   dout_208=dout_173^dout_180;
   dout_209=dout_173&dout_180;
   dout_210=dout_208&dout_169;
   dout_211=dout_208^dout_169;
   dout_212=dout_209|dout_210;
   dout_213=dout_139^dout_181;
   dout_214=dout_139&dout_181;
   dout_215=dout_213&dout_174;
   dout_216=dout_213^dout_174;
   dout_217=dout_214|dout_215;
   dout_218=((B >> 0)&1)&((A >> 6)&1);
   dout_219=((B >> 1)&1)&((A >> 6)&1);
   dout_220=((B >> 2)&1)&((A >> 6)&1);
   dout_221=((B >> 3)&1)&((A >> 6)&1);
   dout_222=((B >> 4)&1)&((A >> 6)&1);
   dout_223=((B >> 5)&1)&((A >> 6)&1);
   dout_224=((B >> 6)&1)&((A >> 6)&1);
   dout_225=((B >> 7)&1)&((A >> 6)&1);
   dout_226=dout_191^dout_218;
   dout_227=dout_191&dout_218;
   dout_228=dout_226&dout_187;
   dout_229=dout_226^dout_187;
   dout_230=dout_227|dout_228;
   dout_231=dout_196^dout_219;
   dout_232=dout_196&dout_219;
   dout_233=dout_231&dout_192;
   dout_234=dout_231^dout_192;
   dout_235=dout_232|dout_233;
   dout_236=dout_201^dout_220;
   dout_237=dout_201&dout_220;
   dout_238=dout_236&dout_197;
   dout_239=dout_236^dout_197;
   dout_240=dout_237|dout_238;
   dout_241=dout_206^dout_221;
   dout_242=dout_206&dout_221;
   dout_243=dout_241&dout_202;
   dout_244=dout_241^dout_202;
   dout_245=dout_242|dout_243;
   dout_246=dout_211^dout_222;
   dout_247=dout_211&dout_222;
   dout_248=dout_246&dout_207;
   dout_249=dout_246^dout_207;
   dout_250=dout_247|dout_248;
   dout_251=dout_216^dout_223;
   dout_252=dout_216&dout_223;
   dout_253=dout_251&dout_212;
   dout_254=dout_251^dout_212;
   dout_255=dout_252|dout_253;
   dout_256=dout_182^dout_224;
   dout_257=dout_182&dout_224;
   dout_258=dout_256&dout_217;
   dout_259=dout_256^dout_217;
   dout_260=dout_257|dout_258;
   dout_261=((B >> 0)&1)&((A >> 7)&1);
   dout_262=((B >> 1)&1)&((A >> 7)&1);
   dout_263=((B >> 2)&1)&((A >> 7)&1);
   dout_264=((B >> 3)&1)&((A >> 7)&1);
   dout_265=((B >> 4)&1)&((A >> 7)&1);
   dout_266=((B >> 5)&1)&((A >> 7)&1);
   dout_267=((B >> 6)&1)&((A >> 7)&1);
   dout_268=((B >> 7)&1)&((A >> 7)&1);
   dout_269=dout_234^dout_261;
   dout_270=dout_234&dout_261;
   dout_271=dout_269&dout_230;
   dout_272=dout_269^dout_230;
   dout_273=dout_270|dout_271;
   dout_274=dout_239^dout_262;
   dout_275=dout_239&dout_262;
   dout_276=dout_274&dout_235;
   dout_277=dout_274^dout_235;
   dout_278=dout_275|dout_276;
   dout_279=dout_244^dout_263;
   dout_280=dout_244&dout_263;
   dout_281=dout_279&dout_240;
   dout_282=dout_279^dout_240;
   dout_283=dout_280|dout_281;
   dout_284=dout_249^dout_264;
   dout_285=dout_249&dout_264;
   dout_286=dout_284&dout_245;
   dout_287=dout_284^dout_245;
   dout_288=dout_285|dout_286;
   dout_289=dout_254^dout_265;
   dout_290=dout_254&dout_265;
   dout_291=dout_289&dout_250;
   dout_292=dout_289^dout_250;
   dout_293=dout_290|dout_291;
   dout_294=dout_259^dout_266;
   dout_295=dout_259&dout_266;
   dout_296=dout_294&dout_255;
   dout_297=dout_294^dout_255;
   dout_298=dout_295|dout_296;
   dout_299=dout_225^dout_267;
   dout_300=dout_225&dout_267;
   dout_301=dout_299&dout_260;
   dout_302=dout_299^dout_260;
   dout_303=dout_300|dout_301;
   dout_304=dout_277^dout_273;
   dout_305=dout_277&dout_273;
   dout_306=dout_282^dout_278;
   dout_307=dout_282&dout_278;
   dout_308=dout_306&dout_305;
   dout_309=dout_306^dout_305;
   dout_310=dout_307|dout_308;
   dout_311=dout_287^dout_283;
   dout_312=dout_287&dout_283;
   dout_313=dout_311&dout_310;
   dout_314=dout_311^dout_310;
   dout_315=dout_312|dout_313;
   dout_316=dout_292^dout_288;
   dout_317=dout_292&dout_288;
   dout_318=dout_316&dout_315;
   dout_319=dout_316^dout_315;
   dout_320=dout_317|dout_318;
   dout_321=dout_320^0xFFFFFFFFFFFFFFFFU;
   dout_322=dout_297&dout_293;
   dout_323=dout_302^dout_298;
   dout_324=dout_302&dout_298;
   dout_325=dout_323&dout_322;
   dout_326=dout_323^dout_322;
   dout_327=dout_324|dout_325;
   dout_328=dout_268^dout_303;
   dout_329=((A >> 7)&1)&dout_303;
   dout_330=dout_328&dout_327;
   dout_331=dout_328^dout_327;
   dout_332=dout_329|dout_330;
   dout_333=dout_297^dout_293;
   dout_335=dout_333^0xFFFFFFFFFFFFFFFFU;
   dout_336=dout_322|dout_333;
   dout_338=dout_320^0xFFFFFFFFFFFFFFFFU;
   dout_339=dout_323&dout_336;
   dout_340=dout_323^dout_336;
   dout_341=dout_324|dout_339;
   dout_344=dout_328&dout_341;
   dout_345=dout_328^dout_341;
   dout_348=dout_333&dout_338;
   dout_349=dout_335&dout_320;
   dout_350=dout_348|dout_349;
   dout_352=dout_326&dout_321;
   dout_353=dout_340&dout_320;
   dout_354=dout_352|dout_353;
   dout_355=dout_320^0xFFFFFFFFFFFFFFFFU;
   dout_356=dout_331&dout_355;
   dout_357=dout_345&dout_320;
   dout_358=dout_356|dout_357;
   dout_361=dout_344&dout_320;
   dout_362=dout_332|dout_361;

   O = 0;
   O |= (dout_265&1) << 0;
   O |= (dout_25&1) << 1;
   O |= (dout_54&1) << 2;
   O |= (dout_100&1) << 3;
   O |= (dout_143&1) << 4;
   O |= (dout_186&1) << 5;
   O |= (dout_229&1) << 6;
   O |= (dout_272&1) << 7;
   O |= (dout_304&1) << 8;
   O |= (dout_309&1) << 9;
   O |= (dout_314&1) << 10;
   O |= (dout_319&1) << 11;
   O |= (dout_350&1) << 12;
   O |= (dout_354&1) << 13;
   O |= (dout_358&1) << 14;
   O |= (dout_362&1) << 15;
   return O;
}

uint64_t mult8_cgp14ep_ep65536_wc4545_rcam(const uint64_t B,const uint64_t A)
{
   uint64_t O, dout_163, dout_208, dout_241, dout_242, dout_251, dout_252, dout_253, dout_260, dout_272, dout_281, dout_283, dout_284, dout_285, dout_286, dout_287, dout_288, dout_289, dout_290, dout_295, dout_296, dout_297, dout_298, dout_312, dout_313, dout_315, dout_316, dout_317, dout_318, dout_319, dout_320, dout_321, dout_322, dout_323, dout_324, dout_325, dout_326, dout_327, dout_328, dout_329, dout_330, dout_331, dout_332, dout_333, dout_334, dout_335;   int avg=0;

   dout_163=((B >> 7)&1)&((A >> 4)&1);
   dout_208=((B >> 7)&1)&((A >> 5)&1);
   dout_241=dout_163^dout_208;
   dout_242=dout_163&((A >> 5)&1);
   dout_251=((B >> 5)&1)&((A >> 6)&1);
   dout_252=((B >> 6)&1)&((A >> 6)&1);
   dout_253=((B >> 7)&1)&((A >> 6)&1);
   dout_260=dout_252&dout_241;
   dout_272=((A >> 5)&1)&((B >> 6)&1);
   dout_281=dout_241^dout_252;
   dout_283=dout_281&dout_251;
   dout_284=dout_281^dout_251;
   dout_285=dout_260|dout_283;
   dout_286=dout_242^dout_253;
   dout_287=dout_242&dout_253;
   dout_288=dout_253&dout_285;
   dout_289=dout_286^dout_285;
   dout_290=dout_287|dout_288;
   dout_295=((B >> 4)&1)&((A >> 7)&1);
   dout_296=((B >> 5)&1)&((A >> 7)&1);
   dout_297=((B >> 6)&1)&((A >> 7)&1);
   dout_298=((B >> 7)&1)&((A >> 7)&1);
   dout_312=((A >> 7)&1)&((B >> 3)&1);
   dout_313=((B >> 4)&1)&((A >> 3)&1);
   dout_315=dout_312|dout_313;
   dout_316=dout_272^dout_295;
   dout_317=dout_272&dout_295;
   dout_318=dout_316&dout_315;
   dout_319=dout_316^dout_315;
   dout_320=dout_317|dout_318;
   dout_321=dout_284^dout_296;
   dout_322=dout_284&dout_296;
   dout_323=dout_321&dout_320;
   dout_324=dout_321^dout_320;
   dout_325=dout_322|dout_323;
   dout_326=dout_289^dout_297;
   dout_327=dout_289&dout_297;
   dout_328=dout_326&dout_325;
   dout_329=dout_326^dout_325;
   dout_330=dout_327|dout_328;
   dout_331=dout_290^dout_298;
   dout_332=dout_290&((A >> 7)&1);
   dout_333=((B >> 7)&1)&dout_330;
   dout_334=dout_331^dout_330;
   dout_335=dout_332|dout_333;

   O = 0;
   O |= (0&1) << 0;
   O |= (dout_287&1) << 1;
   O |= (dout_331&1) << 2;
   O |= (dout_288&1) << 3;
   O |= (0&1) << 4;
   O |= (dout_298&1) << 5;
   O |= (0&1) << 6;
   O |= (dout_331&1) << 7;
   O |= (dout_295&1) << 8;
   O |= (dout_298&1) << 9;
   O |= (dout_272&1) << 10;
   O |= (dout_319&1) << 11;
   O |= (dout_324&1) << 12;
   O |= (dout_329&1) << 13;
   O |= (dout_334&1) << 14;
   O |= (dout_335&1) << 15;
   return O;
}


uint32_t mul16u_8VH (uint16_t a, uint16_t b) {
    static uint16_t * cacheLL = NULL;
    static uint16_t * cacheLH = NULL;
    static uint16_t * cacheHL = NULL;
    static uint16_t * cacheHH = NULL;
    int fillData = cacheLL == NULL || cacheLH == NULL || cacheHL == NULL || cacheHH == NULL;

    if(!cacheLL) cacheLL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheLH) cacheLH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHL) cacheHL = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    if(!cacheHH) cacheHH = (uint16_t *)malloc(256 * 256 * sizeof(uint16_t));
    
    if(fillData) {
        for(int i = 0; i < 256; i++) {
            for(int j = 0; j < 256; j++) {
                cacheLL[i * 256 + j] = mult8_cgp14ep_ep65536_wc16384_2_csamcsa(j, i);
                cacheLH[i * 256 + j] = mult8_cgp14ep_ep65536_wc4545_rcam(j, i);
                cacheHL[i * 256 + j] = mult8_cgp14ep_ep65536_wc4545_rcam(j, i);
                cacheHH[i * 256 + j] = mult8_cgp14_wc3_csamcsa(j, i);
            }
        }
    }

    uint32_t opt = 0;

    opt += (uint32_t)cacheLL[(a & 0xFF       ) * 256 + (b & 0xFF             )];
    opt += (uint32_t)cacheLH[(a & 0xFF       ) * 256 + ((b >> 8) & 0xFF      )] << 8;
    opt += (uint32_t)cacheHL[((a >> 8) & 0xFF) * 256 + (b & 0xFF             )] << 8;
    opt += (uint32_t)cacheHH[((a >> 8) & 0xFF) * 256 + ((b >> 8) & 0xFF      )] << 16;

    return opt;
}
uint32_t mul16u_GPF(uint16_t A, uint16_t B)
{
  uint32_t P, P_;
  uint16_t tmp, C_10_10,C_10_11,C_10_12,C_10_13,C_10_14,C_11_10,C_11_11,C_11_12,C_11_13,C_11_14,C_11_9,C_12_10,C_12_11,C_12_12,C_12_13,C_12_14,C_12_8,C_12_9,C_13_10,C_13_11,C_13_12,C_13_13,C_13_14,C_13_7,C_13_8,C_13_9,C_14_10,C_14_11,C_14_12,C_14_13,C_14_14,C_14_6,C_14_7,C_14_8,C_14_9,C_15_10,C_15_11,C_15_12,C_15_13,C_15_14,C_15_5,C_15_6,C_15_7,C_15_8,C_15_9,C_16_10,C_16_11,C_16_12,C_16_13,C_16_14,C_16_5,C_16_6,C_16_7,C_16_8,C_16_9,C_8_12,C_8_13,C_8_14,C_9_11,C_9_12,C_9_13,C_9_14,S_10_10,S_10_11,S_10_12,S_10_13,S_10_14,S_10_15,S_11_10,S_11_11,S_11_12,S_11_13,S_11_14,S_11_15,S_11_9,S_12_10,S_12_11,S_12_12,S_12_13,S_12_14,S_12_15,S_12_8,S_12_9,S_13_10,S_13_11,S_13_12,S_13_13,S_13_14,S_13_15,S_13_7,S_13_8,S_13_9,S_14_10,S_14_11,S_14_12,S_14_13,S_14_14,S_14_15,S_14_6,S_14_7,S_14_8,S_14_9,S_15_10,S_15_11,S_15_12,S_15_13,S_15_14,S_15_15,S_15_5,S_15_6,S_15_7,S_15_8,S_15_9,S_16_10,S_16_11,S_16_12,S_16_13,S_16_14,S_16_15,S_16_4,S_16_5,S_16_6,S_16_7,S_16_8,S_16_9,S_7_13,S_7_14,S_7_15,S_8_12,S_8_13,S_8_14,S_8_15,S_9_11,S_9_12,S_9_13,S_9_14,S_9_15;
  S_7_13 = (((A>>7)&1) & ((B>>13)&1));
  S_7_14 = (((A>>7)&1) & ((B>>14)&1));
  S_7_15 = (((A>>7)&1) & ((B>>15)&1));
  S_8_12 = S_7_13^(((A>>8)&1) & ((B>>12)&1));
  C_8_12 = S_7_13&(((A>>8)&1) & ((B>>12)&1));
  S_8_13 = S_7_14^(((A>>8)&1) & ((B>>13)&1));
  C_8_13 = S_7_14&(((A>>8)&1) & ((B>>13)&1));
  S_8_14 = S_7_15^(((A>>8)&1) & ((B>>14)&1));
  C_8_14 = S_7_15&(((A>>8)&1) & ((B>>14)&1));
  S_8_15 = (((A>>8)&1) & ((B>>15)&1));
  S_9_11 = S_8_12^(((A>>9)&1) & ((B>>11)&1));
  C_9_11 = S_8_12&(((A>>9)&1) & ((B>>11)&1));
  tmp = S_8_13^C_8_12;
  S_9_12 = tmp^(((A>>9)&1) & ((B>>12)&1));
  C_9_12 = (tmp&(((A>>9)&1) & ((B>>12)&1)))|(S_8_13&C_8_12);
  tmp = S_8_14^C_8_13;
  S_9_13 = tmp^(((A>>9)&1) & ((B>>13)&1));
  C_9_13 = (tmp&(((A>>9)&1) & ((B>>13)&1)))|(S_8_14&C_8_13);
  tmp = S_8_15^C_8_14;
  S_9_14 = tmp^(((A>>9)&1) & ((B>>14)&1));
  C_9_14 = (tmp&(((A>>9)&1) & ((B>>14)&1)))|(S_8_15&C_8_14);
  S_9_15 = (((A>>9)&1) & ((B>>15)&1));
  S_10_10 = S_9_11^(((A>>10)&1) & ((B>>10)&1));
  C_10_10 = S_9_11&(((A>>10)&1) & ((B>>10)&1));
  tmp = S_9_12^C_9_11;
  S_10_11 = tmp^(((A>>10)&1) & ((B>>11)&1));
  C_10_11 = (tmp&(((A>>10)&1) & ((B>>11)&1)))|(S_9_12&C_9_11);
  tmp = S_9_13^C_9_12;
  S_10_12 = tmp^(((A>>10)&1) & ((B>>12)&1));
  C_10_12 = (tmp&(((A>>10)&1) & ((B>>12)&1)))|(S_9_13&C_9_12);
  tmp = S_9_14^C_9_13;
  S_10_13 = tmp^(((A>>10)&1) & ((B>>13)&1));
  C_10_13 = (tmp&(((A>>10)&1) & ((B>>13)&1)))|(S_9_14&C_9_13);
  tmp = S_9_15^C_9_14;
  S_10_14 = tmp^(((A>>10)&1) & ((B>>14)&1));
  C_10_14 = (tmp&(((A>>10)&1) & ((B>>14)&1)))|(S_9_15&C_9_14);
  S_10_15 = (((A>>10)&1) & ((B>>15)&1));
  S_11_9 = S_10_10^(((A>>11)&1) & ((B>>9)&1));
  C_11_9 = S_10_10&(((A>>11)&1) & ((B>>9)&1));
  tmp = S_10_11^C_10_10;
  S_11_10 = tmp^(((A>>11)&1) & ((B>>10)&1));
  C_11_10 = (tmp&(((A>>11)&1) & ((B>>10)&1)))|(S_10_11&C_10_10);
  tmp = S_10_12^C_10_11;
  S_11_11 = tmp^(((A>>11)&1) & ((B>>11)&1));
  C_11_11 = (tmp&(((A>>11)&1) & ((B>>11)&1)))|(S_10_12&C_10_11);
  tmp = S_10_13^C_10_12;
  S_11_12 = tmp^(((A>>11)&1) & ((B>>12)&1));
  C_11_12 = (tmp&(((A>>11)&1) & ((B>>12)&1)))|(S_10_13&C_10_12);
  tmp = S_10_14^C_10_13;
  S_11_13 = tmp^(((A>>11)&1) & ((B>>13)&1));
  C_11_13 = (tmp&(((A>>11)&1) & ((B>>13)&1)))|(S_10_14&C_10_13);
  tmp = S_10_15^C_10_14;
  S_11_14 = tmp^(((A>>11)&1) & ((B>>14)&1));
  C_11_14 = (tmp&(((A>>11)&1) & ((B>>14)&1)))|(S_10_15&C_10_14);
  S_11_15 = (((A>>11)&1) & ((B>>15)&1));
  S_12_8 = S_11_9^(((A>>12)&1) & ((B>>8)&1));
  C_12_8 = S_11_9&(((A>>12)&1) & ((B>>8)&1));
  tmp = S_11_10^C_11_9;
  S_12_9 = tmp^(((A>>12)&1) & ((B>>9)&1));
  C_12_9 = (tmp&(((A>>12)&1) & ((B>>9)&1)))|(S_11_10&C_11_9);
  tmp = S_11_11^C_11_10;
  S_12_10 = tmp^(((A>>12)&1) & ((B>>10)&1));
  C_12_10 = (tmp&(((A>>12)&1) & ((B>>10)&1)))|(S_11_11&C_11_10);
  tmp = S_11_12^C_11_11;
  S_12_11 = tmp^(((A>>12)&1) & ((B>>11)&1));
  C_12_11 = (tmp&(((A>>12)&1) & ((B>>11)&1)))|(S_11_12&C_11_11);
  tmp = S_11_13^C_11_12;
  S_12_12 = tmp^(((A>>12)&1) & ((B>>12)&1));
  C_12_12 = (tmp&(((A>>12)&1) & ((B>>12)&1)))|(S_11_13&C_11_12);
  tmp = S_11_14^C_11_13;
  S_12_13 = tmp^(((A>>12)&1) & ((B>>13)&1));
  C_12_13 = (tmp&(((A>>12)&1) & ((B>>13)&1)))|(S_11_14&C_11_13);
  tmp = S_11_15^C_11_14;
  S_12_14 = tmp^(((A>>12)&1) & ((B>>14)&1));
  C_12_14 = (tmp&(((A>>12)&1) & ((B>>14)&1)))|(S_11_15&C_11_14);
  S_12_15 = (((A>>12)&1) & ((B>>15)&1));
  S_13_7 = S_12_8^(((A>>13)&1) & ((B>>7)&1));
  C_13_7 = S_12_8&(((A>>13)&1) & ((B>>7)&1));
  tmp = S_12_9^C_12_8;
  S_13_8 = tmp^(((A>>13)&1) & ((B>>8)&1));
  C_13_8 = (tmp&(((A>>13)&1) & ((B>>8)&1)))|(S_12_9&C_12_8);
  tmp = S_12_10^C_12_9;
  S_13_9 = tmp^(((A>>13)&1) & ((B>>9)&1));
  C_13_9 = (tmp&(((A>>13)&1) & ((B>>9)&1)))|(S_12_10&C_12_9);
  tmp = S_12_11^C_12_10;
  S_13_10 = tmp^(((A>>13)&1) & ((B>>10)&1));
  C_13_10 = (tmp&(((A>>13)&1) & ((B>>10)&1)))|(S_12_11&C_12_10);
  tmp = S_12_12^C_12_11;
  S_13_11 = tmp^(((A>>13)&1) & ((B>>11)&1));
  C_13_11 = (tmp&(((A>>13)&1) & ((B>>11)&1)))|(S_12_12&C_12_11);
  tmp = S_12_13^C_12_12;
  S_13_12 = tmp^(((A>>13)&1) & ((B>>12)&1));
  C_13_12 = (tmp&(((A>>13)&1) & ((B>>12)&1)))|(S_12_13&C_12_12);
  tmp = S_12_14^C_12_13;
  S_13_13 = tmp^(((A>>13)&1) & ((B>>13)&1));
  C_13_13 = (tmp&(((A>>13)&1) & ((B>>13)&1)))|(S_12_14&C_12_13);
  tmp = S_12_15^C_12_14;
  S_13_14 = tmp^(((A>>13)&1) & ((B>>14)&1));
  C_13_14 = (tmp&(((A>>13)&1) & ((B>>14)&1)))|(S_12_15&C_12_14);
  S_13_15 = (((A>>13)&1) & ((B>>15)&1));
  S_14_6 = S_13_7^(((A>>14)&1) & ((B>>6)&1));
  C_14_6 = S_13_7&(((A>>14)&1) & ((B>>6)&1));
  tmp = S_13_8^C_13_7;
  S_14_7 = tmp^(((A>>14)&1) & ((B>>7)&1));
  C_14_7 = (tmp&(((A>>14)&1) & ((B>>7)&1)))|(S_13_8&C_13_7);
  tmp = S_13_9^C_13_8;
  S_14_8 = tmp^(((A>>14)&1) & ((B>>8)&1));
  C_14_8 = (tmp&(((A>>14)&1) & ((B>>8)&1)))|(S_13_9&C_13_8);
  tmp = S_13_10^C_13_9;
  S_14_9 = tmp^(((A>>14)&1) & ((B>>9)&1));
  C_14_9 = (tmp&(((A>>14)&1) & ((B>>9)&1)))|(S_13_10&C_13_9);
  tmp = S_13_11^C_13_10;
  S_14_10 = tmp^(((A>>14)&1) & ((B>>10)&1));
  C_14_10 = (tmp&(((A>>14)&1) & ((B>>10)&1)))|(S_13_11&C_13_10);
  tmp = S_13_12^C_13_11;
  S_14_11 = tmp^(((A>>14)&1) & ((B>>11)&1));
  C_14_11 = (tmp&(((A>>14)&1) & ((B>>11)&1)))|(S_13_12&C_13_11);
  tmp = S_13_13^C_13_12;
  S_14_12 = tmp^(((A>>14)&1) & ((B>>12)&1));
  C_14_12 = (tmp&(((A>>14)&1) & ((B>>12)&1)))|(S_13_13&C_13_12);
  tmp = S_13_14^C_13_13;
  S_14_13 = tmp^(((A>>14)&1) & ((B>>13)&1));
  C_14_13 = (tmp&(((A>>14)&1) & ((B>>13)&1)))|(S_13_14&C_13_13);
  tmp = S_13_15^C_13_14;
  S_14_14 = tmp^(((A>>14)&1) & ((B>>14)&1));
  C_14_14 = (tmp&(((A>>14)&1) & ((B>>14)&1)))|(S_13_15&C_13_14);
  S_14_15 = (((A>>14)&1) & ((B>>15)&1));
  S_15_5 = S_14_6^(((A>>15)&1) & ((B>>5)&1));
  C_15_5 = S_14_6&(((A>>15)&1) & ((B>>5)&1));
  tmp = S_14_7^C_14_6;
  S_15_6 = tmp^(((A>>15)&1) & ((B>>6)&1));
  C_15_6 = (tmp&(((A>>15)&1) & ((B>>6)&1)))|(S_14_7&C_14_6);
  tmp = S_14_8^C_14_7;
  S_15_7 = tmp^(((A>>15)&1) & ((B>>7)&1));
  C_15_7 = (tmp&(((A>>15)&1) & ((B>>7)&1)))|(S_14_8&C_14_7);
  tmp = S_14_9^C_14_8;
  S_15_8 = tmp^(((A>>15)&1) & ((B>>8)&1));
  C_15_8 = (tmp&(((A>>15)&1) & ((B>>8)&1)))|(S_14_9&C_14_8);
  tmp = S_14_10^C_14_9;
  S_15_9 = tmp^(((A>>15)&1) & ((B>>9)&1));
  C_15_9 = (tmp&(((A>>15)&1) & ((B>>9)&1)))|(S_14_10&C_14_9);
  tmp = S_14_11^C_14_10;
  S_15_10 = tmp^(((A>>15)&1) & ((B>>10)&1));
  C_15_10 = (tmp&(((A>>15)&1) & ((B>>10)&1)))|(S_14_11&C_14_10);
  tmp = S_14_12^C_14_11;
  S_15_11 = tmp^(((A>>15)&1) & ((B>>11)&1));
  C_15_11 = (tmp&(((A>>15)&1) & ((B>>11)&1)))|(S_14_12&C_14_11);
  tmp = S_14_13^C_14_12;
  S_15_12 = tmp^(((A>>15)&1) & ((B>>12)&1));
  C_15_12 = (tmp&(((A>>15)&1) & ((B>>12)&1)))|(S_14_13&C_14_12);
  tmp = S_14_14^C_14_13;
  S_15_13 = tmp^(((A>>15)&1) & ((B>>13)&1));
  C_15_13 = (tmp&(((A>>15)&1) & ((B>>13)&1)))|(S_14_14&C_14_13);
  tmp = S_14_15^C_14_14;
  S_15_14 = tmp^(((A>>15)&1) & ((B>>14)&1));
  C_15_14 = (tmp&(((A>>15)&1) & ((B>>14)&1)))|(S_14_15&C_14_14);
  S_15_15 = (((A>>15)&1) & ((B>>15)&1));
  S_16_4 = S_15_5;
  S_16_5 = S_15_6^C_15_5;
  C_16_5 = S_15_6&C_15_5;
  tmp = S_15_7^C_16_5;
  S_16_6 = tmp^C_15_6;
  C_16_6 = (tmp&C_15_6)|(S_15_7&C_16_5);
  tmp = S_15_8^C_16_6;
  S_16_7 = tmp^C_15_7;
  C_16_7 = (tmp&C_15_7)|(S_15_8&C_16_6);
  tmp = S_15_9^C_16_7;
  S_16_8 = tmp^C_15_8;
  C_16_8 = (tmp&C_15_8)|(S_15_9&C_16_7);
  tmp = S_15_10^C_16_8;
  S_16_9 = tmp^C_15_9;
  C_16_9 = (tmp&C_15_9)|(S_15_10&C_16_8);
  tmp = S_15_11^C_16_9;
  S_16_10 = tmp^C_15_10;
  C_16_10 = (tmp&C_15_10)|(S_15_11&C_16_9);
  tmp = S_15_12^C_16_10;
  S_16_11 = tmp^C_15_11;
  C_16_11 = (tmp&C_15_11)|(S_15_12&C_16_10);
  tmp = S_15_13^C_16_11;
  S_16_12 = tmp^C_15_12;
  C_16_12 = (tmp&C_15_12)|(S_15_13&C_16_11);
  tmp = S_15_14^C_16_12;
  S_16_13 = tmp^C_15_13;
  C_16_13 = (tmp&C_15_13)|(S_15_14&C_16_12);
  tmp = S_15_15^C_16_13;
  S_16_14 = tmp^C_15_14;
  C_16_14 = (tmp&C_15_14)|(S_15_15&C_16_13);
  S_16_15 = C_16_14;
  P = 0;
  P |= (S_16_4 & 1) << 20;
  P |= (S_16_5 & 1) << 21;
  P |= (S_16_6 & 1) << 22;
  P |= (S_16_7 & 1) << 23;
  P |= (S_16_8 & 1) << 24;
  P |= (S_16_9 & 1) << 25;
  P |= (S_16_10 & 1) << 26;
  P |= (S_16_11 & 1) << 27;
  P |= (S_16_12 & 1) << 28;
  P |= (S_16_13 & 1) << 29;
  P |= (S_16_14 & 1) << 30;
  P |= (S_16_15 & 1) << 31;
  return P;
}

uint32_t mul16u_HGP(uint16_t A, uint16_t B)
{
  uint32_t P, P_;
  uint16_t tmp, C_12_11,C_12_12,C_12_13,C_12_14,C_13_11,C_13_12,C_13_13,C_13_14,C_14_11,C_14_12,C_14_13,C_14_14,C_15_11,C_15_12,C_15_13,C_15_14,S_11_11,S_11_12,S_11_13,S_11_14,S_11_15,S_12_10,S_12_11,S_12_12,S_12_13,S_12_14,S_12_15,S_13_10,S_13_11,S_13_12,S_13_13,S_13_14,S_13_15,S_13_9,S_14_10,S_14_11,S_14_12,S_14_13,S_14_14,S_14_15,S_14_8,S_14_9,S_15_10,S_15_11,S_15_12,S_15_13,S_15_14,S_15_15,S_15_7,S_15_8,S_15_9,S_16_10,S_16_11,S_16_12,S_16_13,S_16_14,S_16_15,S_16_6,S_16_7,S_16_8,S_16_9;
  S_11_11 = (((A>>11)&1) & ((B>>11)&1));
  S_11_12 = (((A>>11)&1) & ((B>>12)&1));
  S_11_13 = (((A>>11)&1) & ((B>>13)&1));
  S_11_14 = (((A>>11)&1) & ((B>>14)&1));
  S_11_15 = (((A>>11)&1) & ((B>>15)&1));
  S_12_10 = S_11_11;
  S_12_11 = S_11_12^(((A>>12)&1) & ((B>>11)&1));
  C_12_11 = S_11_12&(((A>>12)&1) & ((B>>11)&1));
  S_12_12 = S_11_13^(((A>>12)&1) & ((B>>12)&1));
  C_12_12 = S_11_13&(((A>>12)&1) & ((B>>12)&1));
  S_12_13 = S_11_14^(((A>>12)&1) & ((B>>13)&1));
  C_12_13 = S_11_14&(((A>>12)&1) & ((B>>13)&1));
  S_12_14 = S_11_15^(((A>>12)&1) & ((B>>14)&1));
  C_12_14 = S_11_15&(((A>>12)&1) & ((B>>14)&1));
  S_12_15 = (((A>>12)&1) & ((B>>15)&1));
  S_13_9 = S_12_10;
  S_13_10 = S_12_11;
  tmp = S_12_12^C_12_11;
  S_13_11 = tmp^(((A>>13)&1) & ((B>>11)&1));
  C_13_11 = (tmp&(((A>>13)&1) & ((B>>11)&1)))|(S_12_12&C_12_11);
  tmp = S_12_13^C_12_12;
  S_13_12 = tmp^(((A>>13)&1) & ((B>>12)&1));
  C_13_12 = (tmp&(((A>>13)&1) & ((B>>12)&1)))|(S_12_13&C_12_12);
  tmp = S_12_14^C_12_13;
  S_13_13 = tmp^(((A>>13)&1) & ((B>>13)&1));
  C_13_13 = (tmp&(((A>>13)&1) & ((B>>13)&1)))|(S_12_14&C_12_13);
  tmp = S_12_15^C_12_14;
  S_13_14 = tmp^(((A>>13)&1) & ((B>>14)&1));
  C_13_14 = (tmp&(((A>>13)&1) & ((B>>14)&1)))|(S_12_15&C_12_14);
  S_13_15 = (((A>>13)&1) & ((B>>15)&1));
  S_14_8 = S_13_9;
  S_14_9 = S_13_10;
  S_14_10 = S_13_11;
  tmp = S_13_12^C_13_11;
  S_14_11 = tmp^(((A>>14)&1) & ((B>>11)&1));
  C_14_11 = (tmp&(((A>>14)&1) & ((B>>11)&1)))|(S_13_12&C_13_11);
  tmp = S_13_13^C_13_12;
  S_14_12 = tmp^(((A>>14)&1) & ((B>>12)&1));
  C_14_12 = (tmp&(((A>>14)&1) & ((B>>12)&1)))|(S_13_13&C_13_12);
  tmp = S_13_14^C_13_13;
  S_14_13 = tmp^(((A>>14)&1) & ((B>>13)&1));
  C_14_13 = (tmp&(((A>>14)&1) & ((B>>13)&1)))|(S_13_14&C_13_13);
  tmp = S_13_15^C_13_14;
  S_14_14 = tmp^(((A>>14)&1) & ((B>>14)&1));
  C_14_14 = (tmp&(((A>>14)&1) & ((B>>14)&1)))|(S_13_15&C_13_14);
  S_14_15 = (((A>>14)&1) & ((B>>15)&1));
  S_15_7 = S_14_8;
  S_15_8 = S_14_9;
  S_15_9 = S_14_10;
  S_15_10 = S_14_11;
  tmp = S_14_12^C_14_11;
  S_15_11 = tmp^(((A>>15)&1) & ((B>>11)&1));
  C_15_11 = (tmp&(((A>>15)&1) & ((B>>11)&1)))|(S_14_12&C_14_11);
  tmp = S_14_13^C_14_12;
  S_15_12 = tmp^(((A>>15)&1) & ((B>>12)&1));
  C_15_12 = (tmp&(((A>>15)&1) & ((B>>12)&1)))|(S_14_13&C_14_12);
  tmp = S_14_14^C_14_13;
  S_15_13 = tmp^(((A>>15)&1) & ((B>>13)&1));
  C_15_13 = (tmp&(((A>>15)&1) & ((B>>13)&1)))|(S_14_14&C_14_13);
  tmp = S_14_15^C_14_14;
  S_15_14 = tmp^(((A>>15)&1) & ((B>>14)&1));
  C_15_14 = (tmp&(((A>>15)&1) & ((B>>14)&1)))|(S_14_15&C_14_14);
  S_15_15 = (((A>>15)&1) & ((B>>15)&1));
  S_16_6 = S_15_7;
  S_16_7 = S_15_8;
  S_16_8 = S_15_9;
  S_16_9 = S_15_10;
  S_16_10 = S_15_11;
  P_ = (((C_15_11 & 1)<<0)|((C_15_12 & 1)<<1)|((C_15_13 & 1)<<2)|((C_15_14 & 1)<<3)) + (((S_15_12 & 1)<<0)|((S_15_13 & 1)<<1)|((S_15_14 & 1)<<2)|((S_15_15 & 1)<<3));
  S_16_11 = (P_ >> 0) & 1;
  S_16_12 = (P_ >> 1) & 1;
  S_16_13 = (P_ >> 2) & 1;
  S_16_14 = (P_ >> 3) & 1;
  S_16_15 = (P_ >> 4) & 1;
  P = 0;
  P |= (S_16_6 & 1) << 22;
  P |= (S_16_7 & 1) << 23;
  P |= (S_16_8 & 1) << 24;
  P |= (S_16_9 & 1) << 25;
  P |= (S_16_10 & 1) << 26;
  P |= (S_16_11 & 1) << 27;
  P |= (S_16_12 & 1) << 28;
  P |= (S_16_13 & 1) << 29;
  P |= (S_16_14 & 1) << 30;
  P |= (S_16_15 & 1) << 31;
  return P;
}

uint32_t mul16u_HGY(uint16_t A, uint16_t B)
{
 return ((A >> 15) * (B >> 15)) << (2*15);
}













//-------------------------------------------------
uint64_t mul16u_exact_t1(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000000001;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t2(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000000003;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t3(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000000007;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t4(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000000000f;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t5(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000000001f;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t6(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000000003f;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t7(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000000007f;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t8(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000000000ff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t9(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000000001ff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t10(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000000003ff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t11(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000000007ff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t12(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000000fff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t13(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000001fff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t14(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000003fff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t15(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000007fff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t16(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000000ffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t17(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000001ffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t18(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000003ffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t19(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000007ffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t20(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000000fffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t21(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000001fffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t22(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000003fffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t23(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000007fffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t24(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000000ffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t25(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000001ffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t26(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000003ffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t27(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000007ffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t28(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000000fffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t29(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000001fffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t30(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000003fffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t31(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000007fffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t32(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000000ffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t33(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000001ffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t34(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000003ffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t35(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000007ffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t36(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000000fffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t37(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000001fffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t38(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000003fffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t39(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000007fffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t40(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000000ffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t41(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000001ffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t42(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000003ffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t43(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000007ffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t44(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00000fffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t45(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00001fffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t46(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00003fffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t47(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00007fffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t48(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0000ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t49(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0001ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t50(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0003ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t51(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0007ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t52(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x000fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t53(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x001fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t54(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x003fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t55(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x007fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t56(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x00ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t57(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x01ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t58(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x03ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t59(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x07ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t60(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x0fffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t61(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x1fffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t62(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x3fffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t63(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0x7fffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t mul16u_exact_t64(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a*b;
  tmp_mask = 0xffffffffffffffff;
  return tmp & tmp_mask;
}

uint64_t adder_16bit_t1(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000000001;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t2(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000000003;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t3(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000000007;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t4(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000000000f;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t5(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000000001f;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t6(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000000003f;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t7(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000000007f;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t8(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000000000ff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t9(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000000001ff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t10(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000000003ff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t11(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000000007ff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t12(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000000fff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t13(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000001fff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t14(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000003fff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t15(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000007fff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t16(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000000ffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t17(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000001ffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t18(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000003ffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t19(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000007ffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t20(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000000fffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t21(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000001fffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t22(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000003fffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t23(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000007fffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t24(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000000ffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t25(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000001ffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t26(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000003ffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t27(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000007ffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t28(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000000fffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t29(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000001fffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t30(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000003fffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t31(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000007fffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t32(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000000ffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t33(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000001ffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t34(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000003ffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t35(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000007ffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t36(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000000fffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t37(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000001fffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t38(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000003fffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t39(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000007fffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t40(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000000ffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t41(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000001ffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t42(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000003ffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t43(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000007ffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t44(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00000fffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t45(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00001fffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t46(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00003fffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t47(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00007fffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t48(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0000ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t49(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0001ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t50(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0003ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t51(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0007ffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t52(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x000fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t53(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x001fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t54(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x003fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t55(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x007fffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t56(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x00ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t57(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x01ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t58(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x03ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t59(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x07ffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t60(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x0fffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t61(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x1fffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t62(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x3fffffffffffffff;
  return tmp & tmp_mask;
}
uint64_t adder_16bit_t63(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0x7fffffffffffffff;
  return tmp & tmp_mask;
}

uint64_t adder_16bit_t64(uint64_t a,uint64_t b){
  uint64_t tmp,tmp_mask;
  tmp =a+b;
  tmp_mask = 0xffffffffffffffff;
  return tmp & tmp_mask;
}