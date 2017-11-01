# ifndef CODE_H
# define CODE_H

# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>

typedef struct code
{
    uint8_t bits[32];
    uint32_t l;
} code;

static inline code newCode()
{
    code t;
    for (int i = 0; i < 32; i++)
    {
        t.bits[i] = 0;
    }
    t.l = 0;
    return t;
}

static inline bool pushCode(code *c, uint32_t k)
{
    if(c->l > 256)
    {
        return false;
    }
    else if(k==0)
    {
        c->bits[c->l / 8] &= ~(0x1 << (c->l % 8));
        c->l++;
    }
    else
    {
        c->bits[c->l / 8] |= (0x1 << (c->l % 8));
        c->l++;
    }
    return true;
}

static inline bool popCodeDel(code *c) //, uint32_t *k
{
    if (c->l == 0)
    {
        return false;
    }
    else
    {
        //printf("before pop c->l is %d\n", c->l);
        c->l -= 1;
        //printf("after pop c->l is %d\n", c->l);
        //*k = ((0x1 << (c->l % 8)) & c->bits[c->l / 8]) >> (c->l % 8);
        return true;
    }
}

static inline bool popCode(code *c, uint32_t *k)
{
    if (c->l == 0)
    {
        return false;
    }
    else
    {
        c->l -= 1;
        *k = ((0x1 << (c->l % 8)) & c->bits[c->l / 8]) >> (c->l % 8);
        return true;
    }
}

static inline void printCode(code *c)
{
    for(uint32_t i = 0; i < c->l; i++)
    {
        printf("%d", (c->bits[i/8] & (1 << (i % 8))) >> (i % 8));
    }
}


static inline bool emptyCode(code *c) { return c->l == 0;}

static inline bool fullCode(code *c) { return c->l == 256;}

# endif
