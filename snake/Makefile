CC=g++
CFLAGS=-g -Wall -c
SRCS:=$(wildcard *.cpp)
#OBJS=$(SRCS:.cpp=.o)
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
EXEC=snake

all:$(SRCS) $(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o snake
