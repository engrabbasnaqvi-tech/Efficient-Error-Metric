#!/bin/sh
set -e
cd "$(dirname "$0")"
mkdir -p bin
g++ -O2 -o bin/inexact_adder_batch_16u src/inexact_adder_batch_16u.cpp src/components_16u.cpp
code=13
for unit in AQ1 5FA DAE F6B CK3 8VH GPF HGP HGY; do
    gcc -O2 -DUNIT_SRC="\"evoapprox/mul16u_$unit.c\"" -DUNIT_FN=mul16u_$unit \
        -o bin/inexact_mul_batch_$code src/inexact_mul_batch.c
    code=$((code + 1))
done
echo "built sim/bin"
