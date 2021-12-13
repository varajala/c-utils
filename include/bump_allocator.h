#ifndef BUMP_ALLOCATOR_H
#define BUMP_ALLOCATOR_H

#include <stddef.h>
#include "integer_types.h"


typedef struct BumpAllocator
{
    uint64 buffer_size;
    uint64 end_index;
    uint8 buffer[];
} BumpAllocator;


static inline uint64 bump_allocator_size(uint64 buf_size)
{
    return offsetof(BumpAllocator, buffer) + buf_size;
}


void bump_allocator_init(BumpAllocator*, uint64);

void* bump_allocator_memory_allocate(BumpAllocator*, uint64);

void bump_allocator_memory_free(BumpAllocator*, void*, uint64);


#endif
