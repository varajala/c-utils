#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include "integer_types.h"
#include "bump_allocator.h"


typedef struct ArenaAllocator
{
    void* (*get_memory)(uint64);
    uint64 arena_size;
    uint32 max_arenas;
    uint32 num_arenas;
    BumpAllocator *arenas[];
} ArenaAllocator;


// Calculate the memory used by an arena allocator based on its max_arenas.
// The created bumo allocators are allocated dynamically when needed,
// these are not included in the total.
static inline uint64 arena_allocator_size(uint32 max_arenas)
{
    return PLATFORM_POINTER_LENGTH + 128 + (max_arenas * PLATFORM_POINTER_LENGTH);
}

// Initialize the ArenaAllocator. Memory needs to be allocated by the caller.
// The pointer to the get_memory-function must be valid for the entire lifetime of
// the ArenaAllocator.
void arena_allocator_init(ArenaAllocator*, void* (*get_memory)(uint64), uint64 arena_size, uint32 max_arenas);

// Allocate memory, returns NULL if out of memory, or an error occurs.
void* arena_allocator_memory_allocate(ArenaAllocator*, uint64);

// Reset all allocated BumpAllocators. Does not overwrite data,
// or free already allocated buffers.
void arena_allocator_reset(ArenaAllocator*);

// Free all memory used by the ArenaAllocator,
// including dynamically allocated bump allocators.
void arena_allocator_destroy(ArenaAllocator*, void (*free_memory)(void*, uint64));

#endif