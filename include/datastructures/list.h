#ifndef LIST_H
#define LIST_H

#include "datastructures/array.h"

#define LIST_DATA_OFFSET 128

typedef struct List
{
    uint64 _allocated_space;
    uint32 member_count;
    uint32 member_size;
    uint8 data[];
} List;


// Safely cast List* to Array*
static inline Array* list_to_array(List *list)
{
    return (Array*) &list->member_count;
}

// Allocate memory and initialize the list.
// All bytes are initialized to 0x00.
// Returns NULL if max_members * member_sixe == 0.
List* list_new(AllocatorInterface*, uint32 max_members, uint32 member_size);

// Resize the list.
// If max_members * member_size is smaller than currently allocated space,
// the remaining data will be discarded and marked as freed.
//
// Returns the given pointer to the list if resizing failed,
// pointer to new list otherwise. Assumes that the provided allocator
// is implemented correctly and allocator.memory_resize
// copies the data into the new memory block and frees the
// old list.
List* list_resize(List *list, AllocatorInterface *allocator, uint32 max_members);

// Copy memory from the list at the provided index to the provided address.
// The provided memory must be atleast list.member_size bytes.
void list_get(List*, uint32 index, uint8*);

// Copy memory into the list from to the provided address
// into the specified index for a total of list.member_size bytes.
// Does not increment the member_count, does replacement instead.
void list_set(List*, uint32 index, uint8*);

// Copy memory into the list from to the provided address
// into the specified index for a total of list.member_size bytes.
// Increments the member_count, always adds the new item without effecting existing ones.
// Fails silently if there is not enough memory for the addition.
void list_insert(List*, uint32 index, uint8*);

// Copy memory into the end of the list from to the provided address
// for a total of list.member_size bytes. Increments the member_count,
// always adds the new item without effecting existing ones.
// Fails silently if there is not enough memory for the addition.
void list_append(List*, uint8*);

// Remove the element at the specified index from list.
void list_remove_at(List*, uint32 index);

// Copy memory into the list from to the provided address
// for a total length of MIN(list.max_members * list.member_size, max_length).
// Overwrites exiting members.
void list_copy_memory(List*, uint8*, uint32 length);

// Create a new array and copy the list's data into it
// from start_index to end_index.
Array* list_create_slice(List*, AllocatorInterface*, uint32 start_index, uint32 end_index);

// Copy the list members into an array and execute the provided function
// for every member in the copied array.
Array* list_map(List*, AllocatorInterface*, void (*func)(uint8*));

// Create a new array with the members of the provided list
// for which the provided test function returns non zero value.
// Returns NULL instead of an empty array.
Array* list_filter(List*, AllocatorInterface*, int (*func)(uint8*));

// Create a reversed array from the given list.
Array* list_reverse(List*, AllocatorInterface*);

// Return the first index after the given start index 
// where the test function returns a non zero value.
// If no mathces are found, or an error occurs, negative value is returned.
int64 list_find_index(List*, uint32 start_index, int (*func)(uint8*));

// Find the first element starting from the given start index
// where the test function returns a non zero value.
// Copies memory for a total length of list.member_size into the given memory location.
void list_find_item(List*, uint32 start_index, int (*func)(uint8*), uint8*);

// Execute the given function for every member of the list.
void list_foreach(List*, void (*func)(uint8*));

// Execute the given function for every member of the list.
// This function recieves three arguments:
// previous_value, current_value, result.
// If there is no previous value, use the result instead.
// 
// EXAMPLE calculate sum of list of integers:
//
// void sum_integers(uint8 *prev, uint8 *current, uint8 *result) { int n = *current; *result += n; }
// int result = 0;
// list_reduce(list, sum_integers, &result);
//
void list_reduce(List*, void (*func)(uint8*, uint8*, uint8*), uint8*);

// Sort the list in place.
void list_sort(List*, enum ComparisonResult (*compare)(uint8*, uint8*));

// Free memory used by the list.
void list_destroy(List*, AllocatorInterface *allocator);

#endif
