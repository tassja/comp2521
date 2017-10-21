#ifndef scaledFootRule_H
#define scaledFootRule_H

typedef struct urlNode *UrlNode;
typedef struct urlList *UrlList;

// function definitions
void appendUrl(UrlList l, UrlNode node);
UrlNode newUrlNode(char *url);
UrlList newUrlList();
void freeUrlList(UrlList l);
void addUrlAlphabetically(UrlList l, UrlNode node);
float calculateW(UrlList list[], int numFiles, UrlNode permutation[], float numUls);
long factorial(int a);
float absVal(float a);
void printMinW(UrlList list[], int numFiles, UrlNode permutation[], float length);

#endif
