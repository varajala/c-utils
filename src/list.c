#include <stddef.h>
#include "datastructures/list.h"


List* list_create(Allocator *allocator, uint32 member_size)
{
    return NULL;
}


void list_get(List *list, uint32 index, uint8 *memory)
{

}


void list_insert(List *list, uint32 index, uint8 *memory)
{

}


void list_remove(List *list, uint32 index)
{

}


void list_copy_memory(List *list, uint8 *memory, uint32 max_size)
{

}


Array* list_create_slice(List *list, uint32 start, uint32 end)
{
    return NULL;
}


void list_foreach(List *list, void (*func)(uint8*))
{

}


void list_sort(List *list, enum ComparisonResult (*compare)(uint8*, uint8*))
{

}

void list_free(List *list)
{

}
