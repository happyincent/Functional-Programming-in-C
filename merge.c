#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define container_of(ptr, type, member)                            \
    __extension__({                                                \
        const __typeof__(((type *) 0)->member) *__pmember = (ptr); \
        (type *) ((char *) __pmember - offsetof(type, member));    \
    })

#define list_entry(node, type, member) container_of(node, type, member)

typedef struct __list {
    struct __list *next;
} list_t;

typedef struct __ele {
    int32_t const val;
    list_t const list;
} ele_t;

typedef void *const CPS_Result;
typedef void (*MakeListCallback)(ele_t *e, CPS_Result res);
void make_list(uint32_t const arr_size,
               int32_t const array[],
               ele_t *e,
               MakeListCallback const cb,
               CPS_Result res);

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
    make_list(arr_size, arr, &(ele_t){.val = 0, .list = (list_t){.next = NULL}}, list2array, res);

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
              &(ele_t){ .val = arr[arr_size - 1], .list = (list_t){ .next = (list_t *)(&(e->list)) } }, 
              cb, res);
}

void list2array(ele_t *e, CPS_Result res) {
    if (e->list.next == NULL) {
        return;
    }
    int32_t *array = res;
    array[0] = e->val;
    list2array(list_entry(e->list.next, ele_t, list), array + 1);
}

void void_map_array(VoidMappable const cb,
                    uint32_t const size,
                    int32_t const *const arr) {
    if (!size)
        return;
    cb(arr[0]);
    void_map_array(cb, size - 1, arr + 1);
}