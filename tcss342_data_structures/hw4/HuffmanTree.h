#ifndef __HUFFMANTREE_H_
#define __HUFFMANTREE_H_
#define HASH_TABLE_SIZE 32768
#define END_OF_TEXT 3

typedef struct _statistics_t {
    const char * inputname, * outputname;
    int input_size, output_size, runtime;
    unsigned char valid;

    unsigned int probes[1024], entries;
} statistics;

statistics Compress(const char * filename,
                    const char * outputname);

statistics Decompress(const char * filename,
                      const char * outputname);

#endif
