#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef List Queue;

// Allocate memory and initialize the queue.
// All bytes are initialized to 0x00.
// Returns NULL if max_members * member_sixe == 0.
Queue* queue_new(AllocatorInterface*, uint32 max_members, uint32 member_size);

// Move the first element from queue into
// the specified memory location. This location must be
// atleast queue.member_size bytes.
// Fails silently if no items are queued.
void queue_pop(Queue*, uint8*);

// Copy the first element from queue into
// the specified memory location. This location must be
// atleast queue.member_size bytes.
// Does not remove the item from queue.
// Fails silently if no items are queued.
void queue_peek(Queue*, uint8*);

// Copy queue.member_size bytes from the
// the specified memory location into the queue.
// Fails silently if not enough space.
void queue_push(Queue*, uint8*);

// Free all memory used by the queue.
void queue_destroy(Queue*, AllocatorInterface*);

#endif