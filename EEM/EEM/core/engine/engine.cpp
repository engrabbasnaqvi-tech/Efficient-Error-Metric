#include <cstdint>
#include <cstring>
#include "components.h"
typedef uint32_t (*mul_fn)(uint16_t,uint16_t);
typedef uint64_t (*add_fn)(uint64_t,uint64_t);
static uint32_t mul_exact(uint16_t a,uint16_t b){return (uint32_t)a*(uint32_t)b;}
struct MU{const char*n;mul_fn f;}; struct AU{const char*n;add_fn f;};
static MU MT[]={{"mul16u_exact",mul_exact},{"mul16u_BMC",mul16u_BMC},{"mul16u_AQ1",mul16u_AQ1},
 {"mul16u_5FA",mul16u_5FA},{"mul16u_DAE",mul16u_DAE},{"mul16u_F6B",mul16u_F6B},{"mul16u_CK3",mul16u_CK3},
 {"mul16u_8VH",mul16u_8VH},{"mul16u_GPF",mul16u_GPF},{"mul16u_HGP",mul16u_HGP},{"mul16u_HGY",mul16u_HGY}};
static AU AT[]={{"adder_16bit",adder_16bit},{"add16u_0AV",add16u_0AV},{"add16u_0EM",add16u_0EM},
 {"add16u_0Q7",add16u_0Q7},{"add16u_073",add16u_073},{"add16u_0M0",add16u_0M0},{"add16u_0DL",add16u_0DL},
 {"add16u_0GK",add16u_0GK},{"add16u_02E",add16u_02E},{"add16u_0MH",add16u_0MH}};
extern "C" void run_mul(const char*name,const uint16_t*a,const uint16_t*b,uint32_t*o,long n){
  for(auto&u:MT) if(!strcmp(name,u.n)){for(long i=0;i<n;i++)o[i]=u.f(a[i],b[i]);return;}
  for(long i=0;i<n;i++)o[i]=0xFFFFFFFF;
}
extern "C" void run_add(const char*name,const uint64_t*a,const uint64_t*b,uint64_t*o,long n){
  for(auto&u:AT) if(!strcmp(name,u.n)){for(long i=0;i<n;i++)o[i]=u.f(a[i],b[i]);return;}
  for(long i=0;i<n;i++)o[i]=~0ull;
}
