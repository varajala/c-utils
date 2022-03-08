#include "datastructures/queue.h"


inline Queue* queue_new(AllocatorInterface *allocator, uint32 initial_member_count, uint32 member_size)
{
    return (Queue*) list_new(allocator, initial_member_count, member_size);
}


inline void queue_pop(Queue *queue, uint8 *memory)
{
    if (queue == NULL)
        return;
    
    list_get((List*) queue, queue->member_count - 1, memory);
    list_remove_at((List*) queue, queue->member_count - 1);
}


inline void queue_peek(Queue *queue, uint8 *memory)
{
    if (queue == NULL)
        return;
    
    list_get((List*) queue, queue->member_count - 1, memory);
}


inline void queue_push(Queue *queue, uint8 *memory)
{
    list_insert((List*) queue, 0, memory);
}


inline void queue_destroy(Queue *queue, AllocatorInterface *allocator)
{
    list_destroy((List*) queue, allocator);
}
