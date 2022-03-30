// This software is licensed under the MIT License:
//
// Copyright (c) 2022 Valtteri Rajalainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _C_UTILS_H
#define _C_UTILS_H

#ifndef NULL
    #define NULL ((void*)0x00)
#endif

// This librabry is targeted for 64-bit platforms.
// If you wish to port this library for other targets,
// change the defenitions of the following integer types.
#define PLATFORM_POINTER_LENGTH 8

#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long int

#define int8 char
#define int16 short
#define int32 int
#define int64 long int


enum ComparisonResult
{
    COMPARISON_RESULT_FIRST_IS_LARGER,
    COMPARISON_RESULT_ARE_EQUAL,
    COMPARISON_RESULT_FIRST_IS_SMALLER
};


typedef struct AllocatorInterface
{
    // Should allocate a continuous block of memory
    // with the minimium size of provided number of bytes.
    // Returns NULL if allocation fails for any reason.
    void* (*memory_allocate) (uint64);
    
    // Should resize the provided memory block into
    // a new continuous memory block with minimium size
    // of provided number of bytes. The memory from
    // the old block must be copied into the new block
    // for the length of MIN(old_size, new_size) and
    // the old memory block block must be freed.
    // If allocation fails, pointer to the old
    // memory block must be returned.
    void* (*memory_resize)   (void *memory, uint64);

    // Mark the memory in segment [memory, memory + n] freed.
    void  (*memory_free)     (void *memory, uint64);
} AllocatorInterface;


#define BUMP_ALLOCATOR_BUFFER_OFFSET 128

typedef struct BumpAllocator
{
    uint64 buffer_size;
    uint64 end_index;
    uint8 buffer[];
} BumpAllocator;


#define ARENA_ALLOCATOR_ARENA_ARRAY_OFFSET (PLATFORM_POINTER_LENGTH + 128)

typedef struct ArenaAllocator
{
    void* (*get_memory)(uint64);
    uint64 arena_size;
    uint32 max_arenas;
    uint32 num_arenas;
    BumpAllocator *arenas[];
} ArenaAllocator;


#define ARRAY_DATA_OFFSET 64

typedef struct Array
{
    uint32 member_count;
    uint32 member_size;
    uint8 data[];
} Array;


#define LIST_DATA_OFFSET 128

typedef struct List
{
    uint64 _allocated_space;
    uint32 member_count;
    uint32 member_size;
    uint8 data[];
} List;


typedef struct Dict {
    uint32 _num_slots;
    uint32 member_count;
    Array *index_table;
    List *keys;
    List *values;
} Dict;


// Calculate the total space used by a bump allocator based on its buffer size.
static inline uint64 bump_allocator_size(uint64 buf_size)
{
    return BUMP_ALLOCATOR_BUFFER_OFFSET + buf_size;
}

// Initialize the bump allocator, memory needs to be allocated by the caller.
void bump_allocator_init(BumpAllocator*, uint64);

// Reset the end_index to point to the start of buffer.
// Does not overwrite the previously allocated data.
void bump_allocator_reset(BumpAllocator*);

// Allocate memory, returns NULL if out of memory or an error occurs.
void* bump_allocator_memory_allocate(BumpAllocator*, uint64);

// Calculate the memory used by an arena allocator based on its max_arenas.
// The created bumo allocators are allocated dynamically when needed,
// these are not included in the total.
static inline uint64 arena_allocator_size(uint32 max_arenas)
{
    return ARENA_ALLOCATOR_ARENA_ARRAY_OFFSET + (max_arenas * PLATFORM_POINTER_LENGTH);
}

// Initialize the ArenaAllocator. Memory needs to be allocated by the caller.
// The pointer to the get_memory-function must be valid for the entire lifetime of
// the ArenaAllocator.
void arena_allocator_init(ArenaAllocator*, void* (*get_memory)(uint64), uint64 arena_size, uint32 max_arenas);

// Allocate memory, returns NULL if out of memory, or an error occurs.
void* arena_allocator_memory_allocate(ArenaAllocator*, uint64);

// Reset all allocated BumpAllocators. Does not overwrite data,
// or free already allocated buffers.
void arena_allocator_reset(ArenaAllocator*);

// Free all memory used by the ArenaAllocator,
// including dynamically allocated bump allocators.
void arena_allocator_destroy(ArenaAllocator*, void (*free_memory)(void*, uint64));

// Allocate space and initialize the array.
// All bytes are initialized into 0x00 values.
// Returns NULL instead of an empty array.
Array* array_new(AllocatorInterface*, uint32 member_count, uint32 member_size);

// Allocate space and initialize the array.
// Returns NULL instead of an empty array.
Array* array_new_no_init(AllocatorInterface*, uint32 member_count, uint32 member_size);

// Copy memory from array at the provided index to the provided address.
// The provided memory must be atleast array.member_size bytes.
void array_get(Array*, uint32 index, uint8*);

// Copy memory into the array from to the provided address
// at the specified index for a total of array.member_size bytes.
void array_set(Array*, uint32 index, uint8*);

// Copy memory into the array from to the provided address
// for a total length of MIN(array.member_count * array.member_size, max_length).
// Overwrites exiting members.
void array_copy_memory(Array*, uint8*, uint64 max_length);

// Create a new array and copy the original array's data into it
// from start_index to end_index.
Array* array_create_slice(Array*, AllocatorInterface*, uint32 start_index, uint32 end_index);

// Copy the old array and execute the provided function
// for every member in the copied array.
Array* array_map(Array*, AllocatorInterface*, void (*func)(uint8*));

// Create a new array with the members of the provided array
// for which the provided test function returns non zero value.
// Returns NULL instead of an empty array.
Array* array_filter(Array*, AllocatorInterface*, int (*func)(uint8*));

// Create a reversed copy of the given array.
Array* array_reverse(Array*, AllocatorInterface*);

// Return the first index after the given start index 
// where the test function returns a non zero value.
// If no mathces are found, or an error occurs, negative value is returned.
int64 array_find_index(Array*, uint32 start_index, int (*func)(uint8*));

// Find the first element starting from the given start index
// where the test function returns a non zero value.
// Copies memory for a total length of array.member_size into the given memory location.
void array_find_item(Array*, uint32 start_index, int (*func)(uint8*), uint8*);

// Execute the given function for every member of the array.
void array_foreach(Array*, void (*func)(uint8*));

// Execute the given function for every member of the array.
// This function recieves three arguments:
// previous_value, current_value, result.
// If there is no previous value, use the result instead.
// 
// EXAMPLE calculate sum of array of integers:
//
// void sum_integers(uint8 *prev, uint8 *current, uint8 *result) { int n = *current; *result += n; }
// int result = 0;
// array_reduce(array, sum_integers, &result);
//
void array_reduce(Array*, void (*func)(uint8*, uint8*, uint8*), uint8 *result);

// Sort the array in place.
void array_sort(Array*, enum ComparisonResult (*compare)(uint8*, uint8*));

// Free memory used by the array.
void array_destroy(Array*, AllocatorInterface*);

// Safely cast List* to Array*
static inline Array* list_to_array(List *list)
{
    return (Array*) &list->member_count;
}

// Allocate memory and initialize the list.
// All bytes are initialized to 0x00.
// Returns NULL if max_members * member_sixe == 0.
List* list_new(AllocatorInterface*, uint32 max_members, uint32 member_size);

// Resize the list.
// If max_members * member_size is smaller than currently allocated space,
// the remaining data will be discarded and marked as freed.
//
// Returns the given pointer to the list if resizing failed,
// pointer to new list otherwise. Assumes that the provided allocator
// is implemented correctly and allocator.memory_resize
// copies the data into the new memory block and frees the
// old list.
List* list_resize(List *list, AllocatorInterface *allocator, uint32 max_members);

// Copy memory from the list at the provided index to the provided address.
// The provided memory must be atleast list.member_size bytes.
void list_get(List*, uint32 index, uint8*);

// Copy memory into the list from to the provided address
// into the specified index for a total of list.member_size bytes.
// Does not increment the member_count, does replacement instead.
void list_set(List*, uint32 index, uint8*);

// Copy memory into the list from to the provided address
// into the specified index for a total of list.member_size bytes.
// Increments the member_count, always adds the new item without effecting existing ones.
// Fails silently if there is not enough memory for the addition.
void list_insert(List*, uint32 index, uint8*);

// Copy memory into the end of the list from to the provided address
// for a total of list.member_size bytes. Increments the member_count,
// always adds the new item without effecting existing ones.
// Fails silently if there is not enough memory for the addition.
void list_append(List*, uint8*);

// Remove the element at the specified index from list.
void list_remove_at(List*, uint32 index);

// Copy memory into the list from to the provided address
// for a total length of MIN(list.max_members * list.member_size, max_length).
// Overwrites exiting members.
void list_copy_memory(List*, uint8*, uint32 length);

// Create a new array and copy the list's data into it
// from start_index to end_index.
Array* list_create_slice(List*, AllocatorInterface*, uint32 start_index, uint32 end_index);

// Copy the list members into an array and execute the provided function
// for every member in the copied array.
Array* list_map(List*, AllocatorInterface*, void (*func)(uint8*));

// Create a new array with the members of the provided list
// for which the provided test function returns non zero value.
// Returns NULL instead of an empty array.
Array* list_filter(List*, AllocatorInterface*, int (*func)(uint8*));

// Create a reversed array from the given list.
Array* list_reverse(List*, AllocatorInterface*);

// Return the first index after the given start index 
// where the test function returns a non zero value.
// If no mathces are found, or an error occurs, negative value is returned.
int64 list_find_index(List*, uint32 start_index, int (*func)(uint8*));

// Find the first element starting from the given start index
// where the test function returns a non zero value.
// Copies memory for a total length of list.member_size into the given memory location.
void list_find_item(List*, uint32 start_index, int (*func)(uint8*), uint8*);

// Execute the given function for every member of the list.
void list_foreach(List*, void (*func)(uint8*));

// Execute the given function for every member of the list.
// This function recieves three arguments:
// previous_value, current_value, result.
// If there is no previous value, use the result instead.
// 
// EXAMPLE calculate sum of list of integers:
//
// void sum_integers(uint8 *prev, uint8 *current, uint8 *result) { int n = *current; *result += n; }
// int result = 0;
// list_reduce(list, sum_integers, &result);
//
void list_reduce(List*, void (*func)(uint8*, uint8*, uint8*), uint8*);

// Sort the list in place.
void list_sort(List*, enum ComparisonResult (*compare)(uint8*, uint8*));

// Free memory used by the list.
void list_destroy(List*, AllocatorInterface *allocator);

// Allocate memory and initialize the dict.
// Returns NULL if max_members * member_sixe == 0.
Dict* dict_new(AllocatorInterface*, uint32 max_members, uint32 key_size, uint32 value_size);

// Test if the specified key is in the dict.
// Returns 1 if key is found, 0 otherwise.
int dict_contains_key(Dict*, uint8 *key);

// Set the key to point to the provided value.
// Adds a new value if key does not exist,
// otherwise updates the value.
// Fails silently if not enough space.
// 
// Memory pointed by key must be atleast dict.key_size bytes.
// Memory pointed by value must be atleast dict.value_size bytes.
// Note that any unitialized struct padding can interfere with the hashing.
//
// Also note that if more than 2/3 of the max
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
