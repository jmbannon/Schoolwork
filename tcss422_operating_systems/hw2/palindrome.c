#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "palindrome.h"

typedef struct __threadArg_ 
{
    char* word;
    int isPalindrome;
} T;

void* isPalindrome(void* arg) 
{
    int i;
    T* ret = (T*)arg;
    size_t len = strlen(ret->word);
  
    for (i = 0; i < len/2; i++)
    {
        if (ret->word[i] != ret->word[len-1-i])
        {
            ret->isPalindrome = 0;
            return (T*)ret;
        }
    }
    ret->isPalindrome = 1;
    pthread_exit(ret);
}

char ** multithreaded_findPalindromes(char ** words, 
                                      int numOfWords, 
                                      int * palindromesFound, 
                                      int numThreads) 
{
    *palindromesFound = 0;
    pthread_t threads[numThreads];
    T* ret[numThreads];
    char** palindromeWords = malloc(numOfWords * sizeof(char*));
    int i = 0, j;

    while (i < numOfWords)
    {
        for (j = 0; j < numThreads; j++)
        {
            ret[j] = malloc(sizeof(T));
            ret[j]->word = words[i++];

            pthread_create(&threads[j], 
                           NULL, 
                           (void*)isPalindrome, 
                           (void*)ret[j]);
        }

        for (j = 0; j < numThreads; j++)
        {
            pthread_join(threads[j], (void*)&ret[j]);
            if (ret[j]->isPalindrome)
            {
                palindromeWords[(*palindromesFound)++] = ret[j]->word;
            }
            free(ret[j]);
        }
    }	
    return palindromeWords;
}
