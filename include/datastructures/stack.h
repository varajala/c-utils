#ifndef STACK_H
#define STACK_H

#include "list.h"

typedef List Stack;


Stack* stack_create(Allocator*, uint32, uint32);

void stack_pop(Stack*, uint8*);

void stack_peek(Stack*, uint8*);

Stack* stack_push(Stack*, uint8*);

void stack_free(Stack*);

#endif