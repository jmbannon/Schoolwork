#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "palindrome.h"

char ** loadWords(const char * path, int * wordsLoaded) {
	char buffer[81];
	FILE * fp;

	fp = fopen(path, "r");
	if (fp == NULL)
		return NULL;

	int size = 10;
	char ** words = malloc(size * sizeof(char *));
	int used = 0;

	while (fgets(buffer, 81, fp) != NULL) {
		if (used == size) {
			char ** tmp = malloc(2 * size * sizeof(char *));
			memcpy(tmp, words, size * sizeof(char *));
			free(words);
			words = tmp;
			size *= 2;
		}
		int i;
		for (i = 0; buffer[i] != '\0' && buffer[i] != '\r' && buffer[i] != '\n'; i++);
		words[used] = malloc((i + 1) * sizeof(char));
		memcpy(words[used], buffer, i);
		words[used][i] = '\0';
		used++;
	}

	fclose(fp);

	*wordsLoaded = used;
	return words;
}

int main(int argc, char* argv[]) {
	char ** words;
	int size;

	words = loadWords("words.txt", &size);
	if (words == NULL) {
		fprintf(stderr, "There was an error loading 'words.txt'\n");
		return EXIT_FAILURE;
	}

	int palindromesFound;
	char ** palindromes = multithreaded_findPalindromes(words, size, &palindromesFound, 4);
	printf("%d palindromes found.\n", palindromesFound);
	for (int i = 0; i < palindromesFound; i++)
		printf("%s\n", palindromes[i]);

	free(palindromes);
	for (int i = 0; i < size; i++)
		free(words[i]);
	free(words);

	return EXIT_SUCCESS;
}
