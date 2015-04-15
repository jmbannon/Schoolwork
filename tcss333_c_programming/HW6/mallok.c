/*
==================================================================
File:   mallok.c

Author: Jesse Bannon
Class:  TCSS 333 A
HW:     HW 6
Desc:   Creates private memory allocation and allows client to
        free or assign memory from it.
==================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct block_node {
    void* start;
    int size;
    int available;
    struct block_node *next;
} Block;

/* Static variables used. */
static void* START;
static Block* HEAD;

/* Function prototype not included in header. */
static void combine_blocks(Block* freedBlock, Block* nextBlock);

/**
 * Creates private memory allocation of int size (in bytes).
 */
void create_pool(int size) {
    HEAD = NULL;
    START = malloc(size);
    Block* first = malloc(sizeof(Block));

    printf("* Created private heap of size %d\n", size);

    first->start = START;
    first->size = size;
    first->available = 1;
    first->next = HEAD;
    HEAD = first;
}

/*
 * Attempts to allocate memory within private allocation. Returns pointer to
 * free memory if there is enough memory available. Returns null if there is
 * no room for allocation.
 */
void *my_malloc(int size) {
    Block* block = HEAD;
    while (block != NULL) {
        //printf("%d > %d && %d\n", block->size, size, block->available);
        if (block->size >= size && block->available == 1) {
            Block* newBlock = malloc(sizeof(Block));
            newBlock->size = size;
            newBlock->available = 0;
            newBlock->start = block->start;
            newBlock->next = block->next;

            block->size -= size;
            block->start += size;
            block->next = newBlock;

            printf("* Allocated block of size %d\n", size);
            return newBlock->start;
        } else
            block = block->next;
    }
    printf("* Failed to allocate block of size %d\n", size);
    return NULL;
}

/*
 * Frees memory at the specified pointer (that was returned by *my_malloc).
 * Also combines any free blocks that exist after the specified pointer
 * to create a larger block of free memory.
 */
void my_free(void *block) {
    Block* theBlock = HEAD;
    Block* tempBlock;
    while (theBlock != NULL) {
        if (block == theBlock->start) {
            printf("* Freed block of size %d\n", theBlock->size);
            theBlock->available = 1;
            tempBlock = theBlock->next;

            while (tempBlock != NULL) {
                if (tempBlock->available == 1) {
                    combine_blocks(theBlock, tempBlock);
                    tempBlock = tempBlock->next;
                } else
                    return;
            }

            return;
        } else
            theBlock = theBlock->next;
    }
    printf("* Could not find pointer to free\n");
}

/**
 * Combines nextBlock to freedBlock and removes nextBlock.
 */
static void combine_blocks(Block* freedBlock, Block* nextBlock) {
    freedBlock->size += nextBlock->size;
    freedBlock->next = nextBlock->next;
    free(nextBlock);
}
