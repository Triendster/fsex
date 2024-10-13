# Variables
CC = gcc                       # Compiler
CFLAGS = -Wall                 # Compilation flags
LDFLAGS = -lncurses            # Link the ncurses library

# Target to build the program
all: fsex

# Build the fsex executable
fsex: fsex.o
	$(CC) -o fsex fsex.o $(LDFLAGS)

# Compile the object file
fsex.o: fsex.c
	$(CC) -c fsex.c $(CFLAGS)

# Clean up build files
clean:
	rm -f *.o fsex
