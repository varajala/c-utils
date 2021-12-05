#include "allocator_implementation.h"
#include "array.h"


int main(int argc, char *argv[])
{
    Allocator *allocator = allocator_create();

    Array* array = array_create(allocator, 10, sizeof(int));
    array_free(allocator, array);

    allocator_free(allocator);
    return 0;
}
