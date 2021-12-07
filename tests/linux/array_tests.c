#include "allocator_implementation.h"
#include "array.h"
#include <stdio.h>
#include <string.h>


int test_basic_array_use(Allocator *allocator)
{
    int error = 0;
    Array* array = array_create(allocator, 8, sizeof(int32));

    int32 number, i;
    for (i = 0; i < array->length; i++)
    {
        number = i + 1;
        array_insert(array, i, (uint8*)&number);
    }

    number = -1;
    for ( i = 0; i < array->length; i++)
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
    for (int i = 0; i < array->length;  i++)
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

    for (int i = 0; i < array->member_size * array->length; i += array->member_size)
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


int (*tests[])(Allocator*) = {
    test_basic_array_use,
    test_array_bound_check,
    test_array_copy_memory,
    test_array_slicing,
    test_array_foreach,
    NULL
};


int main(int argc, char *argv[])
{
    Allocator *allocator = allocator_create();

    int i = 0;
    while (1)
    {
        int (*test_func)(Allocator*) = tests[i];
        if (test_func == NULL) break;
        if (test_func(allocator) > 0)
        {
            printf("Error in testcase %d\n", i);
        }
        i++;
    }

    allocator_free(allocator);

    printf("%d tests executed.\n\n", i);
    return 0;
}
