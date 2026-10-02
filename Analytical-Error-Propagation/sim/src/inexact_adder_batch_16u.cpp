#include <cstdint>
#include <cstdio>
#include <cstdlib>
using namespace std;

// BINARY-I/O counterpart of ../sim/inexact_adder_batch_16u.cpp.
// Same approximate-adder functions, same lib code -> implementation mapping —
// compiled against the SAME, unmodified ../sim/components_16u.cpp (single
// source of truth for the approximation logic; nothing duplicated here).
// Only the I/O layer changed: raw uint64_t in/out instead of decimal text.
//
// lib code -> implementation mapping (identical to sim/inexact_adder_batch_16u.cpp):
//   0  = exact (a + b)
//   2  = add16u_0TA  (= add16u_0AV  mae=0.8   mse=3.0)
//   3  = add16u_0GN  (= add16u_0EM  mae=2.4   mse=8.5)
//   4  = add16u_0BC  (= add16u_0Q7  mae=8.3   mse=96)
//   5  = add16u_0P8  (= add16u_073  mae=27    mse=1136)
//   6  = add16u_0HE  (= add16u_0M0  mae=75    mse=8209)
//   7  = add16u_0KG  (= add16u_0DL  mae=262   mse=92039)
//   8  = add16u_0KU  (= add16u_0GK  mae=1187  mse=2051554.5)
//   9  = add16u_0SL  (= add16u_02E  mae=4619  mse=30582328)
//   10 = add16u_067  (= add16u_0MH  mae=12976 mse=253581030)

uint64_t add16u_0TA(uint64_t a, uint64_t b);
uint64_t add16u_0GN(uint64_t a, uint64_t b);
uint64_t add16u_0BC(uint64_t a, uint64_t b);
uint64_t add16u_0P8(uint64_t a, uint64_t b);
uint64_t add16u_0HE(uint64_t a, uint64_t b);
uint64_t add16u_0KG(uint64_t a, uint64_t b);
uint64_t add16u_0KU(uint64_t a, uint64_t b);
uint64_t add16u_0SL(uint64_t a, uint64_t b);
uint64_t add16u_067(uint64_t a, uint64_t b);

// Reads raw uint64_t (a, b) pairs from stdin, writes raw uint64_t results to stdout.
// Usage: ./inexact_adder_batch_16u <code>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./inexact_adder_batch_16u <code>\n");
        return 1;
    }
    int code = atoi(argv[1]);
    uint64_t pair[2];
    uint64_t result;
    while (fread(pair, sizeof(uint64_t), 2, stdin) == 2) {
        uint64_t a = pair[0], b = pair[1];
        switch (code) {
            case 0: result = a + b; break;
            case 2: result = add16u_0TA(a, b); break;
            case 3: result = add16u_0GN(a, b); break;
            case 4: result = add16u_0BC(a, b); break;
            case 5: result = add16u_0P8(a, b); break;
            case 6: result = add16u_0HE(a, b); break;
            case 7: result = add16u_0KG(a, b); break;
            case 8: result = add16u_0KU(a, b); break;
            case 9: result = add16u_0SL(a, b); break;
            case 10: result = add16u_067(a, b); break;
            default:
                fprintf(stderr, "Unknown code: %d\n", code);
                return 1;
        }
        fwrite(&result, sizeof(uint64_t), 1, stdout);
    }
    return 0;
}
