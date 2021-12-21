#include "datastructures/array.h"


int test_basic_array_use(Allocator *allocator)
{
    int error = 0;
    Array* array = array_create(allocator, 8, sizeof(int32));

    int32 number, i;
    for (i = 0; i < array->member_count; i++)
    {
        number = i + 1;
        array_insert(array, i, (uint8*)&number);
    }

    number = -1;
    for ( i = 0; i < array->member_count; i++)
    {
        array_get(array, i, (uint8*)&number);
        if (number != i+1)
        {
            error = 1;
            break;
        }
    }

    array_free(allocator, array);
    return error;
}


int test_array_bound_check(Allocator *allocator)
{
    Array* array = array_create(allocator, 8, sizeof(int32));
    if (array == NULL)
        return 1;
    
    int32 memory;
    array_get(array, 4096, (uint8*)&memory);
    array_get(array, -4096, (uint8*)&memory);
    array_insert(array, 4096, (uint8*)&memory);
    array_insert(array, -4096, (uint8*)&memory);
    array_free(allocator, array);
    return 0;
}


int test_array_copy_memory(Allocator *allocator)
{
    char *str =  "hello";
    Array* array = array_create(allocator, 8, sizeof(char));
    if (array == NULL)
        return 1;

    array_copy_memory(array, (uint8*)str, strlen(str));

    int result = strcmp(str, (char*)array->data);
    array_free(allocator, array);
    return result;
}


int test_array_slicing(Allocator *allocator)
{
    int error, start, end;
    Array *array, *slice;
    error = 0;
    
    array  = array_create(allocator, 8, sizeof(int32));
    if (array == NULL)
        return 1;
    
    int32 *numbers = (int32*) array->data;
    for (int i = 0; i < array->member_count;  i++)
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
        array_free(allocator, array);
        array_free(allocator, slice);
    
    return error;
}


int test_array_foreach(Allocator *allocator)
{
    char buffer[6];
    char *str = "hello";
    int buffer_index = 0;
    
    Array *array  = array_create(allocator, 6, sizeof(char));
    if (array == NULL)
        return 1;

    for (int i = 0; i < array->member_size * array->member_count; i += array->member_size)
    {
        array->data[i] = str[i];
    }

    void write_char_to_buffer(uint8 *c)
    {
        buffer[buffer_index++] = *c;
    }
    array_foreach(array, write_char_to_buffer);

    array_free(allocator, array);
    return strcmp(buffer, str);
}


int test_array_sorting(Allocator *allocator)
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
    
    Array *array  = array_create(allocator, NUMBERS_LENGTH, sizeof(int));
    if (array == NULL)
        return 1;

    array_copy_memory(array, (uint8*)numbers, NUMBERS_LENGTH * sizeof(int));

    void read_int(uint8 *bytes, uint8 *integer)
    {
        for (int i = 0; i < sizeof(int); i++)
        {
            integer[i] = bytes[i];
        }
    }

    enum ComparisonResult compare(uint8 *a_bytes, uint8 *b_bytes)
    {
        int a, b;

        read_int(a_bytes, (uint8*)&a);
        read_int(b_bytes, (uint8*)&b);
        
        if (a > b)
            return FIRST_IS_LARGER;
        if (a < b)
            return FIRST_IS_SMALLER;
        return ARE_EQUAL;
    }
    
    array_sort(array, compare);

    int *array_numbers = (int*)array->data;
    for (int i = 0; i < array->member_count; i++)
    {
        if (array_numbers[i] != sorted_numbers[i])
        {
            error = 1;
            break;
        }
    }

    array_free(allocator, array);
    return error;
}
