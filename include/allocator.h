#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "integer_types.h"


/*
 The interface that this library uses for handling memory.
 
 #include <stdlib.h>
 
 void* _malloc(uint64 size)
 {
    return malloc(size);
 }

 void* _realloc(void *memory, uint64 new_size)
 {
    return realloc(memory, new_size);
 }

 void _free(void *memory, uint64 size)
 {
    return free(memory);
 }

 int main(int argc, char *argv[])
 {
    Allcator allocator;
    allocator.memory_allocate = _malloc;
    allocator.memory_resize = _realloc;
    allocator.memory_free = _free;
    return 0;
 }
*/
typedef struct Allocator
{
    void* (*memory_allocate) (uint64);
    void* (*memory_resize)   (void*, uint64);
    void  (*memory_free)     (void*, uint64);
} Allocator;


#endif
