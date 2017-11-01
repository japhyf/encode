# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include "queue.h"
# include <stdint.h>

queue *newQueue(uint32_t size)
{
    queue *q = malloc(sizeof(queue));
    q->size = size;
    q->head = 0;
    q->tail = 0;
    q->Q = malloc(size*sizeof(item*));
    return q;
}

void delQueue(queue *q)
{
    free(q->Q);
    free(q);
}

bool qEmpty(queue *q)
{
    if (q->head == q->tail)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool qfull(queue *q)
{
    if(q->tail == q->size)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool enqueue(queue *q, item *i)
{
    if(qfull(q))
    {
        return false;
    }
    else if(qEmpty(q))
    {
        q->Q[q->tail] = i;
        q->tail = q->tail + 1;
        return true;
    }
    for(uint32_t j = 1; j <= q->tail; j++)
    {
        if(i->count <= (q->Q[q->tail-j])->count)
        {
            q->Q[(q->tail-j)+1] = i;
            q->tail = q->tail + 1;
            return true;
        }
        else
        {
            q->Q[(q->tail-j)+1] = q->Q[(q->tail-j)];

        }
    }
    q->Q[q->head] = i;
    q->tail = q->tail + 1;
    return true;
}

item * dequeue(queue *q)
{
    item *i = q->Q[q->tail-1];
    q -> tail = q -> tail - 1;
    return i;
}

void printQ(queue *q)
{
    for(uint32_t i = q->head; i < q->tail; i++)
    {
        printf("%c count is %ld\n", (q->Q[i])->symbol, (q->Q[i])->count);
    }
}
