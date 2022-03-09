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


// Calculate the total space used by a bump allocator based on its buffer size.
static inline uint64 bump_allocator_size(uint64 buf_size)
{
    return offsetof(BumpAllocator, buffer) + buf_size;
}

// Initialize the bump allocator, memory needs to be allocated by the caller.
void bump_allocator_init(BumpAllocator*, uint64);

// Reset the end_index to point to the start of buffer.
// Does not overwrite the previously allocated data.
void bump_allocator_reset_fast(BumpAllocator*);

// Reset the end_index to point to the start of buffer
// and overwrite the previously allocated data with 0x00 bytes.
void bump_allocator_reset_overwrite(BumpAllocator*);

// Allocate memory, returns NULL if out of memory or an error occurs.
void* bump_allocator_memory_allocate(BumpAllocator*, uint64);

#endif
