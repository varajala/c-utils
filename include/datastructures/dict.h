#ifndef DICT_H
#define DICT_H

#include "datastructures/list.h"


typedef struct Dict {
    uint32 _num_slots;
    uint32 member_count;
    Array *index_table;
    List *keys;
    List *values;
} Dict;


Dict* dict_new(AllocatorInterface*, uint32 max_members, uint32 key_size, uint32 value_size);

void dict_set(Dict*, uint8* key, uint8* value);

void dict_get(Dict*, uint8* key, uint8*);

void dict_pop(Dict*, uint8* key, uint8*);

Array* dict_copy_keys(Dict*, AllocatorInterface*);

Array* dict_copy_values(Dict*, AllocatorInterface*);

Array* dict_copy_items(Dict*, AllocatorInterface*);

void dict_destroy(AllocatorInterface*, Dict*);

#endif