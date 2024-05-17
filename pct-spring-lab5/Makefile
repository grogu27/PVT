.PHONY: all graph run clean

all: prog

prog: main.c
	gcc main.c -o prog -fopenmp

graph:
	gnuplot graph.gp

run: prog
	./prog

clean:
	rm -f prog && rm -f graph.pdf && rm -f res.txt