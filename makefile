# Basic compilation macros
CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic # Flags
LDFLAGS = -g # Linker flags for debugging

# Object files
OBJS = assembler.o pre_assembler.o BST.o code.o fpass.o spass.o instructions.o table.o utils.o write_files.o

## Executable
assembler: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(CFLAGS) -o $@

## Main:
assembler.o: assembler.c globals.h
	$(CC) -c assembler.c $(CFLAGS) -o $@

## Pre_assembler:
pre_assembler.o: pre_assembler.c pre_assembler.h globals.h
	$(CC) -c pre_assembler.c $(CFLAGS) -o $@

## Binary Search Tree:
BST.o: BST.c BST.h globals.h
	$(CC) -c BST.c $(CFLAGS) -o $@

## Code helper functions:
code.o: code.c code.h globals.h
	$(CC) -c code.c $(CFLAGS) -o $@

## First Pass:
fpass.o: first_pass.c first_pass.h globals.h
	$(CC) -c first_pass.c $(CFLAGS) -o $@

## Second Pass:
spass.o: second_pass.c second_pass.h globals.h
	$(CC) -c second_pass.c $(CFLAGS) -o $@

## Instructions helper functions:
instructions.o: instructions.c instructions.h globals.h
	$(CC) -c instructions.c $(CFLAGS) -o $@

## Table:
table.o: table.c table.h globals.h
	$(CC) -c table.c $(CFLAGS) -o $@

## Useful functions:
utils.o: utils.c utils.h globals.h
	$(CC) -c utils.c $(CFLAGS) -o $@

## Output Files:
write_files.o: write_files.c write_files.h globals.h
	$(CC) -c write_files.c $(CFLAGS) -o $@

# Clean Target (remove leftovers)
clean:
	rm -f *.o
