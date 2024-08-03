# Target to build the executable
all: main_exec

# Rule to link object files into the executable
main_exec: main.o utils.o pre_assembler.o BST.o
	gcc -o main_exec main.o utils.o pre_assembler.o BST.o -g

# Rules to compile source files into object files
main.o: main.c
	gcc -c main.c -g

utils.o: utils.c
	gcc -c utils.c -g

pre_assembler.o: pre_assembler.c
	gcc -c pre_assembler.c -g

BST.o: BST.c
	gcc -c BST.c -g

# Rule to clean up generated files
clean:
	rm -f *.o