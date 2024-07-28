all: main.o utils.o
	gcc -o main main.o utils.o -g
 
main.o: main.c
	gcc -c main.c -g

utils.o: utils.c
	gcc -c utils.c -g

clean:
	rm -f *.o main