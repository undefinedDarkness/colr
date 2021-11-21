build: src/*.c
	c99 src/*.c -Wall -Os -o colr `pkg-config --cflags --libs gtk+-3.0 x11` 
