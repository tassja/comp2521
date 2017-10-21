#ifndef inverted_H
#define inverted_H

typedef struct wordNode *WordNode;
typedef struct wordList *WordList;
typedef struct urlNode* UrlNode;
typedef struct urlList* UrlList;

void normalise(char *str);
void appendUrl(UrlList l, UrlNode node);
void addWordAlphabetically(WordList l, WordNode node);
UrlNode newUrlNode(char *url);
UrlList newUrlList();
WordList newWordList();
WordNode newWordNode(char *word);
WordNode findWord(WordList l, char *word);
void freeWordList(WordList l);


#endif
