#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef List Queue;


Queue* queue_create(Allocator*, uint32, uint32);

void queue_pop(Queue*, uint8*);

void queue_peek(Queue*, uint8*);

Queue* queue_push(Queue*, uint8*);

void queue_free(Queue*);

#endif