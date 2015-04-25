#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HuffmanTree.h"
#define SHIFT_CODE(c) (c<<=1)
#define WRITE_BIT(c) (c |= 0x1)
#define ENCODE_BIT(i,c) (c |= (0x1 << 31-i))
#define CHECK_CODE(i,c) (c & (0xFFFE << i))
#define CHECK_BIT(i,c) (c & (0x1 << i))

typedef struct __tree_t_ {
    struct __tree_t_ *left;
    struct __tree_t_ *right;
    unsigned int weight;
    char leaf;    
} tree_t;

typedef struct __table_n_ {
    char character;
    unsigned int codeword;
} t_node;

typedef struct __code_d_ {
    tree_t** tree;
    t_node** table;
    unsigned int wordCount;
} code_d;

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
             

t_node** buildTable(tree_t* head,
                  unsigned int charCount) 
{
    unsigned int i, codeword = 0x1, bindex = 1;
    unsigned int * index = calloc(1, sizeof(unsigned int));
    t_node** table = malloc(sizeof(t_node*)*charCount);
    traverseTree(table, head, codeword, bindex, index);
    return table;
}

void printCodewords(t_node** table, 
                    unsigned int charCount)
{
    FILE * out;
    unsigned int i, j;

    out = fopen("test.txt", "w");

    for (i = 0; i < charCount; i++) {
        fprintf(out, "%c : ", table[i]->character);
        j = 1;
        while(CHECK_CODE(j, table[i]->codeword)) {
            if (CHECK_BIT(j++, table[i]->codeword))
                fprintf(out, "%d", (int)1);
            else
                fprintf(out, "%d", (int)0);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

unsigned int getCodeWord(t_node** table, 
                         unsigned int charCount,
                         unsigned char c) 
{
    int i;
    for (i = 0; i < charCount; i++) {
        if (c != table[i]->character) continue;
        else return table[i]->codeword;
    }
 
    return 0;
}

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

int Encode(FILE *in, 
           FILE *out) 
{
    unsigned int c, bindex, bufferSize, bitBuffer, codeword, charCount;
    unsigned int frequency[CHAR_RANGE] = { 0 };;
    t_node** table;
    
    while ((c = fgetc(in)) != EOF) frequency[c]++;
    rewind(in);

    tree_t* head = buildTree(frequency, &charCount);
    table = buildTable(head, charCount);
 
    writeHeader(out, table, charCount);

    bitBuffer = 0;
    bufferSize = 0;
    while ((c = fgetc(in)) != EOF) {
        codeword = getCodeWord(table, charCount, c);
        bindex = 1;

        while (CHECK_CODE(bindex, codeword)) {
            if (CHECK_BIT(bindex++, codeword))
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
}

void buildLeaf(tree_t* head,
               unsigned int codeword, 
               char character)
{
    int i = 31;
    tree_t* traverse = head;
    int j = 0;
    while (!CHECK_BIT(i--, codeword));
        while (i > 0) {
            j++;
            if (CHECK_BIT(i--, codeword)) {
                if (!traverse->right)
                    traverse->right = malloc(sizeof(tree_t));
                traverse = traverse->right;
            } else {
                if (!traverse->left)
                    traverse->left = malloc(sizeof(tree_t));
                traverse = traverse->left;
            } 
        }
    printf("writing %c at depth %d\n", character, j);
    traverse = malloc(sizeof(tree_t));
    traverse->leaf = character;
}

int Decode(FILE * in,
           FILE * out)
{
    unsigned int i, j, charCount, buffer;
    char character;

    tree_t *head = malloc(sizeof(tree_t));
    tree_t *traverse;

    fread(&j, sizeof(unsigned int), 1, in);
    if (j != 13370666) return -1;

    fread(&charCount, sizeof(unsigned int), 1, in); 

    for (i = 0; i < charCount; i++) {
        fread(&character, sizeof(char), 1, in);
        fread(&j, sizeof(unsigned int), 1, in);
        buildLeaf(head, j, character);
    }
  
    traverse = head;

    while (!feof(in)) {
        i = 31;
        fread(&buffer, sizeof(unsigned int), 1, in);
        while (i > 0) {
            if (CHECK_BIT(i--, buffer)) {
                if (!traverse->right) {
                    fprintf(out, "%c", traverse->leaf);
                    //printf("printing %c\n", traverse->leaf);
                    traverse = head;
                } else
                    traverse = traverse->right;
            } else {
                if (!traverse->left) {
                    fprintf(out, "%c", traverse->leaf);
                    traverse = head;
                } else
                    traverse = traverse->left;
            }
        }
    }
  
   
}

int Decompress(const char * filename,
               const char * outputname) 
{
    FILE * in;
    FILE * out;
    in = fopen(filename, "rb");
    out = fopen(outputname, "w");
    Decode(in, out);
    fclose(in);
    fclose(out);
}

int Compress(const char * filename, 
             const char * outputname) 
{
    FILE * in;
    FILE * out;
    in = fopen(filename, "r");
    out = fopen(outputname, "w");
    Encode(in, out);
    fclose(in);
    fclose(out);
}
