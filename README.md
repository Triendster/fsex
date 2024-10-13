# README for `fsex`

## Project Overview

`fsex` is a terminal-based filesystem explorer written in C, designed for simplicity and minimalism. It uses the `ncurses` library to provide a navigable interface for exploring directories in the terminal. Users can navigate up and down directories and open files using a minimal, text-based interface.

## Build Instructions

To build the `fsex` program, you will need the following tools installed:
- **GCC (GNU Compiler Collection)**: for compiling the C source code.
- **ncurses library**: for terminal UI handling.

### Compilation Steps

1. Clone the repository and navigate to its directory:
   ```bash
   git clone https://github.com/Triendster/fsex.git
   cd fsex
   ```

2. Build the program using the provided Makefile:
   ```bash
   make
   ```

   This will:
   - Compile the `fsex.c` source file into an object file (`fsex.o`).
   - Link the object file with the `ncurses` library to produce the final executable `fsex`.

### Clean Up

To clean up the build files (i.e., remove the object files and the executable), run:

```bash
make clean
```

This will remove the `fsex.o` object file and the `fsex` executable.

## Usage

Once compiled, you can run the `fsex` program in your terminal by typing:

```bash
./fsex [options] <directory>
```

For example, to launch `fsex` in the `/home` directory:

```bash
./fsex /home
```

### Options

- **-i**: Ignore hidden files (starting with `.`) in directory listings.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contributing

Feel free to submit issues and pull requests to improve the functionality or extend the features of the project.
