![](https://i.imgur.com/QudIqH1.png)
A simple color picker GUI for X11.

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
Gtk3 Development Headers (libgtk-3-dev)
X11  Development Headers (libx11-dev)
Meson & Ninja

## Todo
- Extend HSV Support
- Fix Clipboard Persistence
- Restyle About Dialog
