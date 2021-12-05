#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "integer_types.h"

typedef struct Allocator
{
    void* (*memory_allocate) (uint64);
    void* (*memory_resize)   (void*, uint64);
    void  (*memory_free)     (void*, uint64);
} Allocator;


#endif
