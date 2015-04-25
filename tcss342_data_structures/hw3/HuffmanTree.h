#ifndef __HUFFMANTREE_H_
#define __HUFFMANTREE_H_
#define CHAR_RANGE 256
#define END_OF_TEXT 3
#define THREADS 4

int Compress(const char * filename, const char * outputname);
int Decompress(const char * filename, const char * outputname);

#endif
