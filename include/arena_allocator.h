#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include <stddef.h>
#include "bump_allocator.h"
#include "integer_types.h"


typedef struct ArenaAllocator
{
    void* (*get_memory)(uint64);
    uint64 arena_size;
    uint32 max_arenas;
    uint32 num_arenas;
    BumpAllocator *arenas[];
} ArenaAllocator;


static inline uint64 arena_allocator_size(uint32 max_arenas)
{
    return offsetof(ArenaAllocator, arenas) + max_arenas * sizeof(void*);
}


void arena_allocator_init(ArenaAllocator*, void* (*)(uint64), uint64, uint32);

void* arena_allocator_memory_allocate(ArenaAllocator*, uint64);

void arena_allocator_destroy(ArenaAllocator*, void (*)(void*, uint64));


#endif