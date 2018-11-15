#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* integer linked list type */
typedef struct __int_list * int_list_t;
// typedef struct __int_list const *const int_list_t; // error: x->next = y, x = x->next

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

// typedef void (*quicksortedListCallback)(int_list_t list, CPS_Result res);
int_list_t quicksort(int_list_t head, int_list_t tail);

typedef void (*VoidMappable)(int32_t const val);
void void_map_array(VoidMappable const cb, uint32_t const size,
                                        int32_t const *const arr);

void list2array(int_list_t list, CPS_Result res);

/* quick sort functions */
int_list_t getTail(int_list_t cur);
int_list_t partition(int_list_t head, int_list_t end, 
                        int_list_t *newhead, int_list_t *newtail);
int_list_t quicksort(int_list_t head, int_list_t tail);

/* sort a list and then store it in an array */
void quicksort_toarray(int_list_t list, CPS_Result res) {
    list2array(
        quicksort(list, getTail(list)), res
    );
}

static void print_val(int32_t const val) { printf("%d ", val); }

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main(int argc, char *argv[]) {
    int32_t arr[] = {99, 95, 90, 85, 80, 20, 75, 15, 10, 5};
    uint32_t const arr_size = ARRAY_SIZE(arr);

    int32_t res[arr_size];
    /* call make_list and pass quicksort_toarray as the "continuation" */
    make_list(arr_size, arr, Nil, quicksort_toarray, res);

    /* print out the quicksortd array */
    void_map_array(print_val, arr_size, res);
    printf("\n");
    return 0;
}

/* construct a linked list from an array */
void make_list(uint32_t const arr_size, int32_t const arr[], int_list_t lst,
                             MakeListCallback const cb, CPS_Result res) {
    
    // cb == quicksort_toarray

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

int_list_t getTail(int_list_t cur)
{
    if (cur == Nil || cur->next == Nil) {
        return cur;
    }
    return getTail(cur->next);
}

int_list_t partition(int_list_t head, int_list_t end, 
                        int_list_t *newhead, int_list_t *newtail) {
    
    int_list_t pivot = end;
    int_list_t prev = Nil, cur = head, tail = pivot;

    while (cur != pivot) {
        if (cur->val < pivot->val) {
            if (*newhead == Nil)
                *newhead = cur;
            prev = cur;
            cur = cur->next;
        }
        else {
            if (prev != Nil)
                prev->next = cur->next;
            int_list_t tmp = cur->next;
            cur->next = Nil;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    if (*newhead == Nil)
        *newhead = pivot;
    *newtail = tail;
    return pivot;
}

int_list_t quicksort(int_list_t head, int_list_t tail) {
    
    if (head == Nil)
        return head;
    
    int_list_t newhead = Nil, newtail = Nil;
    
    int_list_t pivot = partition(head, tail, &newhead, &newtail);

    if (newhead != pivot) {

        int_list_t tmp = newhead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = NULL;

        newhead = quicksort(newhead, tmp);

        tmp = getTail(newhead);
        tmp->next = pivot;
    }

    pivot->next = quicksort(pivot->next, newtail);

    return newhead;
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