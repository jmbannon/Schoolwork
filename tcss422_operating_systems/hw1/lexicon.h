#ifndef _LEXICON_H_
#define _LEXICON_H_

int loadLexicon(const char * path);
void destroyLexicon();
int isWord(const char * str);
int isPrefix(const char * str);

#endif
