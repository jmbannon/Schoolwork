#include <stdio.h>
#include <stdlib.h>
#include "HuffmanTree.h"

typedef struct __tree_t_ {
    struct __tree_t_ *left;
    struct __tree_t_ *right;
    unsigned int weight;
    char leaf;    
} tree_t;

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

tree_t* buildTree(unsigned int freq[], unsigned int * charCount) {
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
        tqueue = mergeSort(tqueue, len);

        toAdd->left = tqueue[--len];
        toAdd->right = tqueue[--len];
        tqueue[len++] = toAdd;
    }
    tree_t* head = malloc(sizeof(tree_t*));
    head = tqueue[0];
    free(tqueue);
    return head;
}

void buildTable(char** table,
                unsigned int charCount,
                tree_t* head) 
{
  
}

int Encode(FILE *in, FILE *out) {
    unsigned int c, charCount, frequency[CHAR_RANGE] = { 0 };
    char** table;
    
    while ((c = fgetc(in)) != EOF) frequency[c]++;
    rewind(in);
  
    tree_t* head = buildTree(frequency, &charCount);

}

int Compress(const char * filename, 
             const char * outputname) 
{
    FILE * in;
    FILE * out;
    in = fopen(filename, "r");
    Encode(in, out);
}
