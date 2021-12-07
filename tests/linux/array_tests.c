#include "allocator_implementation.h"
#include "array.h"
#include <assert.h>


int main(int argc, char *argv[])
{
    Allocator *allocator = allocator_create();

    Array* array = array_create(allocator, 10, sizeof(int32));

    int32 number, i;
    for (i = 0; i < array->length; i++)
    {
        number = i + 1;
        array_insert(array, i, (uint8*)&number);    
    }

    number = -1;
    for ( i = 0; i < array->length; i++)
    {
        array_get(array, i, (uint8*)&number);
        assert(number == i + 1);
    }

    array_free(allocator, array);

    allocator_free(allocator);
    return 0;
}
