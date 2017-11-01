# ifndef HUFFMAN_H
# define HUFFMAN_H
# include <stdint.h>
# include <stdbool.h>
# include "code.h"
//# include "stack.h"

typedef struct DAH treeNode;

typedef treeNode item;

struct DAH
{
    uint8_t symbol;
    uint64_t count;
    bool leaf;
    treeNode *left, *right;
};
// new node with symbol, leaf, and count
treeNode *newNode(uint8_t s, bool l, uint64_t c);

void delTree(treeNode *i);

void dumpTree(treeNode *t, int);

treeNode *join(treeNode *l, treeNode *r);

void printTree(treeNode *i);

treeNode *loadTree (uint8_t savedTree [], uint16_t treeBytes );

int32_t stepTree (treeNode *root, bool direction );

void buildCode(treeNode *t, code *s, code table[256]);

#endif
