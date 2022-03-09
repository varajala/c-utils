#include "datastructures/array.h"


int test_basic_array_use(AllocatorInterface *allocator)
{
    int error = 0;
    Array* array = array_new(allocator, 8, sizeof(int32));

    int32 number;
    uint32 i;
    
    for (i = 0; i < array->member_count; i++)
    {
        number = i + 1;
        array_set(array, i, (uint8*)&number);
    }

    number = -1;
    for ( i = 0; i < array->member_count; i++)
    {
        array_get(array, i, (uint8*)&number);
        if (((uint32) number) != i + 1)
        {
            error = 1;
            break;
        }
    }

    array_destroy(array, allocator);
    return error;
}


int test_array_bound_check(AllocatorInterface *allocator)
{
    Array* array = array_new(allocator, 8, sizeof(int32));
    if (array == NULL)
        return 1;
    
    int32 memory;
    array_get(array, 4096, (uint8*)&memory);
    array_get(array, -4096, (uint8*)&memory);
    array_set(array, 4096, (uint8*)&memory);
    array_set(array, -4096, (uint8*)&memory);
    array_destroy(array, allocator);
    return 0;
}


int test_array_copy_memory(AllocatorInterface *allocator)
{
    char *str =  "hello";
    Array* array = array_new(allocator, 8, sizeof(char));
    if (array == NULL)
        return 1;

    array_copy_memory(array, (uint8*)str, strlen(str));

    int result = strcmp(str, (char*)array->data);
    array_destroy(array, allocator);
    return result;
}


int test_array_slicing(AllocatorInterface *allocator)
{
    int error, start, end;
    Array *array, *slice;
    error = 0;
    
    array  = array_new(allocator, 8, sizeof(int32));
    if (array == NULL)
        return 1;
    
    int32 *numbers = (int32*) array->data;
    for (uint32 i = 0; i < array->member_count;  i++)
        numbers[i] = i;

    start = 4;
    end = 6;
    slice = array_create_slice(array, allocator, start, end);
    if (slice == NULL)
    {
        error = 1;
        goto cleanup;
    }

    int32 *slice_numbers = (int32*) slice->data;
    for (int i = 0; i < end-start; i++)
    {
        if (numbers[start+i] != slice_numbers[i])
        {
            error = 1;
            goto cleanup;
        }
    }

    cleanup:
        array_destroy(array, allocator);
        array_destroy(slice, allocator);
    
    return error;
}

static void ascii_to_uppercase(uint8 *memory)
{
    uint8 mask = ~0x20;
    uint8 c = *memory;
    *memory = c & mask;
}


int test_array_foreach(AllocatorInterface *allocator)
{
    int error = 0;
    uint8 *str = (uint8*) "hello";
    uint8 *expected_result = (uint8*) "HELLO";
    
    Array *array  = array_new(allocator, 8, 1);
    if (array == NULL)
        return 1;

    memcpy(array->data, str, 5);
    array_foreach(array, ascii_to_uppercase);

    error = memcmp(array->data, expected_result, 5) != 0;
    array_destroy(array, allocator);
    return error;
}


void array_read_int(uint8 *bytes, uint8 *integer)
{
    for (uint32 i = 0; i < sizeof(int); i++)
    {
        integer[i] = bytes[i];
    }
}


enum ComparisonResult array_compare(uint8 *a_bytes, uint8 *b_bytes)
{
    int a, b;

    array_read_int(a_bytes, (uint8*)&a);
    array_read_int(b_bytes, (uint8*)&b);
    
    if (a > b)
        return FIRST_IS_LARGER;
    if (a < b)
        return FIRST_IS_SMALLER;
    return ARE_EQUAL;
}


int test_array_sorting(AllocatorInterface *allocator)
{
    int error = 0;
    const int NUMBERS_LENGTH = 16;
    int numbers[] = {
        2, -6, 4, 16,
        44, 2, 0, -1,
        32, 26, 2, -8,
        0, 1, 1, 9
    };

    int sorted_numbers[] = {
        -8, -6, -1, 0,
        0, 1, 1, 2,
        2, 2, 4, 9,
        16, 26, 32, 44
    };
    
    Array *array  = array_new(allocator, NUMBERS_LENGTH, sizeof(int));
    if (array == NULL)
        return 1;

    array_copy_memory(array, (uint8*)numbers, NUMBERS_LENGTH * sizeof(int));
    
    array_sort(array, array_compare);

    int *array_numbers = (int*)array->data;
    for (uint32 i = 0; i < array->member_count; i++)
    {
        if (array_numbers[i] != sorted_numbers[i])
        {
            error = 1;
            break;
        }
    }

    array_destroy(array, allocator);
    return error;
}


static void square(uint8 *memory)
{
    int *number = (int*) memory;
    *number = (*number) * (*number);
}


int test_array_map(AllocatorInterface *allocator)
{
    int error = 0;
    int numbers[16] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    int squared_numbers[16] = {
        1, 4, 9, 16,
        25, 36, 49, 64,
        81, 100, 121, 144,
        169, 196, 225, 256
    };

    Array *array, *new_array;
    array = array_new(allocator, 16, sizeof(int));

    memcpy(array->data, numbers, 16 * sizeof(int));
    new_array = array_map(array, allocator, square);
    if (new_array == NULL)
        error = 1;

    if (new_array != NULL)
    {
        error = memcmp(new_array->data, squared_numbers, 16 * sizeof(int));
        array_destroy(new_array, allocator);
    }

    array_destroy(array, allocator);
    return error;
}


static int array_larger_than_6(uint8 *memory) { return (*memory) > 6; }


int test_array_filter(AllocatorInterface *allocator)
{
    int error = 0;
    int numbers[16] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    Array *array, *new_array;
    array = array_new(allocator, 16, sizeof(int));
    memcpy(array->data, numbers, 16 * sizeof(int));

    new_array = array_filter(array, allocator, array_larger_than_6);
    if (new_array == NULL)
        error = 1;

    if (new_array != NULL)
    {
        error = 2;
        error -= new_array->member_count == 10;
        error -= (memcmp(&numbers[6], new_array->data, 10 * sizeof(int)) == 0);
        
        array_destroy(new_array, allocator);
    }

    array_destroy(array, allocator);
    return error;
}


int test_array_reverse(AllocatorInterface *allocator)
{
    int error = 0;
    int numbers[16] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    int reversed_numbers[16] = {
        16, 15, 14, 13,
        12, 11, 10, 9,
        8, 7, 6, 5,
        4, 3, 2, 1
    };

    Array *array, *new_array;
    array = array_new(allocator, 16, sizeof(int));
    memcpy(array->data, numbers, 16 * sizeof(int));

    new_array = array_reverse(array, allocator);
    if (new_array == NULL)
        error = 1;

    if (new_array != NULL)
    {
        error = memcmp(reversed_numbers, new_array->data, 16 * sizeof(int));
        array_destroy(new_array, allocator);
    }

    array_destroy(array, allocator);
    return error;
}


static int is_even(uint8 *memory)
{
    int *number = (int*) memory;
    return !((*number) & 0x01);
}


static int always_false(uint8* memory)
{
    return 0;
}


int test_array_find_index(AllocatorInterface *allocator)
{
    int error = 0;
    int numbers[16] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    int64 index;
    Array *array = array_new(allocator, 16, sizeof(int));
    memcpy(array->data, numbers, 16 * sizeof(int));

    index = array_find_index(array, 0, is_even);
    if (index != 1)
    {
        error = 1;
        goto cleanup;
    }

    index = array_find_index(array, 12, is_even);
    if (index != 13)
    {
        error = 1;
        goto cleanup;
    }

    index = array_find_index(array, 0, always_false);
    if (!(index < 0))
    {
        error = 1;
        goto cleanup;
    }

    cleanup:
        array_destroy(array, allocator);
    return error;
}


int test_array_find_item(AllocatorInterface *allocator)
{
    int error = 0;
    int numbers[16] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    int result;
    Array *array = array_new(allocator, 16, sizeof(int));
    memcpy(array->data, numbers, 16 * sizeof(int));

    result = -1;
    array_find_item(array, 0, is_even, (uint8*)&result);
    if (result != 2)
    {
        error = 1;
        goto cleanup;
    }

    result = -1;
    array_find_item(array, 12, is_even, (uint8*)&result);
    if (result != 14)
    {
        error = 1;
        goto cleanup;
    }

    result = -1;
    array_find_item(array, 0, always_false, (uint8*)&result);
    if (result != -1)
    {
        error = 1;
        goto cleanup;
    }

    cleanup:
        array_destroy(array, allocator);
    return error;
}


static void sum_func(uint8 *pa, uint8 *pb, uint8* res)
{
    int b = *pb;
    *res += b;
}


int test_array_reduce_simple(AllocatorInterface *allocator)
{
    int numbers[9] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
    };

    int result;
    Array *array = array_new(allocator, 8, sizeof(int));
    memcpy(array->data, numbers, 8 * sizeof(int));

    result = 0;
    array_reduce(array, sum_func, (uint8*)&result);

    array_destroy(array, allocator);
    return !(result == 36);
}

void array_conditional_sum_func(uint8 *pa, uint8 *pb, uint8* res)
{
    int a = *pa;
    int b = *pb;
    if (b > a)
        *res += b;
}


int test_array_reduce_complex(AllocatorInterface *allocator)
{
    int numbers[9] = {
        4, 3, 8, 1,
        2, 6, 5, 7
    };

    int result;
    Array *array = array_new(allocator, 8, sizeof(int));
    memcpy(array->data, numbers, 8 * sizeof(int));

    result = 0;
    array_reduce(array, array_conditional_sum_func, (uint8*)&result);

    array_destroy(array, allocator);
    return !(result == 27);
}
