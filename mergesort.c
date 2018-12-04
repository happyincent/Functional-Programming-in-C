#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define container_of(ptr, type, member)                            \
    ((type *) ((char *) (ptr) -offsetof(type, member)))

#define list_entry(node, type, member) container_of(node, type, member)

typedef struct __list * list_t;
typedef struct __list {
    list_t next;
} node_t;

typedef struct __ele {
    int32_t const val;
    list_t const list;
} ele_t;

static const ele_t Nil = {.val = 0, .list = &(node_t){ .next = NULL }};

typedef void *const CPS_Result;
typedef void (*MakeListCallback)(ele_t *e, CPS_Result res);
void make_list(uint32_t const arr_size,
               int32_t const array[],
               ele_t *e,
               MakeListCallback const cb,
               CPS_Result res);

void mergesort_toarray(ele_t *e, CPS_Result res);

/* Merge sort */
void mergesort(ele_t **source);
void partition(ele_t *head, ele_t **front, ele_t **back);
ele_t *mergeLists(ele_t *a, ele_t *b);

typedef void (*VoidMappable)(int32_t const val);
void void_map_array(VoidMappable const cb,
                    uint32_t const size,
                    int32_t const *const arr);

void list2array(ele_t *e, CPS_Result res);

static void print_val(int32_t const val) { printf("%d ", val); }

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main(int argc, char *argv[]) {
    int32_t arr[] = {99, 95, 90, 85, 80, 20, 75, 15, 10, 5};
    uint32_t const arr_size = ARRAY_SIZE(arr);

    int32_t res[arr_size];
    make_list(arr_size, arr, (ele_t *)(&Nil), mergesort_toarray, res);

    void_map_array(print_val, arr_size, res);
    printf("\n");
    return 0;
}

void make_list(uint32_t const arr_size,
               int32_t const arr[],
               ele_t *e,
               MakeListCallback const cb,
               CPS_Result res) {
    if (!arr_size) {
        cb(e, res);
        return;
    }

    make_list(arr_size - 1, arr,
              &(ele_t){
                   .val = arr[arr_size - 1],
                   .list = &(node_t){ .next = (list_t)(&(e->list)) } 
              }, 
              cb, res);
}

void list2array(ele_t *e, CPS_Result res) {
    if (e->list->next == NULL) {
        return;
    }
    int32_t *array = res;
    array[0] = e->val;
    list2array(list_entry(e->list->next, ele_t, list), array + 1);
}

void mergesort_toarray(ele_t *e, CPS_Result res) {
    if (e->list->next != NULL)
        mergesort(&e);
    list2array(e, res);
}

void void_map_array(VoidMappable const cb,
                    uint32_t const size,
                    int32_t const *const arr) {
    if (!size)
        return;
    cb(arr[0]);
    void_map_array(cb, size - 1, arr + 1);
}

/* Merge sort */
void mergesort(ele_t **source) {
    ele_t *head = *source;
    ele_t *a = NULL;
    ele_t *b = NULL;

    if (head == NULL || head->list->next == (list_t)(&(Nil.list)))
        return;
    
    partition(head, &a, &b);

    mergesort(&a);
    mergesort(&b);

    *source = mergeLists(a, b);
}

void partition(ele_t *head, ele_t **front, ele_t **back) {

    ele_t *fast;
    ele_t *slow;

    if (head == NULL || head->list->next == (list_t)(&(Nil.list))) {
        *front = head;
        *back = NULL;
    }
    else {
        slow = head;
        fast = list_entry(head->list->next, ele_t, list);

        while(fast->list->next != NULL) {
            
            fast = list_entry(fast->list->next, ele_t, list);

            if (fast->list->next != NULL) {
                slow = list_entry(slow->list->next, ele_t, list);
                fast = list_entry(fast->list->next, ele_t, list);
            }
        }

        *front = head;
        *back = list_entry(slow->list->next, ele_t, list);
        slow->list->next = (list_t)(&(Nil.list));
    }

}

ele_t *mergeLists(ele_t *a, ele_t *b) {
    ele_t *mergedList = NULL;
    ele_t *tmp;

    if (a->list->next == NULL) {
        return b;
    }
    else if (b->list->next == NULL) {
        return a;
    }

    if (a->val <= b->val) {
        mergedList = a;
        tmp = mergeLists(list_entry(a->list->next, ele_t, list), b);
        mergedList->list->next = (list_t)(&(tmp->list));
    }
    else {
        mergedList = b;
        tmp = mergeLists(a, list_entry(b->list->next, ele_t, list));
        mergedList->list->next = (list_t)(&(tmp->list));
    }

    return mergedList;
}