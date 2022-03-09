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
    COMPARISON_RESULT_FIRST_IS_LARGER,
    COMPARISON_RESULT_ARE_EQUAL,
    COMPARISON_RESULT_FIRST_IS_SMALLER
};

// Allocate space and initialize the array.
// All bytes are initialized into 0x00 values.
// Returns NULL instead of an empty array.
Array* array_new(AllocatorInterface*, uint32 member_count, uint32 member_size);

// Copy memory from array at the provided index to the provided address.
// The provided memory must be atleast array.member_size bytes.
void array_get(Array*, uint32 index, uint8*);

// Copy memory into the array from to the provided address
// at the specified index for a total of array.member_size bytes.
void array_set(Array*, uint32 index, uint8*);

// Copy memory into the array from to the provided address
// for a total length of MIN(array.member_count * array.member_size, max_length).
// Overwrites exiting members.
void array_copy_memory(Array*, uint8*, uint64 max_length);

// Create a new array and copy the original array's data into it
// from start_index to end_index.
Array* array_create_slice(Array*, AllocatorInterface*, uint32 start_index, uint32 end_index);

// Copy the old array and execute the provided function
// for every member in the copied array.
Array* array_map(Array*, AllocatorInterface*, void (*func)(uint8*));

// Create a new array with the members of the provided array
// for which the provided test function returns non zero value.
// Returns NULL instead of an empty array.
Array* array_filter(Array*, AllocatorInterface*, int (*func)(uint8*));

// Create a reversed copy of the given array.
Array* array_reverse(Array*, AllocatorInterface*);

// Return the first index after the given start index 
// where the test function returns a non zero value.
// If no mathces are found, or an error occurs, negative value is returned.
int64 array_find_index(Array*, uint32 start_index, int (*func)(uint8*));

// Find the first element starting from the given start index
// where the test function returns a non zero value.
// Copies memory for a total length of array.member_size into the given memory location.
void array_find_item(Array*, uint32 start_index, int (*func)(uint8*), uint8*);

// Execute the given function for every member of the array.
void array_foreach(Array*, void (*func)(uint8*));

// Execute the given function for every member of the array.
// This function recieves three arguments:
// previous_value, current_value, result.
// If there is no previous value, use the result instead.
// 
// EXAMPLE calculate sum of array of integers:
//
// void sum_integers(uint8 *prev, uint8 *current, uint8 *result) { int n = *current; *result += n; }
// int result = 0;
// array_reduce(array, sum_integers, &result);
//
void array_reduce(Array*, void (*func)(uint8*, uint8*, uint8*), uint8 *result);

// Sort the array in place.
void array_sort(Array*, enum ComparisonResult (*compare)(uint8*, uint8*));

// Free memory used by the array.
void array_destroy(Array*, AllocatorInterface*);

#endif
