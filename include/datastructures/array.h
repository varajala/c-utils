#ifndef ARRAY_H
#define ARRAY_H

#include "integer_types.h"
#include "allocator.h"
#include <stddef.h>

typedef struct Array
{
    uint32 member_count;
    uint32 member_size;
    uint8 data[];
} Array;


enum ComparisonResult
{
    FIRST_IS_LARGER,
    ARE_EQUAL,
    FIRST_IS_SMALLER
};


/*
 Allocate space and initialize the array.
 All bytes are initialized into 0x00 values.
*/
Array* array_new(AllocatorInterface*, uint32, uint32);

/*
 Copy memory from array.data[index * array.member_size] to the provided address.
 The provided memory must be atleast array.member_size bytes.
*/
void array_get(Array*, uint32, uint8*);

/*
 Copy memory into the array from to the provided address
 for a total of array.member_size bytes.
*/
void array_insert(Array*, uint32, uint8*);

/*
 Copy memory into the array from to the provided address
 for a total length of MIN(array.member_count * array.member_size, max_length).
*/
void array_copy_memory(Array*, uint8*, uint32);

/*
 Create a new array and copy the original array's data into it
 from start_index to end_index.
*/
Array* array_create_slice(Array*, AllocatorInterface*, uint32, uint32);

Array* array_map(Array*, AllocatorInterface*, void (*func)(uint8*, uint8*));

Array* array_filter(Array*, AllocatorInterface*, int (*func)(uint8*));

Array* array_reverse(Array*, AllocatorInterface*);

int64 array_find_index(Array*, int (*func)(uint8*));

void array_find_item(Array*, int (*func)(uint8*), uint8*);

void array_foreach(Array*, void (*func)(uint8*));

void array_sort(Array*, enum ComparisonResult (*compare)(uint8*, uint8*));

/*
 Free memory used by the array.
*/
void array_destroy(AllocatorInterface*, Array*);

#endif
