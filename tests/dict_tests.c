#define INITIAL_DICT_SIZE 16

typedef struct DictKey {
    char string[8];
    uint32 number;
} DictKey;

typedef struct DictValue {
    int x;
    int y;
} DictValue;

typedef struct DictItem {
    DictKey key;
    DictValue value;
} DictItem;


int test_dict_creation(AllocatorInterface *allocator)
{
    int error = 0;
    Dict *dict = dict_new(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));
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

    dict_destroy(dict, allocator);
    return error;
}


int test_dict_usage(AllocatorInterface *allocator)
{
    int error = 0;
    DictKey k;
    DictValue v;

    Dict *dict = dict_new(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));
    if (dict == NULL)
        return 1;

    memset(k.string, 0x00, 8);
    memcpy(k.string, "ABCD", 5);
    k.number = 42;
    v.x = 100;
    v.y = 200;
    dict_set(dict, (uint8*)&k, (uint8*)&v);

    memcpy(k.string, "IJKL", 5);
    k.number = 42;
    v.x = 300;
    v.y = 400;
    dict_set(dict, (uint8*)&k, (uint8*)&v);


    memcpy(k.string, "ABCD", 5);
    if (!(dict_contains_key(dict, (uint8*)&k)))
    {
        error = 1;
        goto cleanup;
    }

    memcpy(k.string, "IJKL", 5);
    if (!(dict_contains_key(dict, (uint8*)&k)))
    {
        error = 1;
        goto cleanup;
    }

    memcpy(k.string, "ABCD", 5);
    dict_get(dict, (uint8*)&k, (uint8*)&v);
    if (!(v.x == 100 && v.y == 200))
    {
        error = 1;
        goto cleanup;
    }

    memcpy(k.string, "IJKL", 5);
    dict_get(dict, (uint8*)&k, (uint8*)&v);
    if (!(v.x == 300 && v.y == 400))
    {
        error = 1;
        goto cleanup;
    }

    memcpy(k.string, "ABCD", 5);
    k.number = 42;
    v.x = -100;
    v.y = -200;
    dict_set(dict, (uint8*)&k, (uint8*)&v);

    memcpy(k.string, "ABCD", 5);
    dict_pop(dict, (uint8*)&k, (uint8*)&v);
    if (!(v.x == -100 && v.y == -200))
    {
        error = 1;
        goto cleanup;
    }

    cleanup:
        dict_destroy(dict, allocator);
    return error;
}


const DictItem dict_test_data[INITIAL_DICT_SIZE] = {
    {
        { "ABCD\0\0\0\0", 1 },
        { .x = 100, .y = 200 }
    },
    {
        { "EFGH\0\0\0\0", 2 },
        { .x = 300, .y = 400 }
    },
    {
        { "IJKL\0\0\0\0", 3 },
        { .x = 500, .y = 600 }
    },
    {
        { "MNOP\0\0\0\0", 4 },
        { .x = 700, .y = 800 }
    },
};


int test_dict_copy_keys(AllocatorInterface *allocator)
{
    const DictKey expected_keys[INITIAL_DICT_SIZE] = {
        { "ABCD\0\0\0\0", 1 },
        { "EFGH\0\0\0\0", 2 },
        { "IJKL\0\0\0\0", 3 },
        { "MNOP\0\0\0\0", 4 },
    };

    Dict *dict = dict_new(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));

    dict_set(dict, (uint8*)&dict_test_data[0].key, (uint8*)&dict_test_data[0].value);
    dict_set(dict, (uint8*)&dict_test_data[1].key, (uint8*)&dict_test_data[1].value);
    dict_set(dict, (uint8*)&dict_test_data[2].key, (uint8*)&dict_test_data[2].value);
    dict_set(dict, (uint8*)&dict_test_data[3].key, (uint8*)&dict_test_data[3].value);

    Array *dict_keys = dict_copy_keys(dict, allocator);

    int error = memcmp(dict_keys->data, expected_keys, 4 * sizeof(DictKey));

    array_destroy(dict_keys, allocator);
    dict_destroy(dict, allocator);
    return !(error == 0);
}


int test_dict_copy_values(AllocatorInterface *allocator)
{
    const DictValue expected_values[INITIAL_DICT_SIZE] = {
        { .x = 100, .y = 200 },
        { .x = 300, .y = 400 },
        { .x = 500, .y = 600 },
        { .x = 700, .y = 800 },
    };

    Dict *dict = dict_new(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));

    dict_set(dict, (uint8*)&dict_test_data[0].key, (uint8*)&dict_test_data[0].value);
    dict_set(dict, (uint8*)&dict_test_data[1].key, (uint8*)&dict_test_data[1].value);
    dict_set(dict, (uint8*)&dict_test_data[2].key, (uint8*)&dict_test_data[2].value);
    dict_set(dict, (uint8*)&dict_test_data[3].key, (uint8*)&dict_test_data[3].value);

    Array *dict_values = dict_copy_values(dict, allocator);

    int error = memcmp(dict_values->data, expected_values, 4 * sizeof(DictValue));

    array_destroy(dict_values, allocator);
    dict_destroy(dict, allocator);
    return !(error == 0);
}


int test_dict_copy_items(AllocatorInterface *allocator)
{
    Dict *dict = dict_new(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));

    dict_set(dict, (uint8*)&dict_test_data[0].key, (uint8*)&dict_test_data[0].value);
    dict_set(dict, (uint8*)&dict_test_data[1].key, (uint8*)&dict_test_data[1].value);
    dict_set(dict, (uint8*)&dict_test_data[2].key, (uint8*)&dict_test_data[2].value);
    dict_set(dict, (uint8*)&dict_test_data[3].key, (uint8*)&dict_test_data[3].value);

    Array *dict_items = dict_copy_items(dict, allocator);

    int error = memcmp(dict_items->data, dict_test_data, 4 * sizeof(DictItem));

    array_destroy(dict_items, allocator);
    dict_destroy(dict, allocator);
    return !(error == 0);
}


int test_dict_resize(AllocatorInterface *allocator)
{
    int err;
    Dict *dict = dict_new(allocator, INITIAL_DICT_SIZE, sizeof(DictKey), sizeof(DictValue));

    dict_set(dict, (uint8*)&dict_test_data[0].key, (uint8*)&dict_test_data[0].value);
    dict_set(dict, (uint8*)&dict_test_data[1].key, (uint8*)&dict_test_data[1].value);
    dict_set(dict, (uint8*)&dict_test_data[2].key, (uint8*)&dict_test_data[2].value);
    dict_set(dict, (uint8*)&dict_test_data[3].key, (uint8*)&dict_test_data[3].value);

    err = dict_resize(dict, allocator, 2 * INITIAL_DICT_SIZE);
    if (err)
        goto cleanup;

    if (!dict_contains_key(dict, (uint8*)&dict_test_data[0].key))
    {
        err = -1;
        goto cleanup;
    }

    if (!dict_contains_key(dict, (uint8*)&dict_test_data[1].key))
    {
        err = 2;
        goto cleanup;
    }

    if (!dict_contains_key(dict, (uint8*)&dict_test_data[2].key))
    {
        err = 3;
        goto cleanup;
    }

    if (!dict_contains_key(dict, (uint8*)&dict_test_data[3].key))
    {
        err = 4;
        goto cleanup;
    }

    cleanup:
        dict_destroy(dict, allocator);
    return err;
}
