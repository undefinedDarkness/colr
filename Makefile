imports = `pkg-config --cflags --libs gtk+-3.0 x11`
options = -Wall -Wno-deprecated-declarations  # -fsanitize=address

build: src/*.c
	cc src/*.c src/**/*.c -o colr -g $(imports) $(options) 

build-no-x11: src/*.c
	cc src/*.c src/**/*.c -o colr '-DSCREENSHOT_PROGRAM="$(exe)"' $(imports) $(options)
