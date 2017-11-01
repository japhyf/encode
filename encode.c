# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <getopt.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include "queue.h"
# include "stack.h"
# include "bv.h"
# include "huffman.h"
# include "code.h"

//# pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
int main(int argc, char **argv)
{
    //int closed;
    bool verbose = false;
    int input; //fd for input file
    FILE *fp;
    int output = 1; //fd for output (assigned to 1 for STDOUT_FILENO)
    int c;
    char *file;
    uint16_t leafCount = 0;
    uint32_t hist[256];
    int count = 0;
    for(int i = 0; i < 256; i++)//set every value in hist to 0
    {
       hist[i] = 0;
    }
    hist[0]++;//increment so the tree always has at least 2 elements

    while ((c = getopt(argc, argv, "i:o:v")) != -1) //reads in flags from command line input
    {
        switch (c)
        {
            case 'i' : //handles input file
                file = optarg;
                fp = fopen(file,"r");
                input = open(file, O_RDONLY); //opens input file to read from
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
    fseek(fp, 0L, SEEK_END);
    int symbols[ftell(fp)];
    uint64_t fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    unsigned char d;
    do //fill the histogram
    {
        d = getc(fp);
        hist[d]++;
        symbols[count] = d;
        count++;
    } while(!feof(fp));
    fclose(fp);
    queue *q = newQueue(257);
    treeNode *node = NULL;
    for(int j = 0; j < 256; j++)//create a leaf node for every value in the histogram
    {
        if(hist[j])
        {
            node = newNode(j, true, hist[j]);
            enqueue(q, node);
            leafCount++;
            node = NULL;
        }
    }
    item *item1;// = malloc(sizeof(item));
    item *item2;//= malloc(sizeof(item));
    item *joined = NULL;
    while(!qEmpty(q))//dequeue and item
    {
        item1 = dequeue(q);
        if(!qEmpty(q))//dequeue another item
        {
            item2 = dequeue(q);
            joined = join(item2, item1);//join the two items
            enqueue(q, joined);//enqueue the joined item
        }
    }
    code apple = newCode();
    code table[256];
    buildCode(item1, &apple, table);//build the codes for every character
    uint32_t *array = malloc(sizeof(uint32_t)*256);
    bitV *vector = newVec(fileSize*8);
    for(uint64_t i = 0; i < fileSize; i++)//append all the codes of the characters in the file to the output
    {
        append(vector, table[symbols[i]], array);
    }
    if (verbose) {
        printf("Original bits: %ld\n", fileSize*8);
        printf("Leaves: %d (%d bytes)\n", leafCount, (leafCount*3)-1);
        printf("Encoding %d bits (%lf%%)\n", vector->pos, (float)(100)*(vector->pos)/(fileSize*8));
    }
    uint32_t magic = 0xdeadd00d;
    write(output, &magic, 4);
    write(output, &fileSize, 8);
    leafCount = (leafCount*3)-1;
    write(output, &leafCount, 2);
    dumpTree(item1, output);
    write(output, vector->v, (vector->pos+7)/8);

    delQueue(q);
    delTree(item1);
    free(array);
    //delTree(item1);
    delVec(vector);
    return 0;
}
