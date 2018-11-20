.PHONY: all clean

CC = gcc
CFLAGS = -O0 -g -Wall -Werror -std=gnu99

all: mergesort_wihout_const #reverse sort_arr quicksort_without_const

reverse: reverse.o

sort_arr: sort_arr.o

quicksort_without_const: quicksort_without_const.o

mergesort_wihout_const: mergesort_wihout_const.o

clean:
	rm -f *.o reverse sort_arr quicksort_without_const mergesort_wihout_const
