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
    printf("* [-c]  [txt_name] [bin_output]");
    printf(" Compresses text file to binary file\n");
    printf("* [-cs] [txt_name] [bin_output]");
    printf(" Compresses and shows hash statistics\n");
    printf("* [-d]  [bin_name] [txt_output]");
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

double * getMaxProbe(statistics stats)
{
    unsigned int i, den = 0;
    double * metrics = calloc(2, sizeof(double));
    for (i = 0; i < 1024; i++) {
        metrics[1] += (i * stats.probes[i]);
        den += i;
        if (stats.probes[i] > 0)
            metrics[0] = i;
    }
    metrics[1] /= den;
    return metrics;
}

void printHashStatistics(statistics stats)
{
    unsigned int i;
    double * metrics = getMaxProbe(stats);
    printf("*\n");
    printf("* HASH TABLE METRICS\n");
    printf("*\n");
    printf("* Number of Entries: %ld\n", stats.entries);
    printf("* Number of Buckets: %ld\n", HASH_TABLE_SIZE);
    printf("* Fill Percentage:   %.2f%\n",
            getRatio(stats.entries, HASH_TABLE_SIZE)*100);
    printf("* Max Probe:         %.0f\n", metrics[0]);
    printf("* Average Probe:     %.2f\n", metrics[1]);  // FILL IN
    printf("* Histogram of Probes:\n*");
    for (i = 0; i <= (int)metrics[0]; i++) {
        if (i % 4 == 0) printf("\n*");

        if (i != 0) printf("%9ld", stats.probes[i]);
        else printf("[%8ld", stats.probes[i]);

        if (i != (int)metrics[0]) printf(",");
        else printf(" ]");
    }
    printf("\n*\n");
}

int main(int argc, char *argv[])
{
    BOOL compress, showStats = 0;
    statistics stats;

    if (argc < 4) {
        printUsage();
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-c") == 0)
        compress = 1;
    else if (strcmp(argv[1], "-d") == 0)
        compress = 0;
    else if (strcmp(argv[1], "-cs") == 0) {
        compress = 1;
        showStats = 1;
    } else {
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
    if (compress && showStats) printHashStatistics(stats);
    return EXIT_SUCCESS;
}

