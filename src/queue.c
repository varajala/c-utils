#include "datastructures/queue.h"


Queue* queue_new(AllocatorInterface *allocator, uint32 initial_member_count, uint32 member_size)
{
    return (Queue*) list_new(allocator, initial_member_count, member_size);
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


void queue_destroy(AllocatorInterface *allocator, Queue *queue)
{
    list_destroy(allocator, (List*) queue);
}
