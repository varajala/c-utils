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