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


// Allocate memory and initialize the dict.
// Returns NULL if max_members * member_sixe == 0.
Dict* dict_new(AllocatorInterface*, uint32 max_members, uint32 key_size, uint32 value_size);

// Set the key to point to the provided value.
// Adds a new value if key does not exist,
// otherwise updates the value.
// Fails silently if not enough space.
// 
// Memory pointed by key must be atleast dict.key_size bytes.
// Memory pointed by value must be atleast dict.value_size bytes.
//
// Note that if more than 2/3 of the max
// capacity of the dict is used, performance
// will decrease due to hash collisions.
void dict_set(Dict*, uint8* key, uint8* value);

// Copy value associated with the provided key into the
// provided memory location. This must be atleast dict.value_size bytes.
// Memory pointed by key must be atleast dict.key_size bytes.
// Fails silently if key is not found.
void dict_get(Dict*, uint8* key, uint8*);

// Move value associated with the provided key into the
// provided memory location. This must be atleast dict.value_size bytes.
// Removes the key and its value from the dict.
// Memory pointed by key must be atleast dict.key_size bytes.
// Fails silently if key is not found.
void dict_pop(Dict*, uint8* key, uint8*);

// Copy the dict keys into a new array.
Array* dict_copy_keys(Dict*, AllocatorInterface*);

// Copy the dict values into a new array.
Array* dict_copy_values(Dict*, AllocatorInterface*);

// Copy the dict items into a new array.
// The resulting array will have member_size of
// dict.key_size + dict.key_values. These values
// are stored in the same 'slot' in the array consecutively.
Array* dict_copy_items(Dict*, AllocatorInterface*);

// Free all memory used by the dict.
void dict_destroy(Dict*, AllocatorInterface*);

#endif