all: display

display: display.c
	gccx display.c -o display `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm display
