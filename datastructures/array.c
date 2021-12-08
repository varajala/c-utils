#include <stddef.h>
#include <stdio.h>
#include "array.h"


Array* array_create(Allocator* allocator, uint32 n_members,  uint32 member_size)
{
    if (allocator == NULL)
        return NULL;
    
    if (n_members <= 0 )
        return NULL;

    void* memory = allocator->memory_allocate(offsetof(Array, data) + n_members * member_size);
    if (memory == NULL)
        return NULL;

    Array *array = (Array*) memory;
    array->length = n_members;
    array->member_size = member_size;

    for (int i = 0; i < n_members * member_size; i++)
        array->data[i] = 0x00;

    return array;
}


void array_get(Array *array, uint32 index, uint8 *memory)
{
    if (array == NULL || memory == NULL)
        return;
    
    if (index < 0 || index >= array->length)
        return;
    
    uint32 offset = array->member_size * index;
    for (uint32 i = 0; i < array->member_size; i++)
        memory[i] = array->data[offset + i];
}


void array_insert(Array *array, uint32 index, uint8 *memory)
{
    if (array == NULL || memory == NULL)
        return;
    
    if (index < 0 || index >= array->length)
        return;

    uint32 offset = array->member_size * index;
    for (uint32 i = 0; i < array->member_size; i++)
        array->data[offset + i] = memory[i];
}


void array_copy_memory(Array *array, uint8 *memory, uint32 max_length)
{
    if (array == NULL)
        return;

    for (int i = 0; i < array->length * array->member_size; i++)
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
    
    if (start < 0 || start >= src_array->length)
        return NULL;

    if (end < 0 || end >= src_array->length)
        return NULL;

    if (end-start <= 0)
        return NULL;

    Array *slice_array = array_create(allocator, end-start, src_array->member_size);
    if (slice_array == NULL)
        return NULL;

    uint32 offset = start * src_array->member_size;
    uint32 i_last = (end-start) * src_array->member_size;
    for (int i = 0; i < i_last; i++)
        slice_array->data[i] = src_array->data[offset+i];
    
    return slice_array;
}   


void array_foreach(Array *array, void (*func)(uint8*))
{
    if (array == NULL)
        return;

    uint32 array_size_in_bytes = array->member_size * array->length;
    for (int i = 0; i < array_size_in_bytes; i += array->member_size)
        func(&array->data[i]);
}


static void merge(uint8 *array, uint32 member_size, int start, int mid, int end, enum ComparisonResult (*compare)(uint8*, uint8*))
{
    uint32 partition_size = (end - start + 1) * member_size;
    uint32 offset = start * member_size;
    
    uint8 temp[partition_size];
    uint8 *num_a, *num_b;
    int a_nmembers, b_nmembers, member_index;
    
    for (member_index = 0; member_index < partition_size; member_index++)
    {
        temp[member_index] = array[offset + member_index];
    }

    a_nmembers = mid - start + 1;
    b_nmembers = end - mid;
    num_a = temp;
    num_b = temp + (a_nmembers * member_size);
    member_index = 0;

    while (a_nmembers > 0 && b_nmembers > 0)
    {
        if (compare(num_a, num_b) != FIRST_IS_LARGER)
        {
            for (int i = 0; i < member_size; i++)
                array[offset + member_size * member_index + i] = num_a[i];
            num_a += member_size;
            a_nmembers--;
        }
        else
        {
            for (int i = 0; i < member_size; i++)
                array[offset + member_size * member_index + i] = num_b[i];
            num_b += member_size;
            b_nmembers--;
        }
        member_index++;
    }

    while (a_nmembers > 0)
    {
        for (int i = 0; i < member_size; i++)
            array[offset + member_size * member_index + i] = num_a[i];
        num_a += member_size;
        a_nmembers--;
        member_index++;
    }

    while (b_nmembers > 0)
    {
        for (int i = 0; i < member_size; i++)
            array[offset + member_size * member_index + i] = num_b[i];
        num_b += member_size;
        b_nmembers--;
        member_index++;
    }
}

static void mergesort(uint8 *numbers, uint32 member_size, int start, int end, enum ComparisonResult (*compare)(uint8*, uint8*))
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

    mergesort(array->data, array->member_size, 0, array->length-1, compare);
}


void array_free(Allocator *allocator, Array *array)
{
    if (allocator == NULL || array == NULL)
        return;
    allocator->memory_free(array, sizeof(uint32) + array->member_size * array->length);
}
