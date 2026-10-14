# imageviewer.c

A minimal X11 image viewer for binary PPM (P6) images.

## Features
- Reads binary PPM (P6) files.
- Displays the image centered in a full-screen window.
- Press any key to exit.

## Dependencies
- X11 development libraries (libX11)
- C compiler (e.g., gcc)
- A P6 PPM image file

## Build
```sh
gcc imageviewer.c -o imageviewer -lX11
