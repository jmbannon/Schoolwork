#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicon.h"

typedef struct Node {
	int isWord;
	int isPrefix;
	struct Node * letter[26];
} Node;

static Node root = {0, 1, {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};

void addWord(const char * str) {
	Node * current = &root;
	Node * previous = NULL;
	while (*str != '\0' && *str != '\n' && *str != '\r') {
		if (current->letter[*str - 'a'] == NULL) {
			current->letter[*str - 'a'] = (Node *)malloc(sizeof(Node));
			memset(current->letter[*str - 'a'], 0, sizeof(Node));
		}
		if (previous != NULL)
			previous->isPrefix = 1;
		previous = current;
		current = current->letter[*str - 'a'];
		str++;
	}
	if (previous != NULL)
		previous->isPrefix = 1;
	current->isWord = 1;
}

int loadLexicon(const char * path) {
	char buffer[81];
	FILE * fp;

	fp = fopen(path, "r");
	if (fp == NULL)
		return 0;

	while (fgets(buffer, 81, fp) != NULL)
		addWord(buffer);

	fclose(fp);
	return 1;
}

void destroyLexiconHelper(Node * n) {
	int i;
	for (i = 0; i < 26; i++)
		if (n->letter[i] != NULL) {
			destroyLexiconHelper(n->letter[i]);
			free(n->letter[i]);
		}
}

void destroyLexicon() {
	destroyLexiconHelper(&root);
	memset(&root, 0, sizeof(Node));
	root.isPrefix = 1;
}

Node * findNode(const char * str) {
	Node * current = &root;
	while (*str != '\0') {
		if (current->letter[*str - 'a'] == NULL)
			return NULL;
		current = current->letter[*str - 'a'];
		str++;
	}
	return current;
}

int isWord(const char * str) {
	Node * n = findNode(str);
	return n == NULL ? 0 : n->isWord;
}

int isPrefix(const char * str) {
	Node * n = findNode(str);
	return n == NULL ? 0 : n->isPrefix;
}
