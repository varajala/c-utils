#ifndef LIST_H
#define LIST_H

#include "datastructures/array.h"

typedef struct List
{
    uint64 _allocated_space;
    uint32 member_count;
    uint32 member_size;
    uint8 data[];
} List;

static inline Array* list_to_array(List *list)
{
    return (Array*) &list->member_count;
}

List* list_new(AllocatorInterface*, uint32 max_members, uint32 member_size);

void list_get(List*, uint32, uint8*);

void list_set(List*, uint32 index, uint8*);

void list_insert(List*, uint32 index, uint8*);

void list_append(List*, uint8*);

void list_remove_at(List*, uint32 index);

void list_copy_memory(List*, uint8*, uint32 length);

Array* list_create_slice(AllocatorInterface*, List*, uint32 start_index, uint32 end_index);

Array* list_map(List*, AllocatorInterface*, void (*func)(uint8*));

Array* list_filter(List*, AllocatorInterface*, int (*func)(uint8*));

Array* list_reverse(List*, AllocatorInterface*);

int64 list_find_index(List*, uint32 start_index, int (*func)(uint8*));

void list_find_item(List*, uint32 start_index, int (*func)(uint8*), uint8*);

void list_foreach(List*, void (*func)(uint8*));

void list_reduce(List*, void (*func)(uint8*, uint8*, uint8*), uint8*);

void list_foreach(List*, void (*func)(uint8*));

void list_sort(List*, enum ComparisonResult (*compare)(uint8*, uint8*));

void list_destroy(AllocatorInterface *allocator, List*);

#endif
