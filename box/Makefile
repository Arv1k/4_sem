COMP = gcc
FLAGS = -c -Wall -pedantic -g
RES = cd resources &&


all: main.o list.o vector.o
	$(RES) $(COMP) main.o list.o vector.o -o main.out
	$(RES) mv main.out ../

main.o: main.c
	$(COMP) $(FLAGS) main.c
	mv main.o resources/

list.o: list/list.c
	$(COMP) $(FLAGS) list/list.c
	mv list.o resources/

vector.o: vector/vector.c
	$(COMP) $(FLAGS) vector/vector.c
	mv vector.o resources/

clean:
	$(RES) rm -rf *.o
	rm -rf *.out
