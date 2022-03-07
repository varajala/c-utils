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

/*
 Initialize the ArenaAllocator. Memory needs to be allocated by the caller.
 The pointer to the get_memory-function must be valid for the entire lifetime of
 the ArenaAllocator.
*/
void arena_allocator_init(ArenaAllocator*, void* (*get_memory)(uint64), uint64 arena_size, uint32 max_arenas);

void* arena_allocator_memory_allocate(ArenaAllocator*, uint64);

/*
 Free all memory used by the ArenaAllocator.
*/
void arena_allocator_destroy(ArenaAllocator*, void (*free_memory)(void*, uint64));


#endif