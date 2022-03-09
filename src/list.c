#include "datastructures/list.h"


static inline uint64 list_get_allocated_buffer_size(List *list)
{
    return list->_allocated_space - offsetof(List, data);
}


List* list_new(AllocatorInterface *allocator, uint32 max_members, uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (max_members == 0 || member_size == 0)
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


inline void list_get(List *list, uint32 index, uint8 *memory)
{
    array_get(list_to_array(list), index, memory);
}


void list_insert(List *list, uint32 index, uint8 *memory)
{
    if (list == NULL || memory == NULL)
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


inline void list_set(List *list, uint32 index, uint8 *memory)
{
    array_set(list_to_array(list), index, memory);
}


inline void list_append(List* list, uint8 *memory)
{
    list_insert(list, list->member_count, memory);
}



void list_remove_at(List *list, uint32 index)
{
    if (list == NULL)
        return;

    if (index > list->member_count - 1)
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


inline Array* list_create_slice(List *list, AllocatorInterface* allocator, uint32 start, uint32 end)
{
    return array_create_slice(list_to_array(list), allocator, start, end);
}


inline void list_foreach(List *list, void (*func)(uint8*))
{
    array_foreach(list_to_array(list), func);
}


inline void list_sort(List *list, enum ComparisonResult (*compare)(uint8*, uint8*))
{
    array_sort(list_to_array(list), compare);
}

inline Array* list_map(List *list, AllocatorInterface *allocator, void (*func)(uint8*))
{
    return array_map(list_to_array(list), allocator, func);
}


inline Array* list_filter(List *list, AllocatorInterface *allocator, int (*func)(uint8*))
{
    return array_filter(list_to_array(list), allocator, func);
}


inline Array* list_reverse(List *list, AllocatorInterface *allocator)
{
    return array_reverse(list_to_array(list), allocator);
}


inline int64 list_find_index(List *list, uint32 start_index, int (*func)(uint8*))
{
    return array_find_index(list_to_array(list), start_index, func);
}


inline void list_find_item(List *list, uint32 start_index, int (*func)(uint8*), uint8 *memory)
{
    return array_find_item(list_to_array(list), start_index, func, memory);
}


inline void list_reduce(List *list, void (*func)(uint8*, uint8*, uint8*), uint8 *result)
{
    return array_reduce(list_to_array(list), func, result);
}


void list_destroy(List *list, AllocatorInterface *allocator)
{
    if (list == NULL || allocator == NULL)
        return;
    
    allocator->memory_free(list, list->_allocated_space);
}
