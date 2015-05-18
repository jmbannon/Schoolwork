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

typedef struct __tree_t_ {
    struct __tree_t_ *left;
    struct __tree_t_ *right;
    unsigned int weight, codeword;
    char * leaf;    
} tree_t;

typedef struct __table_n_ {
    unsigned char * key;
    unsigned int value;
} t_node;

static statistics stats;

/*
 ====================================================================
 =  Function Prototypes  ============================================
 ====================================================================
*/

/* Encode (Compress) Functions */
int Encode(FILE *in, 
           FILE *out);

tree_t* buildTree(t_node * hashTable, 
                  unsigned int * charCount);

unsigned int getCodeWord(t_node * hashTable,
                         unsigned char * c);

void printCodewords(t_node * hashTable, 
                    unsigned int charCount);

void writeHeader(FILE *out, 
                 t_node * hashTable,
                 unsigned int charCount);

tree_t** merge(tree_t** tree_1, 
               tree_t** tree_2, 
               size_t size1, 
               size_t size2);

tree_t** mergeSort(tree_t** tree_a, 
                   size_t size);

void prioritySort(tree_t** tree_a, 
                  size_t size);

/* Hash Table Functions */
t_node * buildHashTable();

unsigned int getHashCode(char * key);

int inputWord(t_node * table,
              unsigned int index,
              char * word);

unsigned int get(t_node * table,
                 unsigned int key,
                 char * word);

int putIncrement(t_node * table,
                 unsigned int key,
                 char * word);

void putCodeword(t_node * table,
                 unsigned int key,
                 char * word,
                 unsigned int codeword);

/* Decode (Decompress) Functions */
int Decode(FILE * in,
           FILE * out);

tree_t* rebuildTree(FILE * in,
                    unsigned int charCount);

void buildTable(tree_t * head,
                t_node * hashTable,
                unsigned int charCount);

void traverseTree(t_node * hashTable,
                  tree_t * node,
                  unsigned int codeword,
                  unsigned char binIndex);

/* Statistic Functions */
void hashStats(unsigned int entries,
               unsigned int probes[]);

int file_length(FILE *f);

long timediff(clock_t t1, 
              clock_t t2);


/*
 ====================================================================
 =  Functions  ======================================================
 ====================================================================
*/


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

    in = fopen(filename, "r");
    out = fopen(outputname, "w");
    if (!in || !out) {
        stats.valid = 0;
        return stats;
    }

    t1 = clock();
    Encode(in, out);
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

void writeCodeword(unsigned int codeword,
                   unsigned int * bitBuffer,
                   unsigned int * bufferSize,
                   FILE * out)
{
    int bindex = 31;
    while (!CHECK_BIT(bindex--, codeword));
    while (bindex >= 0) {
        if (CHECK_BIT(bindex--, codeword))
            ENCODE_BIT((*bufferSize)++, *bitBuffer);
        else
            (*bufferSize)++;
 	if (*bufferSize == 32) {
            fwrite(bitBuffer, sizeof(unsigned int), 1, out);
            *bufferSize = 0;
            *bitBuffer = 0;
        }
    }
}

/* ==================================================================== *
 *  Encode:                                                             *
 *  Encodes an opened text file and writes to an opened binary file.    *
 * ==================================================================== */
int Encode(FILE *in, 
           FILE *out) 
{
    unsigned int bufferSize, bitBuffer, codeword, charCount;
    char c, wordBuffer[64];
    int bindex;
    
    unsigned int idx = 0, count = 0;

    t_node * hashTable = buildHashTable();
    
    while ((c = fgetc(in)) != EOF) {
        if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
            wordBuffer[idx++] = c;
        else {
            wordBuffer[idx] = '\0';
            if (idx != 0)
                putIncrement(hashTable, getHashCode(wordBuffer), wordBuffer);

            wordBuffer[0] = c;
            wordBuffer[1] = '\0';
            putIncrement(hashTable, getHashCode(wordBuffer), wordBuffer);
            idx = 0;
        }
    }
    rewind(in);
    wordBuffer[0] = END_OF_TEXT;
    wordBuffer[1] = '\0';
    putIncrement(hashTable, getHashCode(wordBuffer), wordBuffer);

    tree_t* head = buildTree(hashTable, &charCount);
    buildTable(head, hashTable, charCount);
    stats.entries = charCount;
 
    writeHeader(out, hashTable, charCount);
    printCodewords(hashTable, charCount);
    bitBuffer = 0;
    bufferSize = 0;
    idx = 0;
    while ((c = fgetc(in)) != EOF) {
        if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
            wordBuffer[idx++] = c;
        else {
            wordBuffer[idx] = '\0';
            if (idx != 0) {
                codeword = getCodeWord(hashTable, wordBuffer);
                writeCodeword(codeword, &bitBuffer, &bufferSize, out);
            }

            wordBuffer[0] = c;
            wordBuffer[1] = '\0';
            codeword = getCodeWord(hashTable, wordBuffer);
            writeCodeword(codeword, &bitBuffer, &bufferSize, out);
            idx = 0;
        }
    }
    wordBuffer[0] = END_OF_TEXT;
    wordBuffer[1] = '\0';
    codeword = getCodeWord(hashTable, wordBuffer);
    writeCodeword(codeword, &bitBuffer, &bufferSize, out);
        
    if (bufferSize)
        fwrite(&bitBuffer, sizeof(unsigned int), 1, out);
}

/* ==================================================================== *
 *  buildTree:                                                          *
 *  Builds a Huffman Tree using array of character frequencies.         *
 * ==================================================================== */
tree_t* buildTree(t_node * hashTable, 
                  unsigned int * charCount) 
{
    int i, len = 0;
    tree_t** tqueue = calloc(HASH_TABLE_SIZE, sizeof(tree_t*));

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i].value > 0) {
            tree_t* temp = calloc(1, sizeof(tree_t));
            temp->leaf = hashTable[i].key;
            temp->weight = hashTable[i].value;
            tqueue[len++] = temp;
        }
    }
    *charCount = len;
    tqueue = mergeSort(tqueue, len);

    while (len > 1) {
        tree_t* toAdd = calloc(1, sizeof(tree_t));
        toAdd->weight = 0;

        toAdd->left = tqueue[--len];
        toAdd->weight += tqueue[len]->weight;

        toAdd->right = tqueue[--len];
        toAdd->weight += tqueue[len]->weight;
        tqueue[len++] = toAdd;

        prioritySort(tqueue, len);
    }
    tree_t* head = calloc(1, sizeof(tree_t*));
    head = tqueue[0];
    free(tqueue);
    return head;
}

/* ==================================================================== *
 *  getCodeWord:                                                        *
 *  Returns the codeword from the table for the character given.        *
 * ==================================================================== */
unsigned int getCodeWord(t_node * table, 
                         unsigned char * c) 
{
    unsigned int hashCode = getHashCode(c);
    unsigned int index = get(table, hashCode, c);
    return table[index].value;
}

/* ==================================================================== *
 *  printCodewords:                                                     *
 *  Writes all codeword's binary equivalence to a text file.            *
 * ==================================================================== */
void printCodewords(t_node * hashTable, 
                    unsigned int charCount)
{
    FILE * out;
    int i, j;
    
    out = fopen("codewords.txt", "w");

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i].key != NULL) {
            fprintf(out, "%s : ", hashTable[i].key);
            j = 31;
            while(!CHECK_BIT(j--, hashTable[i].value));
            while(j >= 0) {
                if (CHECK_BIT(j--, hashTable[i].value))
                    fprintf(out, "%d", (int)1);
                else
                    fprintf(out, "%d", (int)0);
            }
            fprintf(out, "\n");
        }
    }
    fclose(out);
}

/* ==================================================================== *
 *  writeHeader:                                                        *
 *  Writes the test number, amount of unique chars, and the chars       *
 *  corresponding to the table followed by their codeword.              *
 * ==================================================================== */
void writeHeader(FILE *out, 
                 t_node * hashTable,
                 unsigned int charCount) 
{
    int i, len, j = 0;
    const unsigned int verification = 13370666;

    fwrite(&verification, sizeof(unsigned int), 1, out);
    fwrite(&charCount, sizeof(unsigned int), 1, out);

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i].key != NULL) {
            len = strlen(hashTable[i].key);
            fwrite(&len, sizeof(int), 1, out);
            fwrite(hashTable[i].key, sizeof(char), len, out);
            fwrite(&hashTable[i].value, sizeof(unsigned int), 1, out);
            ++j;
        }
    }
    assert(charCount == j);
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
    tree_t** toReturn = calloc(size1+size2, sizeof(tree_t*));
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
    
    tree_t** tree_1 = calloc(size1, sizeof(tree_1));
    tree_t** tree_2 = calloc(size2, sizeof(tree_2));
    
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
 *  prioritySort:                                                       *
 *  Use only when the new value is appended to the end of the array.    *
 *  Bubble sorts starting with the last value until the value prior to  *
 *  it is less than its value.                                          *
 * ==================================================================== */
void prioritySort(tree_t** tree_a, 
                  size_t size) 
{
    int idx = size - 1;
    while (idx > 0) {
        if (tree_a[idx]->weight > tree_a[idx-1]->weight) {
            tree_t * temp = tree_a[idx];
            tree_a[idx] = tree_a[idx-1];
            tree_a[idx-- -1] = temp;
        } else
            return;
    }
}

/* ==================================================================== *
 *  buildHashTable:                                                     *
 *  Initializes a hash table with HASH_TABLE_SIZE elements              *
 *  (defined in header file).                                           *
 * ==================================================================== */
t_node * buildHashTable()
{
    t_node * hash_table = calloc(HASH_TABLE_SIZE, sizeof(tree_t));
    if (hash_table == NULL)
        return NULL; // ERROR
    return hash_table;
}

/* ==================================================================== *
 *  getHashCode:                                                        *
 *  Returns the hash code for the string given.                         *
 * ==================================================================== */
unsigned int getHashCode(char * key)
{
    size_t len = strlen(key);
    unsigned int i, hash = 0, seed = 37;
    for (i = 0; i < len; i++)
        hash = (hash * seed) + key[i];
    return hash % HASH_TABLE_SIZE;
}

/* ==================================================================== *
 *  inputWord:                                                          *
 *  Inputs the word into the hash table at the specified index.         *
 * ==================================================================== */
int inputWord(t_node * table,
              unsigned int index,
              char * word)
{
    size_t len = strlen(word);
    table[index].key = calloc(len+1, sizeof(char));
    strcpy(table[index].key, word);
    table[index].key[len] = '\0';
    table[index].value = 1;
}

/* ==================================================================== *
 *  get:                                                                *
 *  Returns the true index of the word. If the word does not exist      *
 *  in the hash table it returns the index it should belong to.         *
 * ==================================================================== */
unsigned int get(t_node * table,
                 unsigned int key,
                 char * word)
{
    unsigned int idx, i = 0;
    if (table[key].key == NULL || strcmp(table[key].key, word) == 0) {
        ++(stats.probes[0]);
        return key;
    } else
        do {
            idx = (++i*i) + key % HASH_TABLE_SIZE;
            if (table[idx].key == NULL || strcmp(table[idx].key, word) == 0) {
                ++(stats.probes[i]);
                return idx;
            }
        } while (1);
}

/* ==================================================================== *
 *  putIncrement:                                                       *
 *  Finds the true hash index of the given word and increments the      *
 *  value. If the hash index is null then it is initialized with the    *
 *  word and value is set to one.                                       *
 * ==================================================================== */
int putIncrement(t_node * table,
                 unsigned int key,
                 char * word)
{
    unsigned int idx = get(table, key, word);
    if (table[idx].key == NULL) {
        inputWord(table, idx, word);
        return 1;
    } else {
        ++(table[idx].value);
        return 0;
    }
}

/* ==================================================================== *
 *  putCodeword:                                                        *
 *  Finds the true hash index of the given word and sets the value      *
 *  to the codeword given.                                              *
 * ==================================================================== */
void putCodeword(t_node * table,
                 unsigned int key,
                 char * word,
                 unsigned int codeword)
{
    unsigned int idx = get(table, key, word);
    table[idx].value = codeword;
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
                if (traverse->leaf[0] == END_OF_TEXT) 
                    return 1;
                fprintf(out, "%s", traverse->leaf);
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
    char * character;
    unsigned int codeword;
    int i, j, k, len;

    tree_t* head = calloc(1, sizeof(tree_t));
    tree_t* traverse;

    for (i = 0; i < charCount; i++) {
        fread(&len, sizeof(int), 1, in);
        character = calloc(len+1, sizeof(char));

        fread(character, sizeof(char), len, in);
        character[len] = '\0';
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
void buildTable(tree_t* head,
                t_node * hashTable,
                unsigned int charCount) 
{
    unsigned int i, codeword = 0x1, bindex = 1;
    traverseTree(hashTable, head, codeword, bindex);
}

/* ==================================================================== *
 *  traverseTree:                                                       *
 *  Adds char and codeword to each leaf in the tree.                    *
 * ==================================================================== */
void traverseTree(t_node * hashTable,
                  tree_t* node,
                  unsigned int codeword,
                  unsigned char binIndex) 
{
    if (!node->left && !node->right) {
        unsigned int idx = get(hashTable, getHashCode(node->leaf), node->leaf);
        hashTable[idx].value = codeword;
        return;
    } 
    SHIFT_CODE(codeword);
    if (node->left) {
        traverseTree(hashTable, node->left, codeword, binIndex);
    }
    if (node->right) {
        WRITE_BIT(codeword);
        traverseTree(hashTable, node->right, codeword, binIndex);
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


