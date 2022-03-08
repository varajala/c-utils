#ifndef STACK_H
#define STACK_H

#include "list.h"

typedef List Stack;

// Allocate memory and initialize the stack.
// All bytes are initialized to 0x00.
// Returns NULL if max_members * member_sixe == 0.
Stack* stack_new(AllocatorInterface*, uint32 max_members, uint32 member_size);

// Move the first element from stack into
// the specified memory location. This location must be
// atleast stack.member_size bytes.
// Fails silently if stack is empty.
void stack_pop(Stack*, uint8*);

// Copy the first element from stack into
// the specified memory location. This location must be
// atleast stack.member_size bytes.
// Does not remove the item from stack.
// Fails silently if stack is empty.
void stack_peek(Stack*, uint8*);

// Copy stack.member_size bytes from the
// the specified memory location into the stack.
// Fails silently if not enough space.
void stack_push(Stack*, uint8*);

// Free all memory used by the queue.
void stack_destroy(Stack*, AllocatorInterface*);

#endif