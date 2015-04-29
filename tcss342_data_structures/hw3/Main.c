#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HuffmanTree.h"
#define BOOL unsigned char

void printUsage() {
    printf("*\n*\n");
    printf("* COMPRESSOR USAGE\n");
    printf("*\n");
    printf("* [-c] [txt_name] [bin_output]");
    printf(" Compresses text file to binary file\n");
    printf("* [-d] [bin_name] [txt_output]");
    printf(" Decompresses binary file to text file\n");
    printf("*\n*\n");
}

int main(int argc, char *argv[])
{
    BOOL compress;

    if (argc < 4) {
        printUsage();
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-c") == 0)
        compress = 1;
    else if (strcmp(argv[1], "-d") == 0)
        compress = 0;
    else {
        printUsage();
        return EXIT_SUCCESS;
    }
      
    if (compress)
        Compress(argv[2], argv[3]);
    else
        Decompress(argv[2], argv[3]);

    return EXIT_SUCCESS;
}
