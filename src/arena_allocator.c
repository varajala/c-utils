#include "arena_allocator.h"


static BumpAllocator* create_arena(ArenaAllocator *allocator, uint64 arena_size)
{
    return NULL;
}

void arena_allocator_init(ArenaAllocator *allocator, uint64 arena_size, uint32 max_arenas)
{

}


void* arena_allocator_memory_allocate(ArenaAllocator *allocator, uint64 size)
{
    return NULL;
}


void* arena_allocator_memory_resize(ArenaAllocator *allocator, void *memory, uint64 size)
{
    return NULL;
}


void arena_allocator_memory_free(ArenaAllocator *allocator, void *memory, uint64 size)
{

}
