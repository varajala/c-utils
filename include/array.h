#ifndef ARRAY_H
#define ARRAY_H

#include "integer_types.h"
#include "allocator.h"

#ifndef ARRAY_TYPE
#define ARRAY_TYPE void*
#endif

typedef struct Array
{
    uint32 length;
    ARRAY_TYPE *data;
} Array;


Array* array_create(Allocator*, uint32, ARRAY_TYPE);

void array_insert(Array*, uint32, ARRAY_TYPE);

ARRAY_TYPE* array_get(Array*, uint32);

void array_free(Allocator*);

#endif
