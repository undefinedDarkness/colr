build: *.c
	c99 *.c -Wall -g -Wextra `pkg-config --cflags --libs gtk+-3.0 x11` 
