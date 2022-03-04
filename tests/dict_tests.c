#include "datastructures/dict.h"


#define INITIAL_DICT_SIZE 10

typedef struct DictKey {
    char string[8];
    uint32 number;
} DictKey;

typedef struct DictValue {
    int x;
    int y;
} DictValue;


int test_dict_creation(Allocator *allocator)
{
    int error = 0;
    Dict *dict = dict_create(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));
    if (dict == NULL)
        return 1;
    
    if (dict->member_count != 0)
        error = 1;

    int64 index;
    for (int i = 0; i < INITIAL_DICT_SIZE; i++)
    {
        array_get(dict->index_table, i, (uint8*)&index);
        if (index >= 0)
        {
            error = 1;
            break;
        }
    }

    dict_free(allocator, dict);
    return error;
}


int test_dict_insert(Allocator *allocator)
{
    int error = 0;
    DictKey k;
    DictValue v;

    Dict *dict = dict_create(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));
    if (dict == NULL)
        return 1;

    memset(k.string, 0x00, 8);
    memcpy(k.string, "ABCD", 5);
    k.number = 42;
    v.x = 100;
    v.y = 200;
    dict_insert(dict, (uint8*)&k, (uint8*)&v);

    memcpy(k.string, "EFGH", 5);
    k.number = 42;
    v.x = 200;
    v.y = 300;
    dict_insert(dict, (uint8*)&k, (uint8*)&v);

    memcpy(k.string, "IJKL", 5);
    k.number = 42;
    v.x = 300;
    v.y = 400;
    dict_insert(dict, (uint8*)&k, (uint8*)&v);

    memcpy(k.string, "ABCD", 5);
    k.number = 42;
    v.x = -100;
    v.y = -200;
    dict_insert(dict, (uint8*)&k, (uint8*)&v);
    
    dict_free(allocator, dict);
    return error;
}