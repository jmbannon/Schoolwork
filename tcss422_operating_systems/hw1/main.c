#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include <sys/time.h>
#else
#include <time.h>
#endif
#include "functions.h"
#include "lexicon.h"

void drive_multiply() {
	int num, denom;
	multiply(2, 3, 7, 5, &num, &denom);
	printf("2/3 * 7/5 = %d/%d\n", num, denom);
	multiply(14, 9, 3, 2, &num, &denom);
	printf("14/9 * 3/2 = %d/%d\n", num, denom);
	multiply(8, 6, 12, 20, &num, &denom);
	printf("8/6 * 12/20 = %d/%d\n", num, denom);
	multiply(-8, 6, 12, 20, &num, &denom);
	printf("-8/6 * 12/20 = %d/%d\n", num, denom);
	multiply(1, 1, -1, 1, &num, &denom);
	printf("1/1 * -1/1 = %d/%d\n", num, denom);
	multiply(0, 2, 0, 3, &num, &denom);
	printf("0/2 * 0/3 = %d/%d\n", num, denom);
}

void drive_rotate() {
	char message[] = "Hello";
 	int i;
	for (i = 0; i <= 10; i++) {
		char * tmp = rotate(message, i);
		printf("%s\n", tmp);
		free(tmp);
	}
	for (i = -1; i >= -10; i--) {
		char * tmp = rotate(message, i);
		printf("%s\n", tmp);
		free(tmp);
	}
}

void drive_readAndDisplayBookInformation() {
	readAndDisplayBookInformation("books.txt");
}

void drive_initializeAndShuffleDeck() {
	struct Card deck[52];
	initializeAndShuffleDeck(deck);
	int i;
	for (i = 0; i < 52; i++) {
		printf("%c%c ", deck[i].rank, deck[i].suit);
		if (i % 13 == 12)
			printf("\n");
	}
}

void drive_findWords() {
	loadLexicon("words.txt");
	const char board[4][4] = {{'d', 'h', 'h', 'i'}, {'j', 'e', 'p', 's'}, {'i', 't', 'z', 't'}, {'a', 'l', 'm', 't'}};
	struct ListNode * words = findWords(board);
	struct ListNode * current = words;
	while (current != NULL) {
		printf("%s\n", current->word);
		current = current->next;
	}
	current = words;
	while (current != NULL) {
		words = current->next;
		free(current->word);
		free(current);
		current = words;
	}
	destroyLexicon();
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	drive_multiply();
	drive_rotate();
	drive_readAndDisplayBookInformation();
	drive_initializeAndShuffleDeck();
	drive_findWords();
	return EXIT_SUCCESS;
}
