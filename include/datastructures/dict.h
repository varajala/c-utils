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


Dict* dict_create(Allocator*, uint32, uint32, uint32);

void dict_set(Dict*, uint8*, uint8*);

void dict_get(Dict*, uint8*, uint8*);

void dict_pop(Dict*, uint8*, uint8*);

void dict_free(Allocator*, Dict*);

#endif