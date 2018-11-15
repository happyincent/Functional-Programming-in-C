#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* integer linked list type */
typedef struct __int_list const *const int_list_t;
static int_list_t const Nil = NULL; /* empty list */

/* singly linked list element */
typedef struct __int_list {
    int_list_t next;
    int32_t const val;
} node_t;

/* A pointer to where we are about to store the result of computation */
typedef void *const CPS_Result;

/* prototypes for the continuations */
typedef void (*MakeListCallback)(int_list_t list, CPS_Result res);
void make_list(uint32_t const arr_size, int32_t const array[], int_list_t lst,
                             MakeListCallback const cb, CPS_Result res);

typedef void (*VoidMappable)(int32_t const val);
void void_map_array(VoidMappable const cb, uint32_t const size,
                                        int32_t const *const arr);

void list2array(int_list_t list, CPS_Result res);

static void print_val(int32_t const val) { printf("%d ", val); }

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/* quick sort functions */
void swap(int *a, int *b);
int Partition(int *arr, int front, int end);
void quicksort(int *arr, int front, int end);

int main(int argc, char *argv[]) {
    int32_t arr[] = {99, 95, 90, 85, 80, 20, 75, 15, 10, 5};
    uint32_t const arr_size = ARRAY_SIZE(arr);

    /* quick sort input arr */
    quicksort(arr, 0, arr_size - 1);

    int32_t res[arr_size];
    /* call make_list and pass list2array as the "continuation" */
    make_list(arr_size, arr, Nil, list2array, res);

    /* print out the quicksorted array */
    void_map_array(print_val, arr_size, res);
    printf("\n");
    return 0;
}

/* construct a linked list from an array */
void make_list(uint32_t const arr_size, int32_t const arr[], int_list_t lst,
                             MakeListCallback const cb, CPS_Result res) {
    
    // cb == list2array

    if (!arr_size) {
        cb(lst, res);
        return;
    }
    make_list(arr_size - 1, arr, &(node_t){.val = arr[arr_size - 1], .next = lst},
                        cb, res);
}

/* transform a linked list into an array */
void list2array(int_list_t list, CPS_Result res) {
    if (Nil == list)
        return;
    int32_t *array = res;
    array[0] = list->val;
    list2array(list->next, array + 1);
}

/* iterate over an array and performs action cb on each element */
void void_map_array(VoidMappable const cb, uint32_t const size,
                                        int32_t const *const arr) {
    
    // cb == print_val
    if (!size)
        return;
    cb(arr[0]);
    void_map_array(cb, size - 1, arr + 1);
}

/* quick sort functions */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int Partition(int *arr, int front, int end) {
    int pivot = arr[end];
    int i = front -1;
    for (int j = front; j < end; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    i++;
    swap(&arr[i], &arr[end]);
    return i;
}

void quicksort(int *arr, int front, int end) {
    if (front < end) {
        int pivot = Partition(arr, front, end);
        quicksort(arr, front, pivot - 1);
        quicksort(arr, pivot + 1, end);
    }
}