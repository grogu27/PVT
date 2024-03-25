#!/bin/sh

gcc DGEMV.c -o prog -fopenmp -O2 -std=c99 -Wall
