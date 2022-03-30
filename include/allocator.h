#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "integer_types.h"


// Common interface for memory allocation.
typedef struct AllocatorInterface
{
    // Should allocate a continuous block of memory
    // with the minimium size of provided number of bytes.
    // Returns NULL if allocation fails for any reason.
    void* (*memory_allocate) (uint64);
    
    // Should resize the provided memory block into
    // a new continuous memory block with minimium size
    // of provided number of bytes. The memory from
    // the old block must be copied into the new block
    // for the length of MIN(old_size, new_size) and
    // the old memory block block must be freed.
    // If allocation fails, pointer to the old
    // memory block must be returned.
    void* (*memory_resize)   (void *memory, uint64);

    // Mark the memory in segment [memory, memory + n] freed.
    void  (*memory_free)     (void *memory, uint64);
} AllocatorInterface;

#endif
