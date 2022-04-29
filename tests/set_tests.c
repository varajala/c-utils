#define INITIAL_SET_SIZE 16

typedef struct SetItem {
    float x, y, z;
} SetItem;


int test_set_usage(AllocatorInterface *allocator)
{
    int error = 0;
    SetItem item;
    Set *set = set_new(allocator, INITIAL_SET_SIZE, sizeof(SetItem));
    if (set == NULL)
        return 1;

    item.x = 1;
    item.y = 2;
    item.z = 3;
    set_add(set, (uint8*)&item);
    if (!set_contains_item(set, (uint8*)&item))
    {
        error = 1;
        goto cleanup;
    }

    item.z = 1;
    item.x = 3;
    if (set_contains_item(set, (uint8*)&item))
    {
        error = 1;
        goto cleanup;
    }

    set_add(set, (uint8*)&item);
    if (!set_contains_item(set, (uint8*)&item))
    {
        error = 1;
        goto cleanup;
    }

    item.x = 1;
    item.z = 3;
    set_remove(set, (uint8*)&item);
    if (set_contains_item(set, (uint8*)&item))
    {
        error = 1;
        goto cleanup;
    }

    cleanup:
        set_destroy(set, allocator);
    return error;
}


const SetItem set_test_data[INITIAL_SET_SIZE] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 },
    { 10, 11, 12 },
    { 13, 14, 15 },
    { 16, 17, 18 },
};


int test_set_copy_items(AllocatorInterface *allocator)
{
    const SetItem expected_items[INITIAL_SET_SIZE] = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
        { 10, 11, 12 },
        { 13, 14, 15 },
        { 16, 17, 18 },
    };

    Set *set = set_new(allocator, INITIAL_SET_SIZE, sizeof(SetItem));

    set_add(set, (uint8*)&set_test_data[0]);
    set_add(set, (uint8*)&set_test_data[1]);
    set_add(set, (uint8*)&set_test_data[2]);
    set_add(set, (uint8*)&set_test_data[3]);
    set_add(set, (uint8*)&set_test_data[4]);
    set_add(set, (uint8*)&set_test_data[5]);
    set_add(set, (uint8*)&set_test_data[6]);

    Array *set_items = set_copy_items(set, allocator);

    int error = memcmp(set_items->data, expected_items, 6 * sizeof(SetItem));

    array_destroy(set_items, allocator);
    set_destroy(set, allocator);
    return !(error == 0);
}


int test_set_resize(AllocatorInterface *allocator)
{
    int err;
    Set *set = set_new(allocator, INITIAL_SET_SIZE, sizeof(SetItem));

    set_add(set, (uint8*)&set_test_data[0]);
    set_add(set, (uint8*)&set_test_data[1]);
    set_add(set, (uint8*)&set_test_data[2]);
    set_add(set, (uint8*)&set_test_data[3]);

    err = set_resize(set, allocator, 2 * INITIAL_SET_SIZE);
    if (err)
        goto cleanup;

    if (!set_contains_item(set, (uint8*)&set_test_data[0]))
    {
        err = 1;
        goto cleanup;
    }

    if (!set_contains_item(set, (uint8*)&set_test_data[1]))
    {
        err = 1;
        goto cleanup;
    }

    if (!set_contains_item(set, (uint8*)&set_test_data[2]))
    {
        err = 1;
        goto cleanup;
    }

    if (!set_contains_item(set, (uint8*)&set_test_data[3]))
    {
        err = 1;
        goto cleanup;
    }

    err = set->_num_slots != 2 * INITIAL_SET_SIZE;

    cleanup:
        set_destroy(set, allocator);
    return err;
}
