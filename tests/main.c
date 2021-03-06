#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c-utils.h"

// Test files
#include "array_tests.c"
#include "list_tests.c"
#include "dict_tests.c"
#include "set_tests.c"
#include "bump_allocator_tests.c"
#include "arena_allocator_tests.c"


void* _memory_allocate(uint64 size)
{
    return malloc(size);
}


void* _memory_resize(void* memory, uint64 size)
{
    return realloc(memory, size);
}


void _memory_free(void* memory, uint64 size)
{
    free(memory);
}


int (*tests[])(AllocatorInterface*) = {
    test_basic_array_use,
    test_array_bound_check,
    test_array_copy_memory,
    test_array_slicing,
    test_array_foreach,
    test_array_sorting,
    test_array_map,
    test_array_filter,
    test_array_reverse,
    test_array_find_index,
    test_array_find_item,
    test_array_reduce_simple,
    test_array_reduce_complex,

    test_bump_allocator_memory_allocation,
    test_bump_allocator_bound_check,

    test_arena_allocator_uniform_memory_allocation,
    test_arena_allocator_memory_allocation,
    test_arena_allocator_size_check,

    test_basic_list_use,
    test_list_resize,
    test_list_insertion,
    test_list_removing,
    test_list_getting_items,
    test_list_copy_memory,
    test_list_create_slice,
    test_list_foreach,
    test_list_sort,

    test_dict_creation,
    test_dict_usage,
    test_dict_copy_keys,
    test_dict_copy_values,
    test_dict_copy_items,
    test_dict_resize,

    test_set_usage,
    test_set_copy_items,
    test_set_resize,
    NULL
};


int main()
{
    AllocatorInterface allocator;
    allocator.memory_allocate = _memory_allocate;
    allocator.memory_resize = _memory_resize;
    allocator.memory_free = _memory_free;

    int errors = 0;
    int i = 0;

    while (1)
    {
        int (*test_func)(AllocatorInterface*) = tests[i];
        if (test_func == NULL) break;
        if (test_func(&allocator) > 0)
        {
            errors++;
            printf("Error in testcase %d\n", i + 1);
        }
        i++;
    }

    printf("%d tests executed.\n\n", i);
    return errors > 0;
}
