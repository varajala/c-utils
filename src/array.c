#include <stddef.h>
#include "datastructures/array.h"


Array* array_new(Allocator* allocator, uint32 member_count,  uint32 member_size)
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


Array* array_create_slice(Array *src_array, Allocator *allocator, uint32 start, uint32 end)
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
    if (array == NULL)
        return;

    for (uint64 i = 0; i < array->member_count * array->member_size; i += array->member_size)
        func(&array->data[i]);
}

static void merge(
    uint8 *array,
    uint32 member_size,
    int start,
    int mid,
    int end,
    enum ComparisonResult (*compare)(uint8*, uint8*)
    )
{
    uint32 partition_size = (end - start + 1) * member_size;
    uint32 offset = start * member_size;
    
    uint8 temp[partition_size];
    uint8 *ptr_a, *ptr_b;
    uint32 members_in_a, members_in_b, member_index;
    
    for (member_index = 0; member_index < partition_size; member_index++)
    {
        temp[member_index] = array[offset + member_index];
    }

    members_in_a = mid - start + 1;
    members_in_b = end - mid;
    ptr_a = temp;
    ptr_b = temp + (members_in_a * member_size);
    member_index = 0;

    while (members_in_a > 0 && members_in_b > 0)
    {
        if (compare(ptr_a, ptr_b) != FIRST_IS_LARGER)
        {
            for (uint32 i = 0; i < member_size; i++)
                array[offset + member_size * member_index + i] = ptr_a[i];
            ptr_a += member_size;
            members_in_a--;
        }
        else
        {
            for (uint32 i = 0; i < member_size; i++)
                array[offset + member_size * member_index + i] = ptr_b[i];
            ptr_b += member_size;
            members_in_b--;
        }
        member_index++;
    }

    while (members_in_a > 0)
    {
        for (uint32 i = 0; i < member_size; i++)
            array[offset + member_size * member_index + i] = ptr_a[i];
        ptr_a += member_size;
        members_in_a--;
        member_index++;
    }

    while (members_in_b > 0)
    {
        for (uint32 i = 0; i < member_size; i++)
            array[offset + member_size * member_index + i] = ptr_b[i];
        ptr_b += member_size;
        members_in_b--;
        member_index++;
    }
}

static void mergesort(
    uint8 *numbers,
    uint32 member_size,
    int start,
    int end,
    enum ComparisonResult (*compare)(uint8*, uint8*)
    )
{
    if (start < end)
    {
        int mid = start + (end-start) / 2;
        mergesort(numbers, member_size, start, mid, compare);
        mergesort(numbers, member_size, mid+1, end, compare);
        merge(numbers, member_size, start, mid, end, compare);   
    }
}


void array_sort(Array *array, enum ComparisonResult (*compare)(uint8*, uint8*))
{
    if (array == NULL || compare == NULL)
        return;

    mergesort(array->data, array->member_size, 0, array->member_count-1, compare);
}


void array_destroy(Allocator *allocator, Array *array)
{
    if (allocator == NULL || array == NULL)
        return;
    allocator->memory_free(array, offsetof(Array, data) + array->member_size * array->member_count);
}
