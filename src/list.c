#include <stddef.h>
#include <stdio.h>
#include "datastructures/list.h"


static inline Array* list_to_array(List *list)
{
    return (Array*) &list->member_count;
}


List* list_create(Allocator *allocator, uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (LIST_INITIAL_SIZE <= 0 )
        return NULL;

    uint64 buffer_size = LIST_INITIAL_SIZE * member_size;
    uint64 required_space = offsetof(List, data) + buffer_size;
    
    List *list = (List*) allocator->memory_allocate(required_space);
    if (list == NULL)
        return NULL;

    list->_allocator = allocator;
    list->_allocated_space = required_space;
    list->member_count = 0;
    list->member_size = member_size;

    for (uint64 i = 0; i < buffer_size; i++)
        list->data[i] = 0x00;

    return list;
}


void list_get(List *list, uint32 index, uint8 *memory)
{
    if (list == NULL || memory == NULL)
        return;

    array_get(list_to_array(list), index, memory);
}


List* list_insert(List *list, uint32 index, uint8 *memory)
{
    if (list == NULL || memory == NULL)
        return list;

    if (index < 0)
        return list;

    if (index >= list->member_count)
        index = list->member_count;

    list->member_count += 1;
    uint64 used_space = list->member_count * list->member_size;
    uint64 buffer_size = list->_allocated_space - offsetof(List, data);
    if (used_space >= LIST_RESIZE_THRESHOLD * buffer_size)
    {
        List *new_list = list->_allocator->memory_resize(list, list->_allocated_space + buffer_size);
        if (new_list == NULL)
        {
            list->member_count -= 1;
            return list;
        }

        list = new_list;
        list->_allocated_space += buffer_size;
    }

    for (int64 i = ((int64)list->member_count) - 2; i >= 0; i -= list->member_size)
    {
        for (int32 j = ((int32)list->member_size) - 1; j >= 0; j--)
        {
            uint64 offset = i * list->member_size;
            list->data[offset + list->member_size + j] = list->data[offset + j];   
        }
    }
    
    array_insert(list_to_array(list), index, memory);
    return list;
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
    
    list->_allocator->memory_free(list, list->_allocated_space);
}
