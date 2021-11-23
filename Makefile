imports = `pkg-config --cflags --libs gtk+-3.0 x11`
options = -Wall -Wno-deprecated-declarations

build-no-x11: src/*.c
	cc src/*.c -o colr '-DSCREENSHOT_PROGRAM="$(exe)"' $(imports) $(options)

build: src/*.c
	cc src/*.c -o colr $(imports) $(options) 
