#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

struct Card {
	char rank;
	char suit;
};

struct ListNode {
	char * word;
	struct ListNode * next;
};

void multiply(int num1, int denom1, int num2, int denom2, int * rNum, int * rDenom);
char * rotate(const char * str, int amount);
int readAndDisplayBookInformation(const char * path);
void initializeAndShuffleDeck(struct Card deck[52]);
struct ListNode * findWords(const char board[4][4]);

#endif
