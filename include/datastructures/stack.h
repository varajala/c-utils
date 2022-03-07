#ifndef STACK_H
#define STACK_H

#include "list.h"

typedef List Stack;


/*
 Allocate memory and initialize the stack.
 The stack stores the pointer to the allocator,
 it must be valid for entire lifetime of the stack.
*/
Stack* stack_new(AllocatorInterface*, uint32 max_members, uint32 member_size);

void stack_pop(Stack*, uint8*);

void stack_peek(Stack*, uint8*);

/*
 Push the data into the stack and reallocate the stack if needed.
 Returns the pointer to the stack, which can be different than
 the pointer passed in as argument.
*/
void stack_push(Stack*, uint8*);

void stack_destroy(AllocatorInterface*, Stack*);

#endif