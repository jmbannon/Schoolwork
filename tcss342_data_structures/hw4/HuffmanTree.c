/*
 ====================================================================

 Author:    Jesse Bannon
 Date:      04/25/15
 Class:     TCSS 342: Data Structures
 School:    University of Washington Tacoma
 Desc:      Uses Huffman Trees to compress and decompress text files.
 Copyright: Use for educational purposes only.

 ====================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "HuffmanTree.h"

/*
 ====================================================================
 =  Bit Operators  ==================================================
 ====================================================================
*/

#define SHIFT_CODE(c) (c<<=1)
#define WRITE_BIT(c) (c |= 0x1)
#define ENCODE_BIT(i,c) (c |= (0x1 << 31-i))
#define CHECK_BIT(i,c) (c & (0x1 << i))

/*
 ====================================================================
 =  Structures  ====================================================
 ====================================================================
*/

/* Char encoding */
typedef struct __tree_t_ {
    struct __tree_t_ *left;
    struct __tree_t_ *right;
    unsigned int weight;
    char leaf;    
} tree_t;

typedef struct __table_n_ {
    unsigned char character;
    unsigned int codeword;
} t_node;

/* String encoding */
typedef struct __tree_w_ {
    struct __tree_w_ *left;
    struct __tree_w_ *right;
    unsigned int weight;
    char * leaf;
} tree_w;

typedef struct __table_w_ {
    unsigned char * word;
    unsigned int codeword;
} w_node;

static unsigned int HASH_TABLE_SIZE = 32768;

/*
 ====================================================================
 =  Function Prototypes  ============================================
 ====================================================================
*/

/* Hash Table Functions */
w_node* buildHashTable();

unsigned int getHashCode(char * word);

int put(w_node * table,
         unsigned int key,
         char * word);

/* Encode (Compress) Functions */
int Encode(FILE *in, 
           FILE *out);

tree_t* buildTree(unsigned int freq[], 
                  unsigned int * charCount);

unsigned int getCodeWord(t_node** table, 
                         unsigned int charCount,
                         unsigned char c);

void printCodewords(t_node** table, 
                    unsigned int charCount);

void writeHeader(FILE *out, 
                 t_node** table,
                 unsigned int charCount);

tree_t** merge(tree_t** tree_1, 
               tree_t** tree_2, 
               size_t size1, 
               size_t size2);

tree_t** mergeSort(tree_t** tree_a, 
                   size_t size);

/* Decode (Decompress) Functions */
int Decode(FILE * in,
           FILE * out);

tree_t* rebuildTree(FILE * in,
                    unsigned int charCount);

t_node** buildTable(tree_t* head,
                  unsigned int charCount);

void traverseTree(t_node** table,
                  tree_t* node,
                  unsigned int codeword,
                  unsigned char binIndex,
                  unsigned int * index);

/* Statistic Functions */
int file_length(FILE *f);

long timediff(clock_t t1, 
              clock_t t2);


/*
 ====================================================================
 =  Functions  ======================================================
 ====================================================================
*/

w_node * buildHashTable()
{
    w_node * hash_table = calloc(HASH_TABLE_SIZE, sizeof(w_node));
    if (hash_table == NULL)
        return NULL; // ERROR
    return hash_table;
}

unsigned int getHashCode(char * key)
{
    size_t len = strlen(key);
    unsigned int i, hash = 0, seed = 37;
    for (i = 0; i < len; i++) {
        hash = (hash * seed) + key[i];
    }

    return hash % HASH_TABLE_SIZE;
}

int inputWord(w_node * table,
              unsigned int index,
              char * word)
{
    size_t len = strlen(word);
    table[index].word = malloc((len+1)*sizeof(char));
    strcpy(table[index].word, word);
    table[index].word[len] = '\0';
    printf("new word: %s\n", table[index].word);
}

int put(w_node * table,
         unsigned int key,
         char * word)
{
    unsigned int idx, i;

    if (table[key].word == NULL) {
        inputWord(table, key, word);
    } else if (strcmp(table[key].word, word) == 0)
        printf("");
    else {
        i = 1;
        idx = (i*i++) + key % HASH_TABLE_SIZE;
        while (1) {
            if (table[idx].word == NULL) {
                inputWord(table, idx, word);
                printf("jumps: %ld\n", i-1);
                return 1;
            } else if (strcmp(table[idx].word, word) == 0) {
                printf("jumps: %ld\n", i-1);
                return 0;
            } else
                idx = (i*i++) + key % HASH_TABLE_SIZE;
        }
    }
    return 0;
}

int Encode_W(FILE * in,
             FILE * out)
{
    unsigned int idx = 0, total = 0;
    char c, wordBuffer[64];
    w_node * table = buildHashTable();
    
    while ((c = fgetc(in)) != EOF) {
        if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
            wordBuffer[idx++] = c;
        else {
            wordBuffer[idx] = '\0';
            if (idx != 0)
                total += put(table, getHashCode(wordBuffer), wordBuffer);

            wordBuffer[0] = c;
            wordBuffer[1] = '\0';
            total += put(table, getHashCode(wordBuffer), wordBuffer);
            idx = 0;
        }
    }
    rewind(in);
    printf("total misshashes: %ld\n", total);
}

/* ==================================================================== *
 *  Compress:                                                           *
 *  Compresses the specified text file and writes it to the             *
 *  specified output name.                                              *
 * ==================================================================== */
statistics Compress(const char * filename, 
                    const char * outputname) 
{
    FILE * in;
    FILE * out;
    clock_t t1, t2;
    statistics stats;

    in = fopen(filename, "r");
    out = fopen(outputname, "w");
    if (!in || !out) {
        stats.valid = 0;
        return stats;
    }

    t1 = clock();
    Encode_W(in, out);
    t2 = clock();
   
    stats.inputname = filename;
    stats.outputname = outputname;
    stats.input_size = file_length(in);
    stats.output_size = file_length(out);
    stats.runtime = timediff(t1, t2);

    fclose(in);
    fclose(out);
    stats.valid = 1;
    return stats;
}

/* ==================================================================== *
 *  Decompress:                                                         *
 *  Decompresses the specified binary file and writes it to the         *
 *  specified output name.                                              *
 * ==================================================================== */
statistics Decompress(const char * filename,
                      const char * outputname) 
{
    FILE * in;
    FILE * out;
    time_t t1, t2;
    statistics stats;

    in = fopen(filename, "rb");
    out = fopen(outputname, "w");
    if (!in || !out) {
        stats.valid = 0;
        return stats;
    }
    t1 = clock();
    Decode(in, out);
    t2 = clock();

    stats.inputname = filename;
    stats.outputname = outputname;
    stats.input_size = file_length(in);
    stats.output_size = file_length(out);
    stats.runtime = timediff(t1, t2);
    fclose(in);
    fclose(out);

    stats.valid = 1;
    return stats;
}

/* ==================================================================== *
 *  Encode:                                                             *
 *  Encodes an opened text file and writes to an opened binary file.    *
 * ==================================================================== */
int Encode(FILE *in, 
           FILE *out) 
{
    unsigned int c, bufferSize, bitBuffer, codeword, charCount, end;
    unsigned int frequency[CHAR_RANGE] = { 0 };
    int bindex;
    t_node** table;
    
    while ((c = fgetc(in)) != EOF) frequency[c]++;
    frequency[END_OF_TEXT]++;
    rewind(in);

    tree_t* head = buildTree(frequency, &charCount);
    table = buildTable(head, charCount);
 
    writeHeader(out, table, charCount);
    printCodewords(table, charCount);

    bitBuffer = 0;
    bufferSize = 0;
    end = 0;
    while ((c = fgetc(in)) != EOF) {
        codeword = getCodeWord(table, charCount, c);
end_of_file:
        bindex = 31;
        while (!CHECK_BIT(bindex--, codeword));
        while (bindex >= 0) {
            if (CHECK_BIT(bindex--, codeword))
                ENCODE_BIT(bufferSize++, bitBuffer);
            else
                bufferSize++;

            if (bufferSize == 32) {
                fwrite(&bitBuffer, sizeof(unsigned int), 1, out);
                bufferSize = 0;
                bitBuffer = 0;
            }
        }
    }
    if (!end) {
        end = 1;
        codeword = getCodeWord(table, charCount, END_OF_TEXT);
        goto end_of_file;
    }
    if (bufferSize)
        fwrite(&bitBuffer, sizeof(unsigned int), 1, out);
}

/* ==================================================================== *
 *  buildTree:                                                          *
 *  Builds a Huffman Tree using array of character frequencies.         *
 * ==================================================================== */
tree_t* buildTree(unsigned int freq[], 
                  unsigned int * charCount) 
{
    int i, len = 0;
    tree_t** tqueue = malloc(CHAR_RANGE*sizeof(tree_t*));

    for (i = 0; i < CHAR_RANGE; i++) {
        if (freq[i] > 0) {
            tree_t* temp = malloc(sizeof(tree_t));
            temp->leaf = (char)i;
            temp->weight = freq[i];
            tqueue[len++] = temp;
        }
    }
    *charCount = len;

    while (len > 1) {
        tree_t* toAdd = malloc(sizeof(tree_t));
        toAdd->weight = 0;

        tqueue = mergeSort(tqueue, len);
        
        toAdd->left = tqueue[--len];
        toAdd->weight += tqueue[len]->weight;

        toAdd->right = tqueue[--len];
        toAdd->weight += tqueue[len]->weight;
        tqueue[len++] = toAdd;
    }
    tree_t* head = malloc(sizeof(tree_t*));
    head = tqueue[0];
    free(tqueue);
    return head;
}

/* ==================================================================== *
 *  getCodeWord:                                                        *
 *  Returns the codeword from the table for the character given.        *
 * ==================================================================== */
unsigned int getCodeWord(t_node** table, 
                         unsigned int charCount,
                         unsigned char c) 
{
    int i;
    for (i = 0; i < charCount; i++) {
        if (c != table[i]->character) continue;
        else return table[i]->codeword;
    }
    printf("unreachable\n");
    return 0;
}

/* ==================================================================== *
 *  printCodewords:                                                     *
 *  Writes all codeword's binary equivalence to a text file.            *
 * ==================================================================== */
void printCodewords(t_node** table, 
                    unsigned int charCount)
{
    FILE * out;
    int i, j;

    out = fopen("codewords.txt", "w");

    for (i = 0; i < charCount; i++) {
        fprintf(out, "%c : ", table[i]->character);
        j = 31;
        while(!CHECK_BIT(j--, table[i]->codeword));
        while(j >= 0) {
            if (CHECK_BIT(j--, table[i]->codeword))
                fprintf(out, "%d", (int)1);
            else
                fprintf(out, "%d", (int)0);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

/* ==================================================================== *
 *  writeHeader:                                                        *
 *  Writes the test number, amount of unique chars, and the chars       *
 *  corresponding to the table followed by their codeword.              *
 * ==================================================================== */
void writeHeader(FILE *out, 
                 t_node** table,
                 unsigned int charCount) 
{
    int i;
    const unsigned int verification = 13370666;

    fwrite(&verification, sizeof(unsigned int), 1, out);
    fwrite(&charCount, sizeof(unsigned int), 1, out);

    for (i = 0; i < charCount; i++) {
        fwrite(&table[i]->character, sizeof(unsigned char), 1, out);
        fwrite(&table[i]->codeword, sizeof(unsigned int), 1, out);
    }
}

/* ==================================================================== *
 *  merge:                                                              *
 *  Merges two huffman trees and returns the head.                      *
 * ==================================================================== */
tree_t** merge(tree_t** tree_1, 
               tree_t** tree_2, 
               size_t size1, 
               size_t size2) 
{
    tree_t** toReturn = malloc((size1+size2)*sizeof(tree_t*));
    int i, j, k;
    i = j = k = 0;

    while (i < size1 && j < size2) {
        if (tree_1[i]->weight > tree_2[j]->weight)
            toReturn[k++] = tree_1[i++];
        else
            toReturn[k++] = tree_2[j++];
    }
    while (i < size1)
        toReturn[k++] = tree_1[i++];
    while (j < size2)
        toReturn[k++] = tree_2[j++];

    free(tree_1);
    free(tree_2);
    return toReturn;
}

/* ==================================================================== *
 *  mergeSort:                                                          *
 *  Sorts Huffman Trees by largest size and returns array of pointers   *
 *  to the trees.                                                       *
 * ==================================================================== */
tree_t** mergeSort(tree_t** tree_a, 
                   size_t size) 
{
    if (size <= 1) return tree_a;

    int i, s1, s2;
    size_t size1 = (size_t)size/2;
    size_t size2 = size - size1;        
    
    tree_t** tree_1 = malloc(size1*sizeof(tree_1));
    tree_t** tree_2 = malloc(size2*sizeof(tree_2));
    
    s1 = s2 = 0;
    for (i = 0; i < size; i++) {

        if (i % 2 == 0) 
            tree_2[s2++] = tree_a[i];
        else 
            tree_1[s1++] = tree_a[i];
    }
    
    tree_1 = mergeSort(tree_1, s1);
    tree_2 = mergeSort(tree_2, s2);

    free(tree_a);
    return merge(tree_1, tree_2, size1, size2);
}

/* ==================================================================== *
 *  Decode:                                                             *
 *  Decodes an opened compressed binary file and writes it to an        *
 *  opened textfile.                                                    *
 * ==================================================================== */
int Decode(FILE * in,
           FILE * out)
{
    unsigned int j, charCount, buffer;
    char character;
    int i;

    fread(&j, sizeof(unsigned int), 1, in);
    if (j != 13370666) return -1;

    fread(&charCount, sizeof(unsigned int), 1, in); 
    
    tree_t* head = rebuildTree(in, charCount);
    tree_t* traverse = head;

    j = 0;
    while (!feof(in)) {
        i = 31;
        fread(&buffer, sizeof(unsigned int), 1, in);
        while (i >= 0) {
            if (CHECK_BIT(i--, buffer)) {
                assert(traverse->right);
                traverse = traverse->right;
            } else {
                assert(traverse->left);
                traverse = traverse->left;
            }
            if (!traverse->right && !traverse->left) {
                if (traverse->leaf == END_OF_TEXT) 
                    return 1;
                fprintf(out, "%c", traverse->leaf);
                traverse = head;        
            }
        }
    }
    return -1; 
}

/* ==================================================================== *
 *  rebuildTree:                                                        *
 *  Rebuilds the huffman tree for a compressed binary file that is      *
 *  open from the header.                                               *
 * ==================================================================== */
tree_t* rebuildTree(FILE * in,
                    unsigned int charCount)
{
    char character;
    unsigned int codeword;
    int i, j, k;

    tree_t* head = calloc(1, sizeof(tree_t));
    tree_t* traverse;

    for (i = 0; i < charCount; i++) {
        fread(&character, sizeof(char), 1, in);
        fread(&codeword, sizeof(unsigned int), 1, in);

        traverse = head;

        k = 0;
        j = 31;
        while (!CHECK_BIT(j--, codeword));
            while (j >= 0) {
                k++;
                if (CHECK_BIT(j--, codeword)) {
                    if (!traverse->right) 
                        traverse->right = calloc(1, sizeof(tree_t));
                    traverse = traverse->right;
                } else {
                    if (!traverse->left)
                        traverse->left = calloc(1, sizeof(tree_t));
                    traverse = traverse->left;
                } 
            }
        if (!traverse)
            traverse = calloc(1, sizeof(tree_t));
        traverse->leaf = character;
    }
    return head;
}

/* ==================================================================== *
 *  buildTable:                                                         *
 *  Builds the codeword table from a Huffman Tree and assigns it to a   *
 *  t_node table.                                                       *
 * ==================================================================== */
t_node** buildTable(tree_t* head,
                    unsigned int charCount) 
{
    unsigned int i, codeword = 0x1, bindex = 1;
    unsigned int * index = calloc(1, sizeof(unsigned int));
    t_node** table = malloc(sizeof(t_node*)*charCount);
    traverseTree(table, head, codeword, bindex, index);
    return table;
}

/* ==================================================================== *
 *  traverseTree:                                                       *
 *  Adds char and codeword to each leaf in the tree.                    *
 * ==================================================================== */
void traverseTree(t_node** table,
                  tree_t* node,
                  unsigned int codeword,
                  unsigned char binIndex,
                  unsigned int * index) 
{
    if (!node->left && !node->right) {
        table[*index] = malloc(sizeof(t_node));
        table[*index]->character = node->leaf;
        table[*index]->codeword = codeword;
        (*index)++;
        return;
    } 
    SHIFT_CODE(codeword);
    if (node->left) {
        traverseTree(table, node->left, codeword, binIndex, index);
    }
    if (node->right) {
        WRITE_BIT(codeword);
        traverseTree(table, node->right, codeword, binIndex, index);
    }
}

/* ==================================================================== *
 *  file_length:                                                        *
 *  Returns the length in bits of an open file.                         *
 * ==================================================================== */
int file_length(FILE *f)
{
    int pos;
    int end;

    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    fseek(f, pos, SEEK_SET);
    return end;
}

/* ==================================================================== *
 *  timediff:                                                           *
 *  Returns the difference between two clock_t variables in             *
 *  milliseconds.                                                       *
 * ==================================================================== */
long timediff(clock_t t1, 
              clock_t t2)
{
    return ((double)t2-t1) / CLOCKS_PER_SEC * 1000;
}


