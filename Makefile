CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c99 -g
CC = gcc
.PHONY: all
all: encode decode

encode : encode.o queue.o stack.o huffman.o bv.o
	$(CC) $(CFLAGS) encode.o queue.o stack.o bv.o huffman.o -o encode
decode : decode.o queue.o stack.o huffman.o
	$(CC) $(CFLAGS) decode.o queue.o stack.o huffman.o -o decode
decode.o : decode.c
		$(CC) $(CFLAGS) -c decode.c
encode.o : encode.c
	$(CC) $(CFLAGS) -c encode.c
queue.o : queue.c
	$(CC) $(CFLAGS) -c queue.c
stack.o : stack.c
	$(CC) $(CFLAGS) -c stack.c
bv.o : bv.c
	$(CC) $(CFLAGS) -c bv.c
huffman.o : huffman.c
	$(CC) $(CFLAGS) -c huffman.c

clean:
	rm -f encode encode.o queue.o stack.o huffman.o decode decode.o bv.o
