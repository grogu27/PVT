#!/bin/sh

gcc -g -Wall -lm -o dgemm ./dgemm.c -lrt
gcc -g -Wall -lm -o dgemm-unroll ./dgemm-unroll.c -lrt

#gcc -Ofast -march=native -fprefetch-loop-arrays -g -Wall -lm -o dgemm-unroll ./dgemm-unroll.c

