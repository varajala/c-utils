#include <stddef.h>
#include "datastructures/list.h"


List* list_create(Allocator *allocator, uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    List *list = allocator->allocate_memory(sizeof(list));
    Array *array = array_create(allocator, INITIAL_LIST_SIZE, member_size)
    if (list == NULL || array == NULL)
        return NULL;
    
    list->member_count = 0;
    list->member_size = member_size
    list->allocator = allocator;
    list->_array = array;
    return list;
}


void list_get(List *list, uint32 index, uint8 *memory)
{

}


void list_insert(List *list, uint32 index, uint8 *memory)
{

}


void list_remove(List *list, uint32 index)
{

}


void list_copy_memory(List *list, uint8 *memory, uint32 max_size)
{

}


Array* list_create_slice(List *list, uint32 start, uint32 end)
{
    return NULL;
}


void list_foreach(List *list, void (*func)(uint8*))
{

}


void list_sort(List *list, enum ComparisonResult (*compare)(uint8*, uint8*))
{

}

void list_free(List *list)
{
    if (list == NULL)
        return;
    
    uint64 allocated_size = offsetof(List, _array) + offsetof(Array, data) + list->member_size * list->_array->member_count;
    list->allocator->memory_free(list, allocated_size);
}
