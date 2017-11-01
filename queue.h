# ifndef QUEUE_H
# define QUEUE_H
# include <stdbool.h>
# include <stdint.h>
# include "stack.h"
# include "huffman.h"

typedef struct queue
{
    uint32_t size;
    uint32_t head, tail;
    item **Q; //change this to item
} queue;

queue *newQueue(uint32_t size);

void delQueue(queue *q);

bool qEmpty(queue *q);

bool qfull(queue *q);

bool enqueue(queue *q, item *i);

item* dequeue(queue *q);

void printQ(queue *q);

#endif
