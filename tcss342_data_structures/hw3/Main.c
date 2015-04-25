#include <stdio.h>
#include <stdlib.h>
#include "HuffmanTree.h"

int main(void)
{

    Compress("WarAndPeace.txt", "nope");
    printf("Compressed\n");
    Decompress("nope", "test.txt");
    return 0;
}
