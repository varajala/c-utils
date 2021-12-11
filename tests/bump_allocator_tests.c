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


int test_bump_allocator_bound_check(Allocator *allocator)
{
    int error = 0;
    int32 *a, *b;
    uint32 bump_alloc_size = bump_allocator_size(6);
    
    BumpAllocator *bump_alloc = allocator->memory_allocate(bump_alloc_size);
    bump_allocator_init(bump_alloc, 6);
    a = (int32*) bump_allocator_memory_allocate(bump_alloc, 4);
    b = (int32*) bump_allocator_memory_allocate(bump_alloc, 4);
    
    if (a == NULL || b != NULL)
    {
        error = 1;
        goto cleanup;
    }

    error = 3;
    error -= ((void*) a) == bump_alloc->buffer;
    error -= bump_alloc->buffer_size == 6;
    error -= bump_alloc->buffer_size - bump_alloc->end_index == 2;

    cleanup:
        allocator->memory_free(bump_alloc, bump_alloc_size);
    return error;
}


int test_bump_allocator_memory_free(Allocator *allocator)
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
    bump_allocator_memory_free(bump_alloc, a, 4);
    error -= bump_alloc->buffer_size - bump_alloc->end_index == 0;

    bump_allocator_memory_free(bump_alloc, b, 4);
    error -= bump_alloc->buffer_size - bump_alloc->end_index == 4;

    bump_allocator_memory_free(bump_alloc, a, 4);
    error -= bump_alloc->buffer_size - bump_alloc->end_index == 8;

    bump_allocator_memory_free(bump_alloc, a - 1024, 1024);
    error -= bump_alloc->buffer_size - bump_alloc->end_index == 8;

    cleanup:
        allocator->memory_free(bump_alloc, bump_alloc_size);
    return error;
}