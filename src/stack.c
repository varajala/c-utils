#include "datastructures/stack.h"


Stack* stack_create(Allocator *allocator, uint32 initial_member_count, uint32 member_size)
{
    return (Stack*) list_create(allocator, initial_member_count, member_size);
}


void stack_pop(Stack *stack, uint8 *memory)
{
    if (stack == NULL)
        return;
    
    list_get((List*) stack, stack->member_count - 1, memory);
    list_remove_at((List*) stack, stack->member_count - 1);
}


void stack_peek(Stack *stack, uint8 *memory)
{
    if (stack == NULL)
        return;
    
    list_get((List*) stack, stack->member_count - 1, memory);
}


void stack_push(Stack *stack, uint8 *memory)
{
    list_append((List*) stack, memory);
}


void stack_free(Stack *stack)
{
    list_free((List*) stack);
}