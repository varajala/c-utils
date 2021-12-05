#include <stddef.h>
#include "array.h"


Array* array_create(Allocator* allocator, uint32 n_members,  uint32 member_size)
{
    // TODO: check for NULL pointer to allocator

    void* memory = allocator->memory_allocate(sizeof(uint32) + n_members * member_size);
    if (memory == NULL)
        return NULL;

    Array *array = (Array*) memory;
    array->length = n_members;
    array->member_size = member_size;
    array->data = memory + sizeof(uint32);

    // TODO: types and intialization
    return array;
}


void array_free(Allocator *allocator, Array *array)
{
    // TODO: check for NULL pointer to allocator & array
    allocator->memory_free(array, sizeof(uint32) + array->member_size * array->length);
}
