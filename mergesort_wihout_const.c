#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* integer linked list type */
// typedef struct __int_list const *const int_list_t;
typedef struct __int_list * int_list_t;
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
void make_list(uint32_t const arr_size,
               int32_t const array[],
               int_list_t lst,
               MakeListCallback const cb,
               CPS_Result res);

// typedef void (*MergesortListCallback)(int_list_t list, CPS_Result res);
void mergesort(int_list_t *list);

typedef void (*VoidMappable)(int32_t const val);
void void_map_array(VoidMappable const cb,
                    uint32_t const size,
                    int32_t const *const arr);

void list2array(int_list_t list, CPS_Result res);

/* mergesort a list and then store it in an array */
void mergesort_toarray(int_list_t list, CPS_Result res) {
    mergesort(&list);
    list2array(list, res);
}

static void print_val(int32_t const val) { printf("%d ", val); }

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main(int argc, char *argv[]) {
    int32_t arr[] = {99, 95, 90, 85, 80, 20, 75, 15, 10, 5};
    uint32_t const arr_size = ARRAY_SIZE(arr);

    int32_t res[arr_size];
    /* call make_list and pass mergesort_toarray as the "continuation" */
    make_list(arr_size, arr, Nil, mergesort_toarray, res);

    /* print out the reversed array */
    void_map_array(print_val, arr_size, res);
    printf("\n");
    return 0;
}

/* construct a linked list from an array */
void make_list(uint32_t const arr_size,
               int32_t const arr[],
               int_list_t lst,
               MakeListCallback const cb,
               CPS_Result res) {
    if (!arr_size) {
        cb(lst, res);
        return;
    }
    make_list(arr_size - 1, arr,
              &(node_t) {.val = arr[arr_size - 1], .next = lst}, cb, res);
}

/* transform a linked list into an array */
void list2array(int_list_t list, CPS_Result res) {
    if (Nil == list)
        return;
    int32_t *array = res;
    array[0] = list->val;
    list2array(list->next, array + 1);
}

// void partition(int_list_t head, int_list_t *front, int_list_t *back) {

//     int_list_t fast;
//     int_list_t slow;

//     if (head == NULL || head->next == NULL) {
//         *front = head; // &a
//         *back = NULL; // &b
//     }
//     else {
//         slow = head;
//         fast = head->next;

//         while(fast != NULL) {
//             fast = fast->next;

//             if (fast != NULL) {
//                 slow = slow->next;
//                 fast = fast->next;
//             }
//         }

//         *front = head; // a
//         *back = slow->next; // b
//         slow->next = NULL;
//     }

// }

void partition(int_list_t head, 
                int_list_t slow, int_list_t fast, 
                int_list_t *front, int_list_t *back) {
    
    if (head == NULL || head->next == NULL) {
        *front = head;
        *back = NULL;
        return;
    }
    
    if (fast == NULL) {
        *front = head;
        *back = slow->next;
        slow->next = NULL;
        return;
    }
    else {
        if (fast->next == NULL)
            partition(head, slow, fast->next, front, back);
        else
            partition(head, slow->next, fast->next->next, front, back);
    }
}

int_list_t mergeLists(int_list_t a, int_list_t b) {

    int_list_t mergedList = NULL;

    if (a == NULL) {
        return b;
    }
    else if (b == NULL) {
        return a;
    }

    if (a->val <= b->val) {
        mergedList = a;
        mergedList->next = mergeLists(a->next, b);
    }
    else {
        mergedList = b;
        mergedList->next = mergeLists(a, b->next);
    }

    return mergedList;
}

void mergesort(int_list_t *source) {

    int_list_t head = *source;
    int_list_t a = NULL;
    int_list_t b = NULL;

    if (head == NULL || head->next == NULL) {
        return;
    }

    partition(head, head, head->next, &a, &b);
    // partition(head, &a, &b);

    mergesort(&a);
    mergesort(&b);

    *source = mergeLists(a, b);
}

/* iterate over an array and performs action cb on each element */
void void_map_array(VoidMappable const cb,
                    uint32_t const size,
                    int32_t const *const arr) {
    if (!size)
        return;
    cb(arr[0]);
    void_map_array(cb, size - 1, arr + 1);
}