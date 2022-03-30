
int test_arena_allocator_uniform_memory_allocation(AllocatorInterface *allocator)
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


int test_arena_allocator_memory_allocation(AllocatorInterface *allocator)
{   
    uint8 *memory;
    uint32 i, alloc_size, memory_allocated;
    ArenaAllocator *arena_alloc = allocator->memory_allocate(arena_allocator_size(4));
    arena_allocator_init(arena_alloc, allocator->memory_allocate, 1024, 4);

    i = 0;
    memory_allocated = 0;
    while (memory_allocated < 4 * 1024)
    {
        alloc_size = (i & 0x03) + 1;
        memory = arena_allocator_memory_allocate(arena_alloc, alloc_size);
        if (memory == NULL)
            return 1;

        memory_allocated += alloc_size;
        i++;
    }

    memory = arena_allocator_memory_allocate(arena_alloc, 1);
    if (memory != NULL)
        return 1;

    if (arena_alloc->num_arenas != 4)
        return 1;

    if (i < 1024)
        return 1;

    for (i = 0; i < arena_alloc->num_arenas; i++)
    {
        if (arena_alloc->arenas[i] == NULL)
            return 1;
    }

    arena_allocator_destroy(arena_alloc, allocator->memory_free);
    return 0;
}


int test_arena_allocator_size_check(AllocatorInterface *allocator)
{
    const int arena_size = 32;
    void *memory;
    int error = 0;
    
    ArenaAllocator *arena_alloc = allocator->memory_allocate(arena_allocator_size(arena_size));
    arena_allocator_init(arena_alloc, allocator->memory_allocate, arena_size, 4);

    memory = arena_allocator_memory_allocate(arena_alloc, 2 * arena_size);
    if (memory != NULL)
    {
        error = 1;
        goto cleanup;
    }

    memory = arena_allocator_memory_allocate(arena_alloc, arena_size);
    if (memory == NULL)
    {
        error = 1;
        goto cleanup;
    }

    cleanup:
        arena_allocator_destroy(arena_alloc, allocator->memory_free);
    return error;
}