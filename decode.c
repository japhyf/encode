# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdbool.h>
# include <getopt.h>
# include <errno.h>
# include "queue.h"
# include "stack.h"
# include "huffman.h"
# define MAGICNUM 0xdeadd00d
# define BUFFSZ 1000
# define NUMBITS 8

bool nextBit(int);

int main(int argc, char *argv[])
{
    int closed;
    bool verbose = false;
    int input; //fd for input file
    int output = 1; //fd for output (assigned to 1 for STDOUT_FILENO)
    int c;
    char *file;
    while ((c = getopt(argc, argv, "i:o:v")) != -1) //reads in flags from command line input
    {
        switch (c)
        {
            case 'i' : //handles input file
                input = open(optarg, O_RDONLY); //opens input file to read from
                if(input == -1)
                {
                    perror("Error opening input file\n");
                    exit(errno);
                }
                break;
            case 'o' : //handles output file
                file = optarg;
                if(access(file, W_OK) == 0) //checks whether file exists in directory
                {
                    printf("Cannot overwrite existing file\n");
                    exit(0);
                }
                output = open(file, O_WRONLY | O_CREAT, 0644);//creates and opens new file to write to
                if(output == -1)
                {
                    perror("Error opening output file\n");
                    exit(errno);
                }
                break;
            case 'v' : //handles verbose
                verbose = true;
                break;
        }
    }
    uint32_t *magicN = malloc(sizeof(uint32_t));
    uint64_t *fileSz = malloc(sizeof(uint64_t));
    uint16_t *treeSz = malloc(sizeof(uint16_t));
    ssize_t bytesRead = read(input, magicN, 4); //reads in magicNumber
    if(bytesRead == -1)//error checking
    {
        perror("error reading file\n");
        free(magicN);
        free(fileSz);
        free(treeSz);
        exit(errno);
    }
    else if(*magicN != MAGICNUM) //checks whether input file is compressed
    {
        printf("not a compressed file\n");
        free(magicN);
        free(fileSz);
        free(treeSz);
        exit(0);
    }
    read(input, fileSz, 8);
    read(input, treeSz, 2);
    uint8_t *tree = malloc(sizeof(uint8_t) * (size_t)treeSz);
    read(input, tree, (size_t)*treeSz);
    treeNode *root = loadTree(tree, *treeSz); //rebuilds huffman tree
    uint8_t *symbols = malloc(sizeof(uint8_t)* *fileSz); //buffer for writing to output file
    uint64_t j = 0;
    while(j < *fileSz) //steps through tree filling buffer with correct characters
    {
        int32_t step = stepTree(root, nextBit(input));
        if(step != -1)
        {
            symbols[j] = step;
            j++;
        }
    }
    if(verbose)
    {
        printf("Original bits: %ld ", (*fileSz)*NUMBITS);
        printf("Tree: %d\n", *treeSz);
    }
    write(output, symbols, j);
    free(magicN);
    free(fileSz);
    free(treeSz);
    delTree(root);
    free(tree);
    free(symbols);
    closed = close(input);
    if(closed == -1)//error checks
    {
        perror("error closing input file\n");
        exit(errno);
    }
    closed = close(output);
    if(closed == -1)//error checks
    {
        perror("error closing output file");
        exit(errno);
    }
}
bool nextBit(int input)
{
    static int bitP = 0; //tracks bit position
    static uint8_t buf[BUFFSZ]; //kilobyte long buff
    if(bitP == 0 || (bitP / NUMBITS) == BUFFSZ) //reads in kilobyte if needed
    {
        ssize_t bytesRead = read(input, buf, BUFFSZ);
        if(bytesRead == -1 || bytesRead == 0)
        {
            perror("error reading bits\n");
            exit(0);
        }
        bitP = 0;
    }
    bool bit = (buf[bitP/NUMBITS] & (1 << (bitP % NUMBITS))) >> (bitP % NUMBITS); //bit twiddling
    bitP ++;
    return bit;
}
