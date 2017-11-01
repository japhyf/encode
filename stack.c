# include <stdio.h>
# include <stdlib.h>
# include "stack.h"
# include "huffman.h"

stack *newStack()
{
    stack *s = malloc(sizeof(stack));
    s->size = 256;
    s->top = 0;
    s->entries = malloc(sizeof(item*)*256);
    return s;
}

void push(stack *s, item *i)
{
    if(!sfull(s))
    {
        s->entries[s->top] = i;
        s->top++;
    }
}

item *pop (stack *s)
{
    s->top--;
    return s->entries[s->top];
}

void delStack(stack *s)
{
    free(s->entries);
    free(s);
}

bool sempty(stack *s)
{
    if (s->top == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool sfull(stack *s)
{
    if (s->top > s->size)
    {
        return true;
    }
    else
    {
        return false;
    }
}
