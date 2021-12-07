#ifndef ARRAY_H
#define ARRAY_H

#include "integer_types.h"
#include "allocator.h"

#define ARRAY_LENGTH_INT_TYPE uint32

typedef struct Array
{
    uint32 length;
    uint32 member_size;
    uint8 data[];
} Array;


enum ComparisonResult
{
    FIRST_IS_LARGER,
    ARE_EQUAL,
    FIRST_IS_SMALLER
};


Array* array_create(Allocator*, uint32, uint32);

void array_get(Array*, uint32, uint8*);

void array_insert(Array*, uint32, uint8*);

void array_copy_memory(Array*, uint8*, uint32);

Array* array_create_slice(Array*, Allocator*, uint32, uint32);

void array_foreach(Array *array, void (*func)(uint8*));

void array_sort(Array *array, enum ComparisonResult (*compare)(uint8*, uint8*));

void array_free(Allocator*, Array*);

#endif
