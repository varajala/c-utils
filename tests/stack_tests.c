#include "datastructures/stack.h"


const uint32 STACK_INITIAL_SIZE = 16;


int test_stack_push(Allocator *allocator)
{
    int error, number;
    int numbers[3] = {2, 4, 8};
    error = 0;
    
    Stack *stack = stack_create(allocator, STACK_INITIAL_SIZE, sizeof(int));

    number = 2;
    stack_push(stack, (uint8*) &number);

    number = 4;
    stack_push(stack, (uint8*) &number);

    number = 8;
    stack_push(stack, (uint8*) &number);

    error = memcmp(stack->data, numbers, 3 * sizeof(int)) != 0;

    stack_free(stack);
    return error;
}