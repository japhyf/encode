# define BITNUM 8

# include <stdint.h>
# include "bv.h"
# include <stdlib.h>
# include <string.h>

bitV *newVec(uint32_t length)
{
    bitV *vector = (bitV*)malloc(sizeof(bitV));
    vector->v = (uint8_t*)malloc(sizeof(uint8_t)*(length/8));
    vector->l = length;
    vector->pos = 0;
    return vector;
}

void delVec(bitV *vector)
{
    free(vector->v);
    free(vector);
    return;
}

void one(bitV *vector)
{
    for( uint32_t i=0; i < vector->l ; i++ )
    {
        setBit(vector, i);
    }
    return;
}

void setBit(bitV *vector, uint32_t n)
{
    vector->v[n/BITNUM] |= 1<<(n%BITNUM);
    return;
}

void clrBit(bitV *vector, uint32_t n)
{
    vector->v[n/BITNUM] &= ~(1<<(n%BITNUM));
    return;
}

uint8_t valBit(bitV *vector, uint32_t n)
{
    return (vector->v[n/BITNUM] & (1 << (n%BITNUM))) >>(n%BITNUM);
}

uint32_t lenVec(bitV *vector)
{
    return vector->l;
}

void append(bitV *bitVec, code c, uint32_t *array)
{
    uint32_t temp = c.l;
    for(uint32_t i = temp; i!=0; i--)
    {
        popCode(&c, &array[i-1]);
    }
    for(uint32_t i = temp; i!= 0; i--)
    {
        pushCode(&c, array[i-1]);
    }
    for(uint32_t i = 0; i<temp; i++)
    {
        if (array[i])
        {
            setBit(bitVec, bitVec->pos);
        }
        else
        {
            clrBit(bitVec, bitVec->pos);
        }
        bitVec->pos++;
    }
}
