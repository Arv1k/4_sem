CC 	  = gcc
FLAGS = -c -Wall -pedantic -g
VPATH = list:vector:resources
FILES = resources/main.o resources/list.o resources/vector.o

all: main.out

main.out: $(FILES)
	$(CC) $(FILES) -o $@

resources/%.o: %.c
	$(CC) $(FLAGS) $< -o $@

clean:
	@cd resources && rm -rf *.o
	@rm -rf *.o
	@rm -rf *.out
