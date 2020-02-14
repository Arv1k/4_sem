CC = gcc
FLAGS = -c -Wall -pedantic -g
VPATH = list:vector:resources

all: main.out

main.out: resources/main.o resources/list.o resources/vector.o
	$(CC) resources/main.o resources/list.o resources/vector.o -o main.out

resources/%.o: %.c
	$(CC) $(FLAGS) $< -o $@

clean:
	cd resources && rm -rf *.o
	rm -rf *.o
	rm -rf *.out
