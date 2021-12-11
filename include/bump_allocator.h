#ifndef BUMP_ALLOCATOR_H
#define BUMP_ALLOCATOR_H

#include "integer_types.h"


typedef struct BumpAllocator
{
    uint64 buffer_size;
    uint64 end_index;
    uint8 buffer[];
} BumpAllocator;


void* bump_allocator_memory_allocate(BumpAllocator*, uint64);

void* bump_allocator_memory_resize(BumpAllocator*, void*, uint64);

void bump_allocator_memory_free(BumpAllocator*, void*, uint64);


#endif
