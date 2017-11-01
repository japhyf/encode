# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include "huffman.h"
# include "stack.h"

treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
    treeNode *node = malloc(sizeof(treeNode));
    node->symbol = s;
    node->leaf = l;
    node->count = c;
    node->left = NULL;//malloc(sizeof(item));
    node->right = NULL; //malloc(sizeof(item));
    return node;
}

treeNode *join(treeNode *l, treeNode *r)
{
    treeNode *internal = newNode('$', false, l->count+r->count);
    internal->left = l;
    internal->right = r;
    return internal;
}
//Author: Oskar
treeNode *loadTree (uint8_t savedTree [], uint16_t treeBytes )
{
    stack *nodeStack = newStack();
    treeNode *right = NULL; //malloc(sizeof(item));
    treeNode *left = NULL; //malloc(sizeof(item));
    treeNode *joined = NULL;
    treeNode *root = NULL;//malloc(sizeof(treeNode));
    for(uint16_t i = 0; i < treeBytes; i++)
    {
        if(savedTree[i] == 'L')
        {
            item *hoja = newNode(savedTree[++i], true, 0);
            push(nodeStack, hoja);
            //free(hoja);
        }
        else
        {
            right = pop(nodeStack);
            left = pop(nodeStack);
            joined = join(left, right);
            push(nodeStack, joined);
            //free(joined);
        }
    }
    root = pop(nodeStack);
    //free(left);
    //free(right);
    delStack(nodeStack);
    return root;
}
//Author: Oskar
int32_t stepTree (treeNode *root , bool direction)
{
    static treeNode *pos = NULL;
    if(pos == NULL)
    {
        pos = root;
    }
    if(direction)
    {
        pos = pos->right;
    }
    else
    {
        pos = pos->left;
    }
    if(pos->leaf)
    {
        int32_t s = pos->symbol;
        pos = root;
        return s;
    }
    else
    {
        return -1;
    }
}
void printTree(treeNode *i)
{
    if(i && i->leaf)
    {
        printf("L%c", i->symbol);
    }
    else
    {
        printTree(i->left);
        printTree(i->right);
        printf("I");
    }
}

void dumpTree(treeNode *i, int fd)
{
    if(i && i->leaf)
    {
        char l = 'L';
        write(fd, &l, 1);
        write(fd, &(i->symbol), 1);
    }
    else
    {
        dumpTree(i->left, fd);
        dumpTree(i->right, fd);
        char i = 'I';
        write(fd, &i, 1);
    }
}

void buildCode(treeNode *i, code *s, code table[256])
{
    if(i && i->leaf)
    {
        table[i->symbol] = *s;
        if (!popCodeDel(s))
        {
            printf("empty code!\n");
        }
    }
    else
    {
        pushCode(s, 0);
        buildCode(i->left, s, table);
        pushCode(s, 1);
        buildCode(i->right, s, table);
        popCodeDel(s);
    }
}

void delTree(treeNode *i)
{
    if(i && i->leaf)
    {
        free(i->right);
        free(i->left);
        free(i);
    }
    else
    {
        delTree(i->left);
        delTree(i->right);
        free(i);
    }
}
