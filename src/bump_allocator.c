#include "bump_allocator.h"


void bump_allocator_init(BumpAllocator *allocator, uint64 buffer_size)
{
    allocator->end_index = 0;
    allocator->buffer_size = buffer_size;
    for (uint64 i = 0; i < buffer_size; i++)
        allocator->buffer[i] = 0x00;
}


void* bump_allocator_memory_allocate(BumpAllocator *allocator, uint64 size)
{
    if (allocator == NULL)
        return NULL;
    
    if (size + allocator->end_index > allocator->buffer_size)
        return NULL;
    
    void *memory = allocator->buffer + allocator->end_index;
    allocator->end_index += size;
    return memory;
}


void* bump_allocator_memory_resize(BumpAllocator *allocator, void *memory, uint64 size)
{
    return NULL;
}


void bump_allocator_memory_free(BumpAllocator *allocator, void *memory, uint64 size)
{
    if (allocator == NULL || memory == NULL)
        return;
    
    if (memory + size != allocator->buffer + allocator->end_index)
        return;
    
    allocator->end_index -= size;
    if (allocator->end_index < 0)
        allocator->end_index = 0;
}
