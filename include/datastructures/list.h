#ifndef LIST_H
#define LIST_H

#include "datastructures/array.h"

/*
 Every time an insertion is made, the fraction
 used_space / allocated_space is checked. If the
 fraction >= LIST_RESIZE_THRESHOLD,
 the list is reallocated to double the size.
*/
#ifndef LIST_RESIZE_THRESHOLD
#define LIST_RESIZE_THRESHOLD 1.00
#endif


typedef struct List
{
    Allocator *_allocator;
    uint64 _allocated_space;
    uint32 member_count;
    uint32 member_size;
    uint8 data[];
} List;

/*
 Allocate memory and initialize the list.
 The list stores the pointer to the allocator,
 it must be valid for entire lifetime of the list.
*/
List* list_create(Allocator*, uint32, uint32);

void list_get(List*, uint32, uint8*);

/*
 Insert the data read from the given pointer into the given index and 
 reallocate the list if needed. Returns the pointer to the list,
 which can be different than the pointer passed in as argument.
*/
List* list_insert(List*, uint32, uint8*);

/*
 Insert the data read from the given pointer into the end of the list and 
 reallocate the list if needed. Returns the pointer to the list,
 which can be different than the pointer passed in as argument.
*/
List* list_append(List*, uint8*);

void list_remove_at(List*, uint32);

/*
 Copy the memory from the pointer given into the list and 
 reallocate the list if needed. Returns the pointer to the list,
 which can be different than the pointer passed in as argument.
*/
List* list_copy_memory(List*, uint8*, uint32);

Array* list_create_slice(List*, uint32, uint32);

void list_foreach(List*, void (*func)(uint8*));

void list_sort(List*, enum ComparisonResult (*compare)(uint8*, uint8*));

void list_free(List*);

#endif
