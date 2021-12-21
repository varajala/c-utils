#include "arena_allocator.h"


void arena_allocator_init(ArenaAllocator *allocator, void* (*get_memory)(uint64), uint64 arena_size, uint32 max_arenas)
{
    allocator->get_memory = get_memory;
    allocator->arena_size = arena_size;
    allocator->max_arenas = max_arenas;
    allocator->num_arenas = 0;

    for (uint32 i = 0; i < max_arenas; i++)
        allocator->arenas[i] = 0x00;
    
    BumpAllocator *arena = get_memory(bump_allocator_size(arena_size));
    if (arena == NULL)
        return;

    allocator->arenas[0] = arena;
    allocator->num_arenas = 1;
    bump_allocator_init(arena, arena_size);
}


void* arena_allocator_memory_allocate(ArenaAllocator *allocator, uint64 size)
{
    if (size > allocator->arena_size)
        return NULL;

    void *memory;
    BumpAllocator *arena;
    for (uint32 i = 0; i < allocator->num_arenas; i++)
    {
        arena = allocator->arenas[i];
        memory = bump_allocator_memory_allocate(arena, size);
        if (memory != NULL)
            return memory;
    }

    if (allocator->num_arenas >= allocator->max_arenas)
        return NULL;
    
    arena = allocator->get_memory(bump_allocator_size(allocator->arena_size));
    if (arena == NULL)
        return NULL;

    allocator->arenas[allocator->num_arenas] = arena;
    allocator->num_arenas++;
    bump_allocator_init(arena, allocator->arena_size);
    return bump_allocator_memory_allocate(arena, size);
}


void arena_allocator_destroy(ArenaAllocator *allocator, void (*free_memory)(void*, uint64))
{
    for (uint32 i = 0; i < allocator->num_arenas; i++)
        free_memory(allocator->arenas[i], allocator->arena_size);
    free_memory(allocator, arena_allocator_size(allocator->max_arenas));
}
