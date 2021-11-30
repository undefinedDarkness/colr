imports = `pkg-config --cflags --libs gtk+-3.0 x11` -lm
options = -Wall -Wno-deprecated-declarations 
linker-opts=-Iresources/
.DEFAULT_GOAL := build

ifneq ($(exe),)
linker-opts += '-DSCREENSHOT_PROGRAM="$(exe)"'
endif

build: resource src/*.c
	cc src/*.c src/**/*.c resources/*.c -o colr $(linker-opts) $(imports) $(options)

.ONESHELL:
resource: resources/*
	cd resources
	glib-compile-resources index.xml --generate-source --target=resources.c
	glib-compile-resources index.xml --generate-header --target=resources.h
