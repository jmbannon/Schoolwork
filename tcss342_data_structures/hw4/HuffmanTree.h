#ifndef __HUFFMANTREE_H_
#define __HUFFMANTREE_H_
#define CHAR_RANGE 256
#define END_OF_TEXT 3

typedef struct _statistics_t {
    const char * inputname, * outputname;
    int input_size, output_size, runtime;
    unsigned char valid;
} statistics;

statistics Compress(const char * filename,
                    const char * outputname);

statistics Decompress(const char * filename,
                      const char * outputname);

#endif
