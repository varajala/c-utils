#include "datastructures/queue.h"


const uint32 QUEUE_INITIAL_SIZE = 16;


int test_queue_push(Allocator *allocator)
{
    int error, number;
    int numbers[3] = {8, 4, 2};
    error = 0;
    
    Queue *queue = queue_create(allocator, QUEUE_INITIAL_SIZE, sizeof(int));

    number = 2;
    queue = queue_push(queue, (uint8*) &number);

    number = 4;
    queue = queue_push(queue, (uint8*) &number);

    number = 8;
    queue = queue_push(queue, (uint8*) &number);

    error = memcmp(queue->data, numbers, 3 * sizeof(int)) != 0;

    queue_free(queue);
    return error;
}


int test_queue_pop_and_peek(Allocator *allocator)
{
    int error;
    char *str = "ABCD";
    char c = 0x00;
    
    Queue *queue = queue_create(allocator, STACK_INITIAL_SIZE, 1);
    queue->member_count = 4;
    memcpy(queue->data, str, 4);

    error = 10;

    queue_pop(queue, (uint8*) &c);
    error -= c == 'D';
    error -= queue->member_count == 3;
    c = 0x00;

    queue_peek(queue, (uint8*) &c);
    error -= c == 'C';
    error -= queue->member_count == 3;
    c = 0x00;

    queue_pop(queue, (uint8*) &c);
    error -= c == 'C';
    error -= queue->member_count == 2;
    c = 0x00;

    queue_pop(queue, (uint8*) &c);
    error -= c == 'B';
    error -= queue->member_count == 1;
    c = 0x00;

    queue_peek(queue, (uint8*) &c);
    error -= c == 'A';
    error -= queue->member_count == 1;
    c = 0x00;

    queue_pop(queue, (uint8*) &c);
    error -= c == 'A';
    error -= queue->member_count == 0;
    c = 0x00;

    queue_pop(queue, (uint8*) &c);
    error -= c == 0x00;
    error -= queue->member_count == 0;
    c = 0x00;
    
    queue_free(queue);
    return error;
}