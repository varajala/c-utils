#include "datastructures/list.h"


static inline Array* list_to_array(List *list)
{
    return (Array*) &list->member_count;
}


static inline uint64 list_get_allocated_buffer_size(List *list)
{
    return list->_allocated_space - offsetof(List, data);
}


List* list_create(Allocator *allocator, uint32 max_members, uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (max_members <= 0 )
        return NULL;

    uint64 buffer_size = max_members * member_size;
    uint64 required_space = offsetof(List, data) + buffer_size;
    
    List *list = (List*) allocator->memory_allocate(required_space);
    if (list == NULL)
        return NULL;

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


void list_insert(List *list, uint32 index, uint8 *memory)
{
    if (list == NULL || memory == NULL)
        return;

    if (index < 0)
        return;

    if (index > list->member_count)
        index = list->member_count;
    
    uint64 used_space = list->member_count * list->member_size;
    uint64 buffer_size = list_get_allocated_buffer_size(list);
    
    if (buffer_size - used_space < list->member_size) {
        return;
    }

    list->member_count += 1;

    if (index < list->member_count - 1)
    {
        for (int64 i = list->member_count - 2; i >= index; i--)
        {
            for (uint32 j = 0; j < list->member_size; j++)
            {
                uint32 offset = i * list->member_size;
                list->data[offset + list->member_size + j] = list->data[offset + j];
            }
        }
    }

    uint32 offset = index * list->member_size;    
    for (uint32 i = 0; i < list->member_size; i++)
        list->data[offset + i] = memory[i];
}


void list_append(List* list, uint8 *memory)
{
    list_insert(list, list->member_count, memory);
}



void list_remove_at(List *list, uint32 index)
{
    if (list == NULL)
        return;

    if (index < 0 || index > list->member_count - 1)
        return;

    for (uint32 i = index + 1; i < list->member_count; i++)
    {
        for (uint32 j = 0; j < list->member_size; j++)
        {
            uint64 offset = i * list->member_size;
            list->data[offset - list->member_size + j] = list->data[offset + j];   
        }
    }
    list->member_count -= 1;
}


void list_copy_memory(List *list, uint8 *memory, uint32 user_buffer_max_size)
{
    if (list == NULL || memory == NULL)
        return;

    uint64 buffer_size = list_get_allocated_buffer_size(list);
    uint64 max_size = user_buffer_max_size > buffer_size ? buffer_size : user_buffer_max_size;

    list->member_count = max_size / list->member_size;
    array_copy_memory(list_to_array(list), memory, max_size);
}


Array* list_create_slice(Allocator* allocator, List *list, uint32 start, uint32 end)
{
    if (list == NULL || allocator == NULL)
        return NULL;
    return array_create_slice(list_to_array(list), allocator, start, end);
}


void list_foreach(List *list, void (*func)(uint8*))
{
    array_foreach(list_to_array(list), func);
}


void list_sort(List *list, enum ComparisonResult (*compare)(uint8*, uint8*))
{
    array_sort(list_to_array(list), compare);
}


void list_free(Allocator *allocator, List *list)
{
    if (list == NULL || allocator == NULL)
        return;
    
    allocator->memory_free(list, list->_allocated_space);
}
