all: main.o utils.o globals.o
	gcc -o main_exec main.o utils.o -g
 
main.o: main.c
	gcc -c main.c -g

utils.o: utils.c
	gcc -c utils.c -g

globals.o: globals.c:
	gcc -c globals.c -g

clean:
	rm -f *.o main