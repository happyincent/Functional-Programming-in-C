.PHONY: all clean

CC = gcc
CFLAGS = -O0 -g -Wall -Werror -std=gnu99

all: reverse mergesort

reverse: reverse.o

mergesort: mergesort.o

clean:
	rm -f *.o reverse mergesort
