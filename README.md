# YAscii

**Status:** Work in progress: currently supports PNG only (any bit depth and color type).  
**License:** GNU General Public License v3.0.

YAscii is a small C program that converts PNG images into Unicode-based ASCII art, designed for use in terminal utilities like `fastfetch` or `neofetch`, and for generating ASCII wallpapers.  
It uses a separable Lanczos convolution filter to downscale images and then maps pixel brightness to a palette of Unicode symbols.

## Features (current)

- Loads PNG images of any bit depth and color type (via libpng).
- Downscales images using a separable Lanczos convolution filter.
- Converts 8-bit RGBA pixels to greyscale luminance values.
- Maps luminance to a wide-character palette, including Unicode Braille symbols.
- Multiple ASCII palettes.

## Planned Features 
- Support for JPEG/JPG input.
- ANSI color output for richer terminal rendering.
- Additional convolution filters.
- Both a TUI interface and a simple CLI command.

## Requirements

- GCC or a compatible C11 compiler  
- make build tool  
- [libpng](http://www.libpng.org/pub/png/libpng.html) development library  
- [zlib](https://zlib.net/) development library  

⚠ Some additional dependencies might be required depending on your system configuration (headers and pkg-config files for libpng/zlib must be available).

## Build

```bash
make
```

This will produce the executable YAscii in the repository root.

## Usage

Basic usage with a single PNG file path:

```bash
./YAscii path/to/image.png
```

Optional parameters:

You can choose a palette with `-p` or `--palette` followed by one of the names or aliases below:
If no palette is specified, **BRAILLE** is used by default

| Palette  | Aliases            | Characters                        |
|----------|--------------------|------------------------------------|
| BRAILLE  | `br`, `braille`    | ⣿⣿⣾⣷⣧⣇⣃⣂⡇⡃⡂⠇⠆⠄⠂⠁⠀ |
| BLOCK    | `bl`, `block`      | █▓▒░                              |
| DENSE    | `ds`, `dense`      | @%#*+=-:.                          |
| SMOOTH   | `sm`, `smooth`     | .,:;+*?0S#@                        |
  Example:

```bash
  ./YAscii path/to/image.png -p braille
  ```

**-s / --scale**  
Sets the scale factor (integer).  

Example:
```bash
./YAscii path/to/image.png -s 2
```

Any unknown option or missing value will result in an error and program termination.



Usage examples:
```bash
./YAscii path/to/image.png -p block -s 2
./YAscii path/to/image.png --palette dense --scale 5 
```

## Contributing

Pull requests and issue reports are welcome. Please format code with gcc warnings enabled.

## License
This project is licensed under the GNU General Public License v3.0. See the LICENSE file for details.

```text
▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓     ▓▓▓▓▓▒     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓     ▓▓▓▓▓▓     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓     ▓▓▓▓▓▓     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ░░░░░      ░░░░░   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ░░░░░      ░░░░░   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒  ▒▒▒▒▒      ▒▒▒▒▒   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ░░▓▓▓▓▓▒░░░░▒▓▓▓▓▓░░░  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ░░▓▓▓▓▓▒░░░░▒▓▓▓▓▓░░░  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  █▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓▓  █▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓      ▓▓                ▓▓▓     ▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓      ▓▓                ▓▓▓     ▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓█▓      ██                ██▓     █▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓█  ░░░░░░  ░░░░░░░░░░░░░░░░   ░░░░░  ▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓  ░░░░░░  ░░░░░░░░░░░░░░░░   ░░░░░  █▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ░░░░░   ░░░░░░░░░░░░░░░░░░░░░  ░░░░░   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ░░░░░   ░░░░░░░░░░░░░░░░░░░░░  ░░░░░   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ░░░░░   ░░░░░░░░░░░░░░░░░░░░░  ░░░░░   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░  ░░░░░░  ░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░  ░░░░░░  ░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓   ▓▓   ░░░░░░░░░░░░░░░░░░░░░░░░░░   ▓▓   ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓        ░░░░░░░░░░░░░░░░░░░░░░░░░░        ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓        ░░░░░░░░░░░░░░░░░░░░░░░░░░        ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓        ░░░░░░░░░░░░░░░░░░░░░░░░░░        ▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓  ▓▓▓   ░░░░░░░░░░░░░░░░░░░░░  ▓▓█  █▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓  ▓▓▓   ░░░░░░░░░░░░░░░░░░░░░  ▓▓█  █▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓  ▒▓▓   ░░░░░░░░░░░░░░░░░░░░░  ▓▓▓  █▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓        ░░░░░░░░░░░░░░░░        ▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓        ░░░░░░░░░░░░░░░░        ▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓                     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓                     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓                     ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
```
