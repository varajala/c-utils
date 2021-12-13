#ifndef LIST_H
#define LIST_H

#include "datastructures/array.h"

#ifndef INITIAL_LIST_SIZE
#define INITIAL_LIST_SIZE 16
#endif


typedef struct List
{
    uint32 member_count;
    uint32 member_size;
    Allocator *_allocator;
    Array *_array;
} List;


List* list_create(Allocator*, uint32);

void list_get(List*, uint32, uint8*);

void list_insert(List*, uint32, uint8*);

void list_remove(List*, uint32);

void list_copy_memory(List*, uint8*, uint32);

Array* list_create_slice(List*, uint32, uint32);

void list_foreach(List*, void (*func)(uint8*));

void list_sort(List*, enum ComparisonResult (*compare)(uint8*, uint8*));

void list_free(List*);

#endif
