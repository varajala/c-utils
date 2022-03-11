#include "bump_allocator.h"


void bump_allocator_init(BumpAllocator *allocator, uint64 buffer_size)
{
    if (allocator == NULL)
        return;
    
    allocator->end_index = 0;
    allocator->buffer_size = buffer_size;
    for (uint64 i = 0; i < buffer_size; i++)
        allocator->buffer[i] = 0x00;
}

inline void bump_allocator_reset(BumpAllocator *allocator)
{
    if (allocator == NULL)
        return;
    allocator->end_index = 0;
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
