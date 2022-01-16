#include "datastructures/stack.h"


Stack* stack_create(Allocator *allocator, uint32 initial_member_count, uint32 member_size)
{
    return (Stack*) list_create(allocator, initial_member_count, member_size);
}


uint8* stack_pop(Stack *stack)
{
    return NULL;
}


uint8* stack_peek(Stack *stack)
{
    return NULL;
}


void stack_push(Stack *stack, uint8 *memory)
{

}


void stack_free(Stack *stack)
{
    list_free((List*) stack);
}