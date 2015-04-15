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

#define avalaible_size sizeof(unsigned char)
#define size_size sizeof(int)
#define ptr_size sizeof(void*)
#define INFO_BLOCK (avalaible_size+size_size+ptr_size)

/* Last storage unit must be free to store free memory.
 * This technically only supports 20 blocks to malloc. */
#define INFO_BLOCK_COUNT 21

struct block_node {
    void* dataPtr;
    void* dataInfoPtr;
    int dataSize, dataInfoSize;
} Block;

/* Function prototype not included in header. */
static void combine_blocks();

/**
 * Creates private memory allocation of size (in bytes).
 */
void create_pool(int size) {
    Block.dataPtr = malloc(size + (INFO_BLOCK*INFO_BLOCK_COUNT));
    if (Block.dataPtr == NULL)
        printf("*** Could not initialize private heap of size %d. Returning...", size);

    Block.dataSize = size;
    Block.dataInfoPtr = Block.dataPtr + size;
    Block.dataInfoSize = INFO_BLOCK*INFO_BLOCK_COUNT;

    *(unsigned char*)Block.dataInfoPtr = 1;
    *(int*)(Block.dataInfoPtr + avalaible_size) = size;

    void** temp = (Block.dataInfoPtr + avalaible_size + size_size);
    *temp = Block.dataPtr;

    printf("* Created private heap of dataSize %d\n", size);
}

/*
 * Attempts to allocate memory within private allocation. Returns pointer to
 * free memory if there is enough memory available. Returns null if there is
 * no room for allocation.
 */
void *my_malloc(int size) {
    int i;
    for (i = 0; i < Block.dataInfoSize; i+=INFO_BLOCK) {

        unsigned char* currentAvailablity = (Block.dataInfoPtr + i);
        int* currentSize = (Block.dataInfoPtr + i + avalaible_size);
        void** currentPtr = (Block.dataInfoPtr + i + avalaible_size + size_size);

        if (*(currentAvailablity) == 1 && *(currentSize) >= size) {

            if (i+INFO_BLOCK >= Block.dataInfoSize) {
                printf("* Not enough Malloc Information memory.\n"
                        "* Current malloc counts = %d. Returning null...\n", INFO_BLOCK_COUNT-1);
                return NULL;
            }

            // Sets next info block with available storage information
            *(unsigned char*)(Block.dataInfoPtr + i + INFO_BLOCK) = *(currentAvailablity);
            *(int*)(Block.dataInfoPtr + i + INFO_BLOCK + avalaible_size)
                    = *(currentSize) - (int)size;

            void** temp = (Block.dataInfoPtr + i + INFO_BLOCK + avalaible_size + size_size);
            *temp = (*currentPtr + size);

            // Sets existing block with new storage information
            *(currentAvailablity) = 0;
            *(currentSize) = (int)size;

            printf("* Created block of dataSize %d\n", size);

            return *currentPtr;
        }
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
    int i;
    for (i = 0; i < Block.dataInfoSize; i+=INFO_BLOCK) {

        unsigned char* currentAvailablity = (Block.dataInfoPtr + i);
        int* currentSize = (Block.dataInfoPtr + i + avalaible_size);
        void** currentPtr = (Block.dataInfoPtr + i + avalaible_size + size_size);

        if (*currentPtr == block) {
            memset(block, 0, *(currentSize));
            printf("* Freed block of size %d\n", *(currentSize));
            *(currentAvailablity) = 1;
            combine_blocks();
            return;
        }
    }
    printf("* Could not find block. Returning...\n");
}

/**
 * Loops through all blocks and their proceeding neighbor seeing if they
 * are both available to combine free space for future my_mallocs.
 */
static void combine_blocks() {
    int i = 0, j = INFO_BLOCK;
    unsigned char* currentAvailablity;
    unsigned char* nextAvailability;
    int* currentSize;
    int* nextSize;

    while (i < Block.dataInfoSize - INFO_BLOCK) {
        while (j < Block.dataInfoSize) {

            currentAvailablity = (Block.dataInfoPtr + i);
            currentSize = (Block.dataInfoPtr + i + avalaible_size);

            nextAvailability = (Block.dataInfoPtr + j);
            nextSize = (Block.dataInfoPtr + j + avalaible_size);

            if (*(currentAvailablity) == 1 && *(nextAvailability) == 1 && *(nextSize) > 0) {
                *(currentSize) += *(nextSize);
                *(nextSize) = 0;
                j+=INFO_BLOCK;
            }
            else
                j+=INFO_BLOCK;
        }

        i += INFO_BLOCK;
        j = i+INFO_BLOCK;
    }
}
