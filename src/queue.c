#include "datastructures/queue.h"


Queue* queue_create(Allocator *allocator, uint32 initial_member_count, uint32 member_size)
{
    return (Queue*) list_create(allocator, initial_member_count, member_size);
}


void queue_pop(Queue *queue, uint8 *memory)
{
    if (queue == NULL)
        return;
    
    list_get((List*) queue, queue->member_count - 1, memory);
    list_remove_at((List*) queue, queue->member_count - 1);
}


void queue_peek(Queue *queue, uint8 *memory)
{
    if (queue == NULL)
        return;
    
    list_get((List*) queue, queue->member_count - 1, memory);
}


void queue_push(Queue *queue, uint8 *memory)
{
    list_insert((List*) queue, 0, memory);
}


void queue_free(Allocator *allocator, Queue *queue)
{
    list_free(allocator, (List*) queue);
}
