#include "datastructures/stack.h"


inline Stack* stack_new(AllocatorInterface *allocator, uint32 initial_member_count, uint32 member_size)
{
    return (Stack*) list_new(allocator, initial_member_count, member_size);
}


inline void stack_pop(Stack *stack, uint8 *memory)
{
    if (stack == NULL)
        return;
    
    list_get((List*) stack, stack->member_count - 1, memory);
    list_remove_at((List*) stack, stack->member_count - 1);
}


inline void stack_peek(Stack *stack, uint8 *memory)
{
    if (stack == NULL)
        return;
    
    list_get((List*) stack, stack->member_count - 1, memory);
}


inline void stack_push(Stack *stack, uint8 *memory)
{
    list_append((List*) stack, memory);
}


inline void stack_destroy(Stack *stack, AllocatorInterface *allocator)
{
    list_destroy((List*) stack, allocator);
}