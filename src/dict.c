#include "datastructures/dict.h"

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
    return (hash(key, key_length) + tries) % slots;
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


Dict* dict_create(Allocator *allocator, uint32 max_members, uint32 key_size, uint32 value_size)
{
    if (allocator == NULL)
        return NULL;
    
    Dict *dict = allocator->memory_allocate(sizeof(Dict));
    if (dict == NULL)
        return NULL;

    Array *index_table = array_create(allocator, max_members, 8);
    if (index_table == NULL)
    {
        allocator->memory_free(dict, sizeof(dict));
        return NULL;
    }
    
    
    List *key_list = list_create(allocator, max_members, key_size);
    if (key_list == NULL)
    {
        allocator->memory_free(dict, sizeof(dict));
        array_free(allocator, index_table);
        return NULL;
    }
    
    List *value_list = list_create(allocator, max_members, value_size);
    if (value_list == NULL)
    {
        allocator->memory_free(dict, sizeof(dict));
        array_free(allocator, index_table);
        list_free(allocator, key_list);
        return NULL;
    }

    dict->_num_slots = max_members;
    dict->member_count = 0;
    dict->index_table = index_table;
    dict->keys = key_list;
    dict->values = value_list;

    for (int i = 0; i < max_members; i++)
    {
        array_insert(index_table, i, (uint8*)&EMPTY_SLOT);
    }

    return dict;
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
            array_insert(dict->index_table, index, (uint8*)&slot_value);
            list_append(dict->keys, key);
            list_append(dict->values, value);
            dict->member_count++;
            break;
        }

        // Check if key is the same, and do updating instead...
        key_value = &dict->keys->data[slot_value];
        if (mem_equals(key, key_value, key_size)) {
            array_insert(list_to_array(dict->values), slot_value, value);
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
}


void dict_free(Allocator *allocator, Dict* dict)
{
    if (allocator == NULL || dict == NULL)
        return;

    if (dict->index_table != NULL)
        array_free(allocator, dict->index_table);
    
    if (dict->keys != NULL)
        list_free(allocator, dict->keys);

    if (dict->values != NULL)
        list_free(allocator, dict->values);

    allocator->memory_free(dict, sizeof(dict));
}