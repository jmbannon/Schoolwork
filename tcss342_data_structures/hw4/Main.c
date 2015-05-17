#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

double getRatio(int x, int y) {
    return (double)x/(double)y;
}

void printStatistics(statistics stats,
                     BOOL compress) 
{
    printf("*\n");
    if (compress) printf("* COMPRESSION METRICS\n");
    else          printf("* DECOMPRESSION METRICS\n");
    printf("*\n");
    printf("* %-15s size: %d bits\n", stats.inputname, stats.input_size);
    printf("* %-15s size: %d bits\n", stats.outputname, stats.output_size);
    printf("*\n");
    if (compress) printf("* Compression ratio: %.4f\n", 
                      getRatio(stats.output_size, stats.input_size));
    else          printf("* Decompression ratio: %.4f\n",
                      getRatio(stats.output_size, stats.input_size));
    printf("* Runtime: %d ms\n", stats.runtime);
    printf("*\n"); 
}

int main(int argc, char *argv[])
{
    BOOL compress;
    statistics stats;

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
        stats = Compress(argv[2], argv[3]);
    else
        stats = Decompress(argv[2], argv[3]);

    if (stats.valid == 0) {
        printf("*\n* %s does not exist.\n*\n", argv[2]);
        return EXIT_SUCCESS;
    }

    printStatistics(stats, compress);

    return EXIT_SUCCESS;
}

