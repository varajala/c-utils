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


int test_list_insertion(Allocator *allocator)
{
    uint8 c;
    char buffer[17];
    int error = 4;

    List *list = list_create(allocator, 1);

    for (int i = 0; i < 16; i++)
        buffer[i] = ' ';
    buffer[16] = 0x00;

    c = 'd';
    list = list_insert(list, 3, &c);
    for (int i = 0; i < list->member_count; i++)
        buffer[i] = list->data[i];
    error -= strcmp(buffer, "d               ") == 0;

    c = 'c';
    list = list_insert(list, 2, &c);
    for (int i = 0; i < list->member_count; i++)
        buffer[i] = list->data[i];
    error -= strcmp(buffer, "dc              ") == 0;

    c = 'b';
    list = list_insert(list, 1, &c);
    for (int i = 0; i < list->member_count; i++)
        buffer[i] = list->data[i];
    error -= strcmp(buffer, "dbc             ") == 0;

    c = 'a';
    list = list_insert(list, 0, &c);
    for (int i = 0; i < list->member_count; i++)
        buffer[i] = list->data[i];
    error -= strcmp(buffer, "adbc            ") == 0;

    list_free(list);
    return error;
}