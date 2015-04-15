/*
==================================================================
File:   list.c

Author: Jesse Bannon
Class:  TCSS 333 A
HW:     HW 5
Desc:   Reads in two Civil War books and compares most
        frequent words between both books.
==================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

/*
==================================================================

    Prototypes not included in list.h

==================================================================
 */

static int getWordCount(const wordData* word);
static void swapWords(wordData* wordOne, wordData* wordTwo);
static void insertWord(wordData** firstWord, char* theWord, int story);
static int searchWord(wordData* theWordEntry, char* theWord, int story);
static char* cleanWord(char* dirtyWord);


/*
 * Returns smaller count of each book.
 */
static int getWordCount(const wordData* word) {
    if (word->littleRegimentCount < word->redBadgeCount)
        return word->littleRegimentCount;
    else
        return word->redBadgeCount;
}

/*
 * Swaps all values between two wordData structs besides the next pointer.
 */
static void swapWords(wordData* wordOne, wordData* wordTwo) {
    char tempWord[WORD_LENGTH];
    strcpy(tempWord, wordOne->word);
    int tempLittle = wordOne->littleRegimentCount;
    int tempRed = wordOne->redBadgeCount;


    strcpy(wordOne->word, wordTwo->word);
    wordOne->littleRegimentCount = wordTwo->littleRegimentCount;
    wordOne->redBadgeCount = wordTwo->redBadgeCount;

    strcpy(wordTwo->word, tempWord);
    wordTwo->littleRegimentCount = tempLittle;
    wordTwo->redBadgeCount = tempRed;
}

/*
 * Sorts words based on greatest frequency of word count.
 */
void sortWords(wordData* firstWord) {
    int firstCount, nextCount, swapped;
    wordData* i = firstWord;

    do {
        swapped = 0;
        i = firstWord;

        while (i->next != NULL) {
            firstCount = getWordCount(i);
            nextCount = getWordCount(i->next);

            if (firstCount < nextCount) {
                swapWords(i, i->next);
                swapped = 1;
            }
            i = i->next;
        }
    } while(swapped);
}

/*
 * Inserts word into wordData linked list.
 */
static void insertWord(wordData** firstWord, char* theWord, int story) {
    wordData* newWord = malloc(sizeof(wordData));
    if (newWord == NULL) {
        printf("Memory allocation failed.");
        return;
    }

    strcpy(newWord->word, theWord);
    //printf("inputting %s\n", theWord);

    if (story == 1)
        newWord->redBadgeCount = 1;
    else
        newWord->littleRegimentCount = 1;

    newWord->next = *firstWord;
    *firstWord = newWord;

}

/*
 * Searches for word within wordData linked list.
 * Returns 1 if found, 0 if not found.
 */
static int searchWord(wordData* theWordEntry, char* theWord, int story) {
    wordData* wordEntry = theWordEntry;
    while (wordEntry != NULL) {
        if (strcmp(wordEntry->word, theWord) == 0) {
            if (story == 1)
                wordEntry->redBadgeCount++;
            else
                wordEntry->littleRegimentCount++;
            return 1;
        }
        wordEntry = wordEntry->next;
    }
    return 0;
}

/*
 * Lowercases all characters within a word and removes all other characters
 * besides apostrophes and dashes.
 */
static char* cleanWord(char* dirtyWord) {
    char tempWord[WORD_LENGTH];
    strcpy(tempWord, dirtyWord);

    int i;
    int cleanSize = 0;
    for (i = 0; i < strlen(tempWord); i++) {
        if (isalpha(tempWord[i]) || tempWord[i] == '\'' || tempWord[i] == '-')
            dirtyWord[cleanSize++] = tolower(tempWord[i]);
    }
    dirtyWord[cleanSize] = '\0';
    return dirtyWord;
}

/*
 * Reads file of specified fileName and stores word frequency into
 * wordData linked list. int story indicates which story count to increment.
 */
void readFile(wordData **firstWord, char* fileName, int story) {
    FILE *infile = fopen(fileName, "r");
    char tempWord[WORD_LENGTH];

    while(fscanf(infile, "%s", tempWord) == 1) {
        if (searchWord(*firstWord, cleanWord(tempWord), story) == 0)
            insertWord(firstWord, tempWord, story);
    }
}

/*
 * Prints list of top 50 words in linked list to console.
 */
void printList(wordData* word) {
    int i;
    wordData* tempWord = word;

    printf("Word\t\tRedBadge\tLittleRegiment\n");
    printf("-------------------------------------------------\n");
    for (i = 0; i < 50; i++) {
        printf("%s\t\t%d\t\t%d\n", tempWord->word,
                tempWord->redBadgeCount, tempWord->littleRegimentCount);
        tempWord = tempWord->next;
    }
}
