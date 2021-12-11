#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include <stddef.h>
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


#endif