.PHONY: all clean

CC = gcc
CFLAGS = -O0 -g -Wall  -std=gnu99

all: merge #mergesort_wihout_const reverse sort_arr quicksort_without_const

reverse: reverse.o

sort_arr: sort_arr.o

quicksort_without_const: quicksort_without_const.o

mergesort_wihout_const: mergesort_wihout_const.o

merge: merge.o

clean:
	rm -f *.o reverse sort_arr quicksort_without_const mergesort_wihout_const merge
