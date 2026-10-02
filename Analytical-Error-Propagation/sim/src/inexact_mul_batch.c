#include <stdint.h>
#include <stdio.h>

#include UNIT_SRC

int main(void) {
    uint64_t pair[2];
    while (fread(pair, sizeof(uint64_t), 2, stdin) == 2) {
        uint64_t product = (uint64_t)UNIT_FN((uint16_t)pair[0], (uint16_t)pair[1]);
        fwrite(&product, sizeof(uint64_t), 1, stdout);
    }
    return 0;
}
