#include <stddef.h>
#include "bump_allocator.h"


void* bump_allocator_memory_allocate(BumpAllocator*, uint64)
{
    return NULL;
}


void* bump_allocator_memory_resize(BumpAllocator*, void*, uint64)
{
    return NULL;
}


void bump_allocator_memory_free(BumpAllocator*, void*, uint64)
{

}
