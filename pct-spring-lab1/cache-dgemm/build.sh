#!/bin/sh

<<<<<<< HEAD
gcc -g -Wall -lm -o dgemm ./dgemm.c -lrt
gcc -g -Wall -lm -o dgemm-unroll ./dgemm-unroll.c -lrt
=======
gcc -g -Wall -lm -o dgemm ./dgemm.c
gcc -g -Wall -lm -o dgemm-unroll ./dgemm-unroll.c
>>>>>>> 71a2be0245402c1e167a02db659d9cfac6b1a93d

#gcc -Ofast -march=native -fprefetch-loop-arrays -g -Wall -lm -o dgemm-unroll ./dgemm-unroll.c

