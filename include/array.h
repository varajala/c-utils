#ifndef ARRAY_H
#define ARRAY_H

#include "integer_types.h"
#include "allocator.h"

#define ARRAY_LENGTH_INT_TYPE uint32

typedef struct Array
{
    uint32 length;
    uint32 member_size;
    uint8 *data;
} Array;


Array* array_create(Allocator*, uint32, uint32);

void array_free(Allocator*, Array*);

#endif
