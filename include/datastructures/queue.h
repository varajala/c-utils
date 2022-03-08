#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef List Queue;


/*
 Allocate memory and initialize the queue.
 The queue stores the pointer to the allocator,
 it must be valid for entire lifetime of the queue.
*/
Queue* queue_new(AllocatorInterface*, uint32 max_members, uint32 member_size);

void queue_pop(Queue*, uint8*);

void queue_peek(Queue*, uint8*);

/*
 Push the data into the queue and reallocate the queue if needed.
 Returns the pointer to the queue, which can be different than
 the pointer passed in as argument.
*/
void queue_push(Queue*, uint8*);

void queue_destroy(Queue*, AllocatorInterface*);

#endif