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


int test_stack_pop_and_peek(Allocator *allocator)
{
    int error;
    char *str = "ABCD";
    char c = 0x00;
    
    Stack *stack = stack_create(allocator, STACK_INITIAL_SIZE, 1);
    stack->member_count = 4;
    memcpy(stack->data, str, 4);

    error = 10;

    stack_pop(stack, (uint8*) &c);
    error -= c == 'D';
    error -= stack->member_count == 3;
    c = 0x00;

    stack_peek(stack, (uint8*) &c);
    error -= c == 'C';
    error -= stack->member_count == 3;
    c = 0x00;

    stack_pop(stack, (uint8*) &c);
    error -= c == 'C';
    error -= stack->member_count == 2;
    c = 0x00;

    stack_pop(stack, (uint8*) &c);
    error -= c == 'B';
    error -= stack->member_count == 1;
    c = 0x00;

    stack_peek(stack, (uint8*) &c);
    error -= c == 'A';
    error -= stack->member_count == 1;
    c = 0x00;

    stack_pop(stack, (uint8*) &c);
    error -= c == 'A';
    error -= stack->member_count == 0;
    c = 0x00;

    stack_pop(stack, (uint8*) &c);
    error -= c == 0x00;
    error -= stack->member_count == 0;
    c = 0x00;
    
    stack_free(stack);
    return error;
}