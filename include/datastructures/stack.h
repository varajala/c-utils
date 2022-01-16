#ifndef STACK_H
#define STACK_H

#include "list.h"

typedef List Stack;


Stack* stack_create(Allocator*, uint32, uint32);

uint8* stack_pop(Stack*);

uint8* stack_peek(Stack*);

void stack_push(Stack*, uint8*);

void stack_free(Stack*);

#endif