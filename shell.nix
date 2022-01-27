{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell {
  buildInputs = [ pkg-config gtk3 xorg.libX11 ];
}
