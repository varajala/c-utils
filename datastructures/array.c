#include <stddef.h>
#include <stdio.h>
#include "array.h"


Array* array_create(Allocator* allocator, uint32 n_members,  uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (n_members <= 0 )
        return NULL;

    void* memory = allocator->memory_allocate(offsetof(Array, data) + n_members * member_size);
    if (memory == NULL)
        return NULL;

    Array *array = (Array*) memory;
    array->length = n_members;
    array->member_size = member_size;

    for (int i = 0; i < n_members * member_size; i++)
        array->data[i] = 0x00;

    return array;
}


void array_get(Array *array, uint32 index, uint8 *memory)
{
    if (array == NULL || memory == NULL)
        return;
    
    if (index < 0 || index >= array->length)
        return;
    
    uint32 offset = array->member_size * index;
    for (uint32 i = 0; i < array->member_size; i++)
        memory[i] = array->data[offset + i];
}


void array_insert(Array *array, uint32 index, uint8 *memory)
{
    if (array == NULL || memory == NULL)
        return;
    
    if (index < 0 || index >= array->length)
        return;

    uint32 offset = array->member_size * index;
    for (uint32 i = 0; i < array->member_size; i++)
        array->data[offset + i] = memory[i];
}


void array_copy_memory(Array *array, uint8 *memory, uint32 max_length)
{
    if (array == NULL)
        return;

    for (int i = 0; i < array->length; i++)
    {
        if (i >= max_length)
            break;

        array->data[i] = memory[i];
    }
}


Array* array_create_slice(Array *src_array, Allocator *allocator, uint32 start, uint32 end)
{
    if (src_array == NULL || allocator == NULL)
        return NULL;
    
    if (start < 0 || start >= src_array->length)
        return NULL;

    if (end < 0 || end >= src_array->length)
        return NULL;

    if (end-start <= 0)
        return NULL;

    Array *slice_array = array_create(allocator, end-start, src_array->member_size);
    if (slice_array == NULL)
        return NULL;

    uint32 offset = start * src_array->member_size;
    uint32 i_last = (end-start) * src_array->member_size;
    for (int i = 0; i < i_last; i++)
        slice_array->data[i] = src_array->data[offset+i];
    
    return slice_array;
}   


void array_foreach(Array *array, void (*func)(uint8*))
{
    if (array == NULL)
        return;

    uint32 array_size_in_bytes = array->member_size * array->length;
    for (int i = 0; i < array_size_in_bytes; i += array->member_size)
        func(&array->data[i]);
}


void array_sort(Array *array, enum ComparisonResult (*compare_func)(uint8*, uint8*))
{

}


void array_free(Allocator *allocator, Array *array)
{
    if (allocator == NULL || array == NULL)
        return;
    allocator->memory_free(array, sizeof(uint32) + array->member_size * array->length);
}
