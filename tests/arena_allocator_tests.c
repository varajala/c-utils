#include "arena_allocator.h"


int test_arena_allocator_memory_allocation(Allocator *allocator)
{   
    uint32 i;
    ArenaAllocator *arena_alloc = allocator->memory_allocate(arena_allocator_size(4));
    arena_allocator_init(arena_alloc, allocator->memory_allocate, 1024, 4);

    for (i = 0; i < 1024; i++)
    {
        int32 *memory = arena_allocator_memory_allocate(arena_alloc, sizeof(int32));
        if (memory == NULL)
            return 1;
        *memory = i;
    }

    int32 *memory = arena_allocator_memory_allocate(arena_alloc, sizeof(int32));
    if (memory != NULL)
        return 1;

    if (arena_alloc->num_arenas != 4)
        return 1;

    for (i = 0; i < arena_alloc->num_arenas; i++)
    {
        if (arena_alloc->arenas[i] == NULL)
            return 1;
    }

    arena_allocator_destroy(arena_alloc, allocator->memory_free);
    return 0;
}