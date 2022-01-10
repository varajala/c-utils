#include "datastructures/list.h"


int test_basic_list_use(Allocator *allocator)
{
    List *list = list_create(allocator, sizeof(int));

    for (int i = 0; i < 100; i++)
    {
        list = list_insert(list, list->member_count, (uint8*) &i);
    }
    
    list_free(list);
    return 0;
}


typedef struct FourBytes
{
    uint8 high;
    uint8 high_mid;
    uint8 low_mid;
    uint8 low;
} FourBytes;


int test_list_insertion(Allocator *allocator)
{
    FourBytes data;
    char buffer[17];
    int error = 4;

    List *list = list_create(allocator, 4);

    for (int i = 0; i < 16; i++)
        buffer[i] = ' ';
    buffer[16] = 0x00;

    data.high       = 'A';
    data.high_mid   = 'A';
    data.low_mid    = 'A';
    data.low        = 'A';
    
    list = list_append(list, (uint8*) &data);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    error -= strcmp(buffer, "AAAA            ") == 0;

    data.high       = 'B';
    data.high_mid   = 'B';
    data.low_mid    = 'B';
    data.low        = 'B';

    list = list_insert(list, 0, (uint8*) &data);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    error -= strcmp(buffer, "BBBBAAAA        ") == 0;

    data.high       = 'C';
    data.high_mid   = 'C';
    data.low_mid    = 'C';
    data.low        = 'C';

    list = list_insert(list, 1, (uint8*) &data);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    error -= strcmp(buffer, "BBBBCCCCAAAA    ") == 0;

    data.high       = 'D';
    data.high_mid   = 'D';
    data.low_mid    = 'D';
    data.low        = 'D';

    list = list_insert(list, 0, (uint8*) &data);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    error -= strcmp(buffer, "DDDDBBBBCCCCAAAA") == 0;

    list_free(list);
    return error;
}


int test_list_removing(Allocator *allocator)
{
    char *str = "AAAABBBBCCCCDDDD";
    const int buffer_length = strlen(str);
    char buffer[buffer_length + 1];
    int error = 3;
    
    List *list = list_create(allocator, 4);
    list->member_count = 4;
    memcpy(list->data, str, buffer_length);

    list_remove_at(list, buffer_length / 4);
    memset(buffer, 0x00, buffer_length + 1);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    error -= strcmp(buffer, str) == 0;

    list_remove_at(list, 1);
    memset(buffer, 0x00, buffer_length + 1);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    error -= strcmp(buffer, "AAAACCCCDDDD") == 0;

    list_remove_at(list, 0);
    memset(buffer, 0x00, buffer_length + 1);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    error -= strcmp(buffer, "CCCCDDDD") == 0;

    list_free(list);
    return error;
}


int test_list_getting_items(Allocator *allocator)
{
    int number, error;
    int array[4] = { 1, 2, 3, 4 };
    
    List *list = list_create(allocator, sizeof(int));
    list->member_count = 4;
    memcpy(list->data, array, 4 * sizeof(int));

    error = 0;
    
    list_get(list, 0, (uint8*) &number);
    if (number != 1)
    {
        error = 1;
        goto cleanup;
    }

    list_get(list, 1, (uint8*) &number);
    if (number != 2)
    {
        error = 1;
        goto cleanup;
    }

    list_get(list, 3, (uint8*) &number);
    if (number != 4)
    {
        error = 1;
        goto cleanup;
    }

    number = 0;
    
    list_get(list, -1, (uint8*) &number);
    if (number != 0)
    {
        error = 1;
        goto cleanup;
    }

    list_get(list, 8, (uint8*) &number);
    if (number != 0)
    {
        error = 1;
        goto cleanup;
    }
    
    cleanup:
        list_free(list);
    return error;
}


int test_list_copy_memory(Allocator *allocator)
{
    char *short_str = "AABBCCDD";
    char *long_str = "AAAABBBBCCCCDDDDEEEEFFFF";
    const int buffer_length = 32;
    char buffer[buffer_length];
    int error = 0;
    
    List *list, *new_list; 
    list = list_create(allocator, 1);
    new_list = list_copy_memory(list, (uint8*) short_str, strlen(short_str));

    if (new_list != list)
    {
        list = new_list;
        error = 1;
        goto cleanup;
    }

    memset(buffer, 0x00, buffer_length);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    if (strcmp((char*)buffer, short_str) != 0)
    {
        error = 1;
        goto cleanup;
    }

    new_list = list_copy_memory(list, (uint8*) long_str, strlen(long_str));
    if (new_list == list)
    {
        error = 1;
        goto cleanup;
    }
    
    list = new_list;
    memset(buffer, 0x00, buffer_length);
    memcpy(buffer, list->data, list->member_count * list->member_size);
    if (strcmp((char*)buffer, long_str) != 0)
    {
        error = 1;
        goto cleanup;
    }

    cleanup:
        list_free(list);
    return error;
}


int test_list_create_slice(Allocator *allocator)
{
    int error = 0;
    Array *slice;
    
    List *list = list_create(allocator, 1);
    memcpy(list->data, "abcdef", 6);
    list->member_count = 6;

    slice = list_create_slice(list, 0, 3);
    if (slice == NULL || memcmp(slice->data, "abc", 3) != 0)
    {
        error = 1;
        goto cleanup;
    }

    cleanup:
        if (slice != NULL) array_free(allocator, slice);
        list_free(list);
    return error;
}


int test_list_foreach(Allocator *allocator)
{
    int error = 0;
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int modified_data[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
    
    List *list = list_create(allocator, sizeof(int));
    memcpy(list->data, data, 8 * sizeof(int));
    list->member_count = 8;

    void incr(uint8 *item)
    {
        int *number = (int*)item;
        *number = (*number) + 1;
    }

    list_foreach(list, incr);
    if (memcmp(list->data, modified_data, 8 * sizeof(int)) != 0)
        error = 1;
    
    list_free(list);
    return error;
}


int test_list_sort(Allocator *allocator)
{
    int error = 0;
    int data[] = { 2, 5, 8, 1, 7, 6, 4, 3 };
    int sorted_data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    
    List *list = list_create(allocator, sizeof(int));
    memcpy(list->data, data, 8 * sizeof(int));
    list->member_count = 8;

    void read_int(uint8 *bytes, uint8 *integer)
    {
        for (int i = 0; i < sizeof(int); i++)
        {
            integer[i] = bytes[i];
        }
    }

    enum ComparisonResult compare(uint8 *a_bytes, uint8 *b_bytes)
    {
        int a, b;

        read_int(a_bytes, (uint8*)&a);
        read_int(b_bytes, (uint8*)&b);
        
        if (a > b)
            return FIRST_IS_LARGER;
        if (a < b)
            return FIRST_IS_SMALLER;
        return ARE_EQUAL;
    }

    list_sort(list, compare);

    if (memcmp(list->data, sorted_data, 8 * sizeof(int)) != 0)
        error = 1;
    
    list_free(list);
    return error;
}
