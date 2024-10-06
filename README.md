<p align="center">
<img src="https://i.imgur.com/QudIqH1.png" /><br>
A simple color picker GUI for X11.
</p>

## Motivation
I just needed something pretty for my rice and I didn't want to pull out a terminal app and I really liked the design of M$ PowerToys's Colour Picker so I created this, It ain't perfect but it's good enough in my opinion

## Install & Usage
Obtain the binary package from the `./binaries/` folder:
To install the deb package, run `sudo apt install ./*.deb`
To use it, run: `colr`

## Building

```sh
$ git clone https://github.com/undefinedDarkness/colr.git 
$ cd colr
$ meson build
$ cd build && meson compile
$ mv colr ~/.local/bin # Move to path
```

Building Requirements:
- Gtk3 Development Headers (libgtk-3-dev)
- X11  Development Headers (libx11-dev)
- Meson & Ninja

## Todo
- Extend HSV Support
- Fix Clipboard Persistence - Not easily fixed
- Restyle About Dialog
