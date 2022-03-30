// This software is licensed under the MIT License:
//
// Copyright (c) 2022 Valtteri Rajalainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "c-utils.h"


void bump_allocator_init(BumpAllocator *allocator, uint64 buffer_size)
{
    if (allocator == NULL)
        return;
    
    allocator->end_index = 0;
    allocator->buffer_size = buffer_size;
    for (uint64 i = 0; i < buffer_size; i++)
        allocator->buffer[i] = 0x00;
}

inline void bump_allocator_reset(BumpAllocator *allocator)
{
    if (allocator == NULL)
        return;
    allocator->end_index = 0;
}


void* bump_allocator_memory_allocate(BumpAllocator *allocator, uint64 size)
{
    if (allocator == NULL)
        return NULL;
    
    if (size + allocator->end_index > allocator->buffer_size)
        return NULL;
    
    void *memory = allocator->buffer + allocator->end_index;
    allocator->end_index += size;
    return memory;
}


void arena_allocator_init(ArenaAllocator *allocator, void* (*get_memory)(uint64), uint64 arena_size, uint32 max_arenas)
{
    if (allocator == NULL || get_memory == NULL)
        return;

    if (arena_size == 0 || max_arenas == 0)
        return;

    allocator->get_memory = get_memory;
    allocator->arena_size = arena_size;
    allocator->max_arenas = max_arenas;
    allocator->num_arenas = 0;

    for (uint32 i = 0; i < max_arenas; i++)
        allocator->arenas[i] = NULL;
    
    BumpAllocator *arena = get_memory(bump_allocator_size(arena_size));
    if (arena == NULL)
        return;

    allocator->arenas[0] = arena;
    allocator->num_arenas = 1;
    bump_allocator_init(arena, arena_size);
}


void* arena_allocator_memory_allocate(ArenaAllocator *allocator, uint64 size)
{
    if (allocator == NULL)
        return NULL;
    
    if (size > allocator->arena_size)
        return NULL;

    void *memory;
    BumpAllocator *arena;
    
    for (uint32 i = 0; i < allocator->num_arenas; i++)
    {
        arena = allocator->arenas[i];
        memory = bump_allocator_memory_allocate(arena, size);
        if (memory != NULL)
            return memory;
    }

    if (allocator->num_arenas >= allocator->max_arenas)
        return NULL;
    
    arena = allocator->get_memory(bump_allocator_size(allocator->arena_size));
    if (arena == NULL)
        return NULL;

    allocator->arenas[allocator->num_arenas] = arena;
    allocator->num_arenas++;
    bump_allocator_init(arena, allocator->arena_size);
    return bump_allocator_memory_allocate(arena, size);
}

inline void arena_allocator_reset(ArenaAllocator *allocator)
{
    if (allocator == NULL)
        return;
    
    BumpAllocator *bump_allocator;
    for (uint32 i = 0; i < allocator->num_arenas; i++)
    {
        bump_allocator = allocator->arenas[i];
        bump_allocator_reset(bump_allocator);
    }
}


void arena_allocator_destroy(ArenaAllocator *allocator, void (*free_memory)(void*, uint64))
{
    if (allocator == NULL || free_memory == NULL)
        return;
    
    for (uint32 i = 0; i < allocator->num_arenas; i++)
        free_memory(allocator->arenas[i], allocator->arena_size);
    free_memory(allocator, arena_allocator_size(allocator->max_arenas));
}


inline Array* array_new(AllocatorInterface* allocator, uint32 member_count,  uint32 member_size)
{
    Array *array = array_new_no_init(allocator, member_count, member_size);
    if (array == NULL)
        return NULL;

    for (uint64 i = 0; i < member_count * member_size; i++)
        array->data[i] = 0x00;

    return array;
}


Array* array_new_no_init(AllocatorInterface* allocator, uint32 member_count,  uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (member_count == 0 || member_size == 0)
        return NULL;

    void *memory = allocator->memory_allocate(ARRAY_DATA_OFFSET + (member_count * member_size));
    if (memory == NULL)
        return NULL;

    Array *array = (Array*) memory;
    array->member_count = member_count;
    array->member_size = member_size;
    return array;
}


void array_get(Array *array, uint32 index, uint8 *memory)
{
    if (array == NULL || memory == NULL)
        return;
    
    if (index >= array->member_count)
        return;
    
    uint32 offset = array->member_size * index;
    for (uint32 i = 0; i < array->member_size; i++)
        memory[i] = array->data[offset + i];
}


void array_set(Array *array, uint32 index, uint8 *memory)
{
    if (array == NULL || memory == NULL)
        return;
    
    if (index >= array->member_count)
        return;

    uint32 offset = array->member_size * index;
    for (uint32 i = 0; i < array->member_size; i++)
        array->data[offset + i] = memory[i];
}


void array_copy_memory(Array *array, uint8 *memory, uint64 max_length)
{
    if (array == NULL || memory == NULL)
        return;

    for (uint64 i = 0; i < array->member_count * array->member_size; i++)
    {
        if (i >= max_length)
            break;

        array->data[i] = memory[i];
    }
}


Array* array_create_slice(Array *src_array, AllocatorInterface *allocator, uint32 start, uint32 end)
{
    if (src_array == NULL || allocator == NULL)
        return NULL;
    
    if (start >= src_array->member_count)
        return NULL;

    if (end >= src_array->member_count)
        return NULL;

    if (end-start <= 0)
        return NULL;

    Array *slice_array = array_new(allocator, end-start, src_array->member_size);
    if (slice_array == NULL)
        return NULL;

    uint32 offset = start * src_array->member_size;
    uint32 i_last = (end-start) * src_array->member_size;
    for (uint64 i = 0; i < i_last; i++)
        slice_array->data[i] = src_array->data[offset+i];
    
    return slice_array;
}   


void array_foreach(Array *array, void (*func)(uint8*))
{
    if (array == NULL || func == NULL)
        return;

    for (uint64 i = 0; i < array->member_count * array->member_size; i += array->member_size)
        func(&array->data[i]);
}


static void swap(Array *array, uint32 index_a, uint32 index_b)
{
    if (array == NULL)
        return;

    if (index_a >= array->member_count || index_b >= array->member_count)
        return;

    uint8 temp;
    const uint32 member_size = array->member_size;
    const uint32 offset_a = member_size * index_a;
    const uint32 offset_b = member_size * index_b;

    for (uint32 i = 0; i < member_size; i++)
    {
        temp = array->data[offset_a + i];
        array->data[offset_a + i] = array->data[offset_b + i];
        array->data[offset_b + i] = temp;
    }
}


static int64 partition(Array *array, int64 start, int64 end, enum ComparisonResult (*compare)(uint8*, uint8*))
{
    uint32 member_size = array->member_size;
    
    uint8 *pivot = &array->data[member_size * end];
    uint8 *comparison;
    int64 index = start - 1;
    
    for (uint32 i = start; i < end; i++)
    {
        comparison = &array->data[member_size * i];
        if (compare(comparison, pivot) == COMPARISON_RESULT_FIRST_IS_SMALLER)
            swap(array, ++index, i);
    }

    swap(array, ++index, end);
    return index;
}


static void quicksort(Array *array, int64 start, int64 end, enum ComparisonResult (*compare)(uint8*, uint8*))
{
    if (array == NULL)
        return;

    if (start >= end || start < 0 || end < 0)
        return;

    int64 pivot_index = partition(array, start, end, compare);

    quicksort(array, start, pivot_index - 1, compare);
    quicksort(array, pivot_index + 1, end, compare);
}


inline void array_sort(Array *array, enum ComparisonResult (*compare)(uint8*, uint8*))
{
    if (array == NULL || compare == NULL)
        return;

    quicksort(array, 0, ((int64)array->member_count) - 1, compare);
}


Array* array_map(Array *array, AllocatorInterface *allocator, void (*func)(uint8*))
{
    if (array == NULL || allocator == NULL || func == NULL)
        return NULL;

    Array *new_array = array_new(allocator, array->member_count, array->member_size);
    if (new_array == NULL)
        return NULL;

    array_foreach(new_array, func);
    return new_array;
}


Array* array_filter(Array *array, AllocatorInterface *allocator, int (*func)(uint8*))
{
    if (array == NULL || allocator == NULL || func == NULL)
        return NULL;

    uint64 src_offset;
    uint64 dest_offset;
    uint32 passed_items = 0;
    uint32 index = 0;
    
    for (uint32 i = 0; i < array->member_count; i++)
    {
        src_offset = i * array->member_size;
        if (func((uint8*)&array->data[src_offset]))
            passed_items++;
    }

    Array *new_array = array_new(allocator, passed_items, array->member_size);
    if (new_array == NULL)
        return NULL;

    for (uint32 i = 0; i < array->member_count; i++)
    {
        src_offset = i * array->member_size;
        if (func((uint8*)&array->data[src_offset]))
        {
            dest_offset = index++ * array->member_size;
            for (uint32 j = 0; j < array->member_size; j++)
                new_array->data[dest_offset + j] = array->data[src_offset + j];
        }
    }
    return new_array;
}


Array* array_reverse(Array *array, AllocatorInterface *allocator)
{
    if (array == NULL || allocator == NULL)
        return NULL;

    Array *new_array = array_new(allocator, array->member_count, array->member_size);
    if (new_array == NULL)
        return NULL;

    uint32 start_offset, end_offset;
    for (int64 i = array->member_count - 1; i > -1; i--)
    {
        start_offset = (array->member_count - i - 1) * array->member_size;
        end_offset = i * array->member_size;
        for (uint32 j = 0; j < array->member_size; j++)
            new_array->data[start_offset + j] = array->data[end_offset + j];
    }
    return new_array;
}


int64 array_find_index(Array *array, uint32 start_index, int (*func)(uint8*))
{
    if (array == NULL || func == NULL)
        return -2;
    
    if (start_index >= array->member_count)
        return -3;

    uint64 offset;
    for (uint32 index = start_index; index < array->member_count; index++)
    {
        offset = index * array->member_size;
        if (func((uint8*)&array->data[offset]))
            return index;
    }
    return -1;
}


void array_find_item(Array *array, uint32 start_index, int (*func)(uint8*), uint8 *memory)
{
    if (array == NULL || func == NULL || memory == NULL)
        return;

    if (start_index >= array->member_count)
        return;

    uint64 offset;
    for (uint32 index = start_index; index < array->member_count; index++)
    {
        offset = index * array->member_size;
        if (func((uint8*)&array->data[offset]))
        {
            for (uint32 i = 0; i < array->member_size; i++)
                memory[i] = array->data[offset + i];
            return;
        }
    }
}


void array_reduce(Array *array, void (*func)(uint8*, uint8*, uint8*), uint8 *result)
{
    if (array == NULL || func == NULL || result == NULL)
        return;

    uint8 *current_value = NULL;
    uint8 *previous_value = NULL;
    uint64 offset;
    
    for (uint32 index = 0; index < array->member_count; index++)
    {
        offset = index * array->member_size;
        current_value = (uint8*)&array->data[offset];
        
        if (previous_value == NULL)
            previous_value = result;

        func(previous_value, current_value, result);
        previous_value = current_value;
    }
}


void array_destroy(Array *array, AllocatorInterface *allocator)
{
    if (allocator == NULL || array == NULL)
        return;
    allocator->memory_free(array, ARRAY_DATA_OFFSET + (array->member_size * array->member_count));
}


static inline uint64 list_get_allocated_buffer_size(List *list)
{
    return list->_allocated_space - LIST_DATA_OFFSET;
}


List* list_new(AllocatorInterface *allocator, uint32 max_members, uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (max_members == 0 || member_size == 0)
        return NULL;

    uint64 buffer_size = max_members * member_size;
    uint64 required_space = LIST_DATA_OFFSET + buffer_size;
    
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


List* list_resize(List *list, AllocatorInterface *allocator, uint32 max_members)
{
    if (list == NULL || allocator == NULL)
        return list;
    
    List *new_list;
    uint32 new_member_count = list->member_count < max_members ? list->member_count : max_members;
    uint64 new_buffer_size = max_members * list->member_size;
    uint64 new_size = new_buffer_size + LIST_DATA_OFFSET;
    
    // Assume the allocator is implemented correctly
    // and memory is copied into new memory block...
    new_list = allocator->memory_resize(list, new_size);
    
    new_list->_allocated_space = new_size;
    new_list->member_count = new_member_count;
    return new_list;
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
    array_find_item(list_to_array(list), start_index, func, memory);
}


inline void list_reduce(List *list, void (*func)(uint8*, uint8*, uint8*), uint8 *result)
{
    array_reduce(list_to_array(list), func, result);
}


void list_destroy(List *list, AllocatorInterface *allocator)
{
    if (list == NULL || allocator == NULL)
        return;
    
    allocator->memory_free(list, list->_allocated_space);
}


static const int64 EMPTY_SLOT = -1;
static const int64 REMOVED_SLOT = -2;

static uint64 hash(uint8 *data, uint32 length)
{
    uint64 result = 5381;
    for (uint32 i = 0; i < length; i++)
        result = ((result << 5) + result) + data[i];
    return result;
}

static inline uint32 probe_index(uint8 *key, uint32 key_length, uint32 tries, uint32 slots)
{
    // Quadratic probing, index = (h(k) + c1 * t + c2 * t^2) % num_slots
    const uint32 MAGIC_PRIME_1 = 7841;
    const uint32 MAGIC_PRIME_2 = 5903;
    return (
        hash(key, key_length)
        + (MAGIC_PRIME_1 * tries)
        + (MAGIC_PRIME_2 * tries * tries)
    ) % slots;
}

static inline int mem_equals(uint8* src, uint8 *cmp, uint64 length)
{
    for (uint64 i = 0; i < length; i++)
    {
        if (src[i] != cmp[i])
            return 0;
    }
    return 1;
}


static int64 dict_get_index(Dict *dict, uint8 *key)
{
    uint8 *key_value;
    uint64 key_size = dict->keys->member_size;
    int64 slot_value;
    uint32 index;
    uint32 tries = 1;

    while (tries < dict->_num_slots)
    {
        index = probe_index(key, key_size, tries-1, dict->_num_slots);
        array_get(dict->index_table, index, (uint8*)&slot_value);
        
        if (slot_value == EMPTY_SLOT)
            return -1;

        if (slot_value != REMOVED_SLOT)
        {
            key_value = &dict->keys->data[key_size * slot_value];
            if (mem_equals(key, key_value, key_size)) {
                return slot_value;
            }
        }
        tries++;
    }
    return -1;
}


Dict* dict_new(AllocatorInterface *allocator, uint32 max_members, uint32 key_size, uint32 value_size)
{
    if (allocator == NULL)
        return NULL;
    
    Dict *dict = allocator->memory_allocate(sizeof(Dict));
    if (dict == NULL)
        return NULL;

    Array *index_table = array_new(allocator, max_members, 8);
    if (index_table == NULL)
    {
        allocator->memory_free(dict, sizeof(dict));
        return NULL;
    }
    
    
    List *key_list = list_new(allocator, max_members, key_size);
    if (key_list == NULL)
    {
        allocator->memory_free(dict, sizeof(dict));
        array_destroy(index_table, allocator);
        return NULL;
    }
    
    List *value_list = list_new(allocator, max_members, value_size);
    if (value_list == NULL)
    {
        allocator->memory_free(dict, sizeof(dict));
        array_destroy(index_table, allocator);
        list_destroy(key_list, allocator);
        return NULL;
    }

    dict->_num_slots = max_members;
    dict->member_count = 0;
    dict->index_table = index_table;
    dict->keys = key_list;
    dict->values = value_list;

    for (uint32 i = 0; i < max_members; i++)
    {
        array_set(index_table, i, (uint8*)&EMPTY_SLOT);
    }

    return dict;
}


int dict_contains_key(Dict *dict, uint8 *key)
{
    if (dict == NULL || key == NULL)
        return 0;
    
    int64 index = dict_get_index(dict, key);
    if (index < 0) return 0;
    return 1;
}


void dict_set(Dict *dict, uint8 *key, uint8* value)
{
    if (dict == NULL || key == NULL || value == NULL)
        return;

    uint8 *key_value;
    uint64 key_size = dict->keys->member_size;
    int64 slot_value;
    uint32 index;
    uint32 tries = 1;
    
    while (tries < dict->_num_slots)
    {
        index = probe_index(key, key_size, tries-1, dict->_num_slots);
        array_get(dict->index_table, index, (uint8*)&slot_value);
        
        if (slot_value == EMPTY_SLOT || slot_value == REMOVED_SLOT)
        {
            slot_value = (int64) dict->member_count;
            array_set(dict->index_table, index, (uint8*)&slot_value);
            list_append(dict->keys, key);
            list_append(dict->values, value);
            dict->member_count++;
            break;
        }

        // Check if key is the same, and do updating instead...
        key_value = &dict->keys->data[slot_value];
        if (mem_equals(key, key_value, key_size)) {
            list_set(dict->values, slot_value, value);
            break;
        }
        tries++;
    }
}


void dict_get(Dict *dict, uint8 *key, uint8 *memory)
{
    if (dict == NULL || key == NULL || memory == NULL)
        return;

    int64 index = dict_get_index(dict, key);
    if (index > -1)
        list_get(dict->values, index, memory);
}


void dict_pop(Dict *dict, uint8 *key, uint8 *memory)
{
    if (dict == NULL || key == NULL || memory == NULL)
        return;
    
    int64 index = dict_get_index(dict, key);
    if (index > -1)
    {
        list_get(dict->values, index, memory);
        array_set(dict->index_table, index, (uint8*)&REMOVED_SLOT);
        list_remove_at(dict->keys, index);
        list_remove_at(dict->values, index);
        dict->member_count--;
    }
}


Array* dict_copy_keys(Dict *dict, AllocatorInterface *allocator)
{
    if (dict == NULL || allocator == NULL)
        return NULL;

    Array *keys = list_to_array(dict->keys);
    Array *copy = array_new(allocator, keys->member_count, keys->member_size);
    array_copy_memory(copy, (uint8*)keys->data, keys->member_count * keys->member_size);
    return copy;
}


Array* dict_copy_values(Dict *dict, AllocatorInterface *allocator)
{
    if (dict == NULL || allocator == NULL)
        return NULL;

    Array *values = list_to_array(dict->values);
    Array *copy = array_new(allocator, values->member_count, values->member_size);
    array_copy_memory(copy, (uint8*)values->data, values->member_count * values->member_size);
    return copy;
}


Array* dict_copy_items(Dict *dict, AllocatorInterface *allocator)
{
    if (dict == NULL || allocator == NULL)
        return NULL;
    
    const uint32 key_size = dict->keys->member_size;
    const uint32 value_size = dict->values->member_size;
    const uint32 member_size = key_size + value_size;
    
    Array *items = array_new(allocator, dict->member_count, member_size);

    uint64 dest_offset;
    uint64 src_offset;
    
    for (uint32 i = 0; i < items->member_count; i++)
    {
        dest_offset = i * member_size;
        src_offset = i * key_size;
        for (uint32 j = 0; j < key_size; j++)
            items->data[dest_offset + j] = dict->keys->data[src_offset + j];
    }

    for (uint32 i = 0; i < items->member_count; i++)
    {
        dest_offset = (i * member_size) + key_size;
        src_offset = i * value_size;
        for (uint32 j = 0; j < value_size; j++)
            items->data[dest_offset + j] = dict->values->data[src_offset + j];
    }
    
    return items;
}


void dict_destroy(Dict* dict, AllocatorInterface *allocator)
{
    if (allocator == NULL || dict == NULL)
        return;

    if (dict->index_table != NULL)
        array_destroy(dict->index_table, allocator);
    
    if (dict->keys != NULL)
        list_destroy(dict->keys, allocator);

    if (dict->values != NULL)
        list_destroy(dict->values, allocator);

    allocator->memory_free(dict, sizeof(dict));
}
