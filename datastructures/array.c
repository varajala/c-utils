#include <stddef.h>
#include <stdio.h>
#include "array.h"


Array* array_create(Allocator* allocator, uint32 n_members,  uint32 member_size)
{
    if (allocator == NULL)
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


void array_free(Allocator *allocator, Array *array)
{
    if (allocator == NULL || array == NULL)
        return;
    allocator->memory_free(array, sizeof(uint32) + array->member_size * array->length);
}
