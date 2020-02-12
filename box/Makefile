COMP = gcc
FLAGS = -c -Wall -pedantic -g
RES = cd resources &&


all: main.o list.o vector.o
	$(RES) $(COMP) main.o list.o vector.o -o main.out
	$(RES) mv main.out ../

main.o: main.c
	$(COMP) $(FLAGS) main.c -o resources/main.o

list.o: list/list.c
	$(COMP) $(FLAGS) list/list.c -o resources/list.o

vector.o: vector/vector.c
	$(COMP) $(FLAGS) vector/vector.c -o resources/vector.o

clean:
	$(RES) rm -rf *.o
	rm -rf *.out
