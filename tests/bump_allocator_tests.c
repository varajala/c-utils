#include <stdio.h>
#include "bump_allocator.h"


int test_bump_allocator_memory_allocation(Allocator *allocator)
{
    int error = 0;
    int32 *a, *b;
    uint32 bump_alloc_size = bump_allocator_size(8);
    
    BumpAllocator *bump_alloc = allocator->memory_allocate(bump_alloc_size);
    bump_allocator_init(bump_alloc, 8);
    a = (int32*) bump_allocator_memory_allocate(bump_alloc, 4);
    b = (int32*) bump_allocator_memory_allocate(bump_alloc, 4);
    
    if (a == NULL || b == NULL)
    {
        error = 1;
        goto cleanup;
    }

    error = 4;
    error -= ((void*) a) == bump_alloc->buffer;
    error -= ((void*) b) == bump_alloc->buffer + 4;

    error -= bump_alloc->buffer_size == 8;
    error -= bump_alloc->buffer_size - bump_alloc->end_index == 0;

    cleanup:
        allocator->memory_free(bump_alloc, bump_alloc_size);
    return error;
}
