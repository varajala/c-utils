#include "allocator_implementation.h"
#include <stdlib.h>


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


Allocator* allocator_create()
{
    Allocator *allocator = malloc(sizeof(Allocator));
    allocator->memory_allocate = _memory_allocate;
    allocator->memory_resize = _memory_resize;
    allocator->memory_free = _memory_free;
    return allocator;
}

void allocator_free(Allocator *allocator)
{
    free(allocator);
}
