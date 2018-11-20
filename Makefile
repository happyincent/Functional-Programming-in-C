.PHONY: all clean

CC = gcc
CFLAGS = -O0 -g -Wall -Werror -std=gnu99

all: mergesort_wihout_const #reverse sort_arr sort_without_const sort_without_const1

reverse: reverse.o

sort_arr: sort_arr.o

sort_without_const: sort_without_const.o

sort_without_const1: sort_without_const1.o

mergesort_wihout_const: mergesort_wihout_const.o

clean:
	rm -f *.o reverse sort_arr sort_without_const sort_without_const1 mergesort_wihout_const
