# Image Viewer (BMP 24-bit)

A minimal image viewer for 24‑bit BMP files, built with C and SDL2. This project demonstrates low‑level image parsing, pixel manipulation, and window rendering using the SDL2 library.

## Dependencies

- **SDL2** – Simple DirectMedia Layer for window creation and graphics.
- **gcc** – GNU Compiler Collection.

## Installing SDL2

### Ubuntu / Debian / Linux Mint

```bash
sudo apt update
sudo apt install libsdl2-dev
```

### Fedora / RHEL / CentOS

```bash
sudo dnf install SDL2-devel
```
### Arch Linux / Manjaro
```bash
sudo pacman -S sdl2
```
### macOS (Homebrew)
```bash
brew install sdl2
```
### Windows (MSYS2 / MinGW)
```bash
pacman -S mingw-w64-x86_64-SDL2
```
After installation, the SDL2 headers (SDL2/SDL.h) will be placed in a standard system include path (e.g., /usr/include/SDL2). The library files (.so or .dll) will be installed in the corresponding library directory.

## Compilation
Use sdl2-config to automatically retrieve the correct compiler and linker flags. Open a terminal in the project directory and run:

```bash
gcc iv.c -o ./iv `sdl2-config --cflags --libs`
```
Note the backticks: `sdl2-config --cflags --libs` – they execute the sdl2-config tool and insert its output into the gcc command line. The --cflags part gives the include path for SDL2 headers, and --libs provides the linker flags for the SDL2 library.

If the command succeeds, an executable named iv will be created in the current directory.

## Usage
Run the viewer with a 24‑bit BMP file as argument:

bash
./iv blackbuck.bmp
The program will open a window displaying the image. Press ESC or close the window to exit.

## Supported BMP Features (Version 1)
24‑bit uncompressed BMP files.

Bottom‑up pixel order (automatically flipped).

Row padding (handles correct byte alignment).

Basic error detection for missing file or invalid signature.

## Limitations
Only 24‑bit BMP files without compression are supported.

No support for 1‑bit, 4‑bit, 8‑bit, 16‑bit, or 32‑bit BMP.

No support for RLE‑compressed BMP.

Performance is not optimised (per‑pixel SDL_FillRect is used for simplicity).

## Future improvements
Use raw system calls (open, read, mmap) instead of fopen/fread.

Support more BMP variants and JPEG.

Add quadtree‑based progressive rendering.

Direct rendering to Linux framebuffer (/dev/fb0).

## Troubleshooting
sdl2-config: command not found – SDL2 development package is not installed or not in PATH. Re‑install libsdl2-dev.

undefined reference to 'SDL_CreateWindow' – Linker flags are missing. Ensure you use the backticks exactly as shown.

Window opens but no image – The BMP file might not be 24‑bit or may use compression not supported in this version.

error: SDL2/SDL.h: No such file – The SDL2 headers are not in the include path. Verify installation and try adding -I/usr/include/SDL2 manually.

