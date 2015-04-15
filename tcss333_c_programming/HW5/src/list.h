/*
==================================================================
File:   list.h

Author: Jesse Bannon
Class:  TCSS 333 A
HW:     HW 5
Desc:   Reads in two Civil War books and compares most
        frequent words between both books.
==================================================================
 */

#ifndef LIST_H_
#define LIST_H_
#define WORD_LENGTH 40

struct word_tag {
    char word[WORD_LENGTH];
    int redBadgeCount;
    int littleRegimentCount;
    struct word_tag *next;
};

typedef struct word_tag wordData;


/*
==================================================================

    Function prototypes used in main.c

==================================================================
 */

void sortWords(wordData* firstWord);
void readFile(wordData **firstWord, char* fileName, int story);
void printList(wordData* word);

#endif /* LIST_H_ */
