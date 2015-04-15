/*
 ===============================================================

 Author:	Jesse Bannon
 Class:		TCSS 422: Operating Systems
 HW Num:	Homework 1

 HW Desc:	Complete the functions correctly

 ===============================================================
*/
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

/*
 ===============================================================

	Encapsulated Function Prototypes

 ===============================================================
*/

int GCF(int x, 
        int y);

int containsChar(char c, 
                 char* word);

char getAdjacentChar(int x, 
                     int y,  
                     const char board[4][4]);

/*
 ===============================================================

	Functions

 ===============================================================
*/

int GCF(int x, 
        int y)
{
    if (x == 0 || y == 0)
        return y;

    if ((x % y) == 0)
        return y;
    else
        return GCF(y, (x % y));
}

void multiply(int num1,
              int denom1,
              int num2, 
              int denom2, 
              int * rNum, 
              int * rDenom) 
{
    *rNum = num1 * num2;
    *rDenom = denom1 * denom2;

    if (*rDenom < 0)
    {
        *rDenom *= -1;
        *rNum *= -1;    
    }

    int gcfNum = GCF(*rNum, *rDenom);
    int gcfDen = GCF(*rDenom, *rNum);

    if (gcfNum != *rNum)
    {
         *rNum /= gcfNum;
         *rDenom /= gcfNum;      
    }
    else if (gcfDen != *rDenom)
    {
        *rNum /= gcfDen;
        *rDenom /= gcfDen;
    }
}

char * rotate(const char * str, 
              int amount) 
{
    int i, len;
    len = strlen(str);
    char* rotStr = malloc(len + 1);
    if (rotStr == NULL) 
    {
        printf("Could not allocate enough memory. Returning null.\n");
        return NULL;
    }
   
    rotStr[len] = '\0';

    while (amount < 0)
    {
        amount += len;
    }

    for (i = 0; i < len; i++)
    {
        rotStr[(i + amount) % len] = str[i];
    }
    return rotStr;
}

int readAndDisplayBookInformation(const char * path) {
    FILE * infile;
    char line[80], comma[2] = ",", *token;
    int i;

    infile = fopen(path, "r");
    
    if (infile == NULL)
        return 1;

    while (fgets(line, 80, infile) != NULL)
    {
        token = strtok(line, comma);
        printf("\"%s\" ", token, '"'); 

        token = strtok(NULL, comma);
        printf("by %s ", token);

        token = strtok(NULL, comma);

        printf("(%.4s)\n", token);
        memset(token, 0, strlen(token));
    }

    //fclose(path);    
}

void initializeAndShuffleDeck(struct Card deck[52]) 
{
    int i, randIndex, offset = 0;
    char rank, suite;
    struct Card temp;

    while (offset < 52)
    {
        switch(offset)
        {
            case 0: suite = 'c'; break;
            case 13: suite = 's'; break;
            case 26: suite = 'h'; break;
            case 39: suite = 'd'; break;
            default: return;
        }
     
        for (i = 0; i < 13; i++)
        {
            deck[offset+i].suit = suite;
        }

        deck[offset].rank = 'A';
        for (i = 1; i < 9; i++)
        {
            deck[offset+i].rank = (char)(((int)'1')+i);
        }
        deck[offset + 9].rank = '0';
        deck[offset + 10].rank = 'J';
        deck[offset + 11].rank = 'Q';
        deck[offset + 12].rank = 'K';
       
        offset += 13;        
    }

    for (i = 51; i > 0; i--)
    {
        temp = deck[i];
        randIndex = rand() % 52;
        deck[i] = deck[randIndex];
        deck[randIndex] = temp;
    }
}

int containsChar(char c, 
                 char* word)
{
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        if (c == word[i])
            return 1;
    }
    return 0;
}

char getAdjacentChar(int x, 
                     int y,  
                     const char board[4][4])
{
    if (x >= 0 && x < 4
     && y >= 0 && y < 4) 
        return board[x][y];
    
    else
        return 0;
}

void findWord(char* currentWord, const char board[4][4], int position, struct ListNode** HEAD)
{
    int i, j, index;
    int row = position / 4;
    int col = position % 4;

    char* newWord = malloc(sizeof(char) * 33);
    if (newWord == NULL) 
    {
        printf("Could not allocate enough memory. Returning.\n");
        return;
    }
    
    memset(newWord, 0, 32);

    char adjacent;

    strcpy(newWord, currentWord);
    index = strlen(newWord);

    for (i = row-1; i <= row+1; i++)
    {
        for (j = col-1; j <= col+1; j++)
        {
            adjacent = getAdjacentChar(i, j, board);

            if (adjacent != 0 && !containsChar(adjacent, newWord))
            {
                newWord[index] = board[i][j];

                if (isWord(newWord))
                {
                    struct ListNode* newNode = malloc(sizeof(struct ListNode));
                    newNode->word = malloc(strlen(newWord) + 1);
                    if (newNode == NULL || newNode->word == NULL) 
                    {
                        printf("Could not allocate enough memory. Returning.\n");
                        return;
                    }
                    strcpy(newNode->word, newWord);
                    newNode->next = *HEAD;
                    *HEAD = newNode;
                    ++index;
                }

                if (isPrefix(newWord))
                {
                    findWord(newWord, board, i*4+j, HEAD);
                }                  
            }
        }
    }
    free(newWord);
}

struct ListNode * findWords(const char board[4][4]) {
    int i, j;
    char* word = malloc(sizeof(char) * 33);
    struct ListNode* HEAD = malloc(sizeof(struct ListNode*));
    if (word == NULL || HEAD == NULL) 
    {
        printf("Could not allocate enough memory. Returning null.\n");
        return NULL;
    }

    memset(word, 0, 33);
    HEAD = NULL;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            word[0] = board[i][j];
            findWord(word, board, i*4+j, &HEAD);
        }
    }
    free(word);
    return HEAD;
}

