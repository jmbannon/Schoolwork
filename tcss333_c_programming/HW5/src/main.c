/*
==================================================================
File:   main.c

Author: Jesse Bannon
Class:  TCSS 333 A
HW:     HW 5
Desc:   Reads in two Civil War books and compares most
        frequent words between both books.
==================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(void) {

    wordData* firstWord = NULL;
    readFile(&firstWord, "RedBadge.txt", 1);
    readFile(&firstWord, "LittleRegiment.txt", 0);

    sortWords(firstWord);
    printList(firstWord);

    return 0;

}
