all: solver

solver: solver.c
	gccx solver.c -o solver `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm solver
