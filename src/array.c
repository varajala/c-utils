#include "datastructures/array.h"


Array* array_new(AllocatorInterface* allocator, uint32 member_count,  uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (member_count <= 0 )
        return NULL;

    void* memory = allocator->memory_allocate(offsetof(Array, data) + member_count * member_size);
    if (memory == NULL)
        return NULL;

    Array *array = (Array*) memory;
    array->member_count = member_count;
    array->member_size = member_size;

    for (uint64 i = 0; i < member_count * member_size; i++)
        array->data[i] = 0x00;

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


void array_insert(Array *array, uint32 index, uint8 *memory)
{
    if (array == NULL || memory == NULL)
        return;
    
    if (index >= array->member_count)
        return;

    uint32 offset = array->member_size * index;
    for (uint32 i = 0; i < array->member_size; i++)
        array->data[offset + i] = memory[i];
}


void array_copy_memory(Array *array, uint8 *memory, uint32 max_length)
{
    if (array == NULL)
        return;

    for (int i = 0; i < array->member_count * array->member_size; i++)
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
        if (compare(comparison, pivot) == FIRST_IS_SMALLER)
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


void array_sort(Array *array, enum ComparisonResult (*compare)(uint8*, uint8*))
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

    uint32 passed_items = 0;
    for (uint64 i = 0; i < array->member_count; i += array->member_size)
    {
        if (func((uint8*)&array->data[i]))
            passed_items++;
    }

    Array *new_array = array_new(allocator, array->member_count, array->member_size);
    if (new_array == NULL)
        return NULL;

    for (uint64 i = 0; i < array->member_count; i += array->member_size)
    {
        if (func((uint8*)&array->data[i]))
        {
            for (uint32 j = 0; j < array->member_size; j++)
                new_array->data[i + j] = array->data[i + j];
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


int64 array_find_index(Array *array, int (*func)(uint8*))
{
    return -1;
}


void array_find_item(Array *array, int (*func)(uint8*), uint8*)
{

}


void array_destroy(AllocatorInterface *allocator, Array *array)
{
    if (allocator == NULL || array == NULL)
        return;
    allocator->memory_free(array, offsetof(Array, data) + array->member_size * array->member_count);
}
