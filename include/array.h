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


typedef enum ComparisonResult
{
    FIRST_IS_LARGER,
    EQUAL,
    FIRST_IS_SMALLER
} ComparisonResult;


Array* array_create(Allocator*, uint32, uint32);

void array_get(Array*, uint32, uint8*);

void array_insert(Array*, uint32, uint8*);

Array* array_create_slice(Array*, Allocator*, uint32, uint32);

void array_foreach(Array *array, void (*func)(uint8*));

void array_map(Array *src, Array *dst, uint8* (*func)(uint8*));

void array_sort(Array *array, enum ComparisonResult (*compare_func)(uint8*, uint8*));

void array_free(Allocator*, Array*);

#endif
