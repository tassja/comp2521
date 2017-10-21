////////////////////////////////////////////////////////////////////////
// nameofgroup - Tom Day, Julian Jankowski, Tassja Kriek              //
// F09B, Henry Zhao                                                   //
// 21/10/2017                                                         //
// COMP2521 assignment 2, Part 1 - B                                  //
//                                                                    //
// Inverted index                                                     //
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "inverted.h"

#define BUFF 1000

typedef struct wordList {
   WordNode first;
   WordNode last;
   int numWords;
} wordList;

typedef struct wordNode {
   char *word;
   UrlList list;
   WordNode next;
   WordNode prev;
} wordNode;

typedef struct urlList {
   UrlNode first;
   UrlNode last;
   int numUrls;
} urlList;

typedef struct urlNode {
   char *word;
   UrlNode next;
} urlNode;

int main(int argc, char *argv[]) {

   WordList words = newWordList();

   // load url files to open into array
   FILE *fp = fopen("collection.txt", "r");
   fscanf(fp,"\n");

   int size;
   int array[BUFF];
   for (size = 0; fscanf(fp,"url%d " , &array[size]) != EOF; size++);
   fclose(fp);

   // for each url file
   int i;
   for (i = 0; i < size; i++) {

      // load current filename / url
      char filename[BUFF];
      char url[BUFF];
      sprintf(url, "url%d", array[i]);
      sprintf(filename, "url%d.txt", array[i]);

      // opencurrent file
      FILE *f2 = fopen(filename, "r");

      // get to section 2 by scanning until "Section-2" is found
      char tmpString[BUFF];
      fscanf(f2, "%s", tmpString);
      while (strcmp(tmpString, "Section-2") != 0) {
         fscanf(f2, "%s", tmpString);
      }
      fscanf(f2, "%s", tmpString);
     
      // while reading words from section 2 add them to the list
      // in alphabetical order and add the current url to the url
      // list of the current word
      WordNode tmpWordNode;
      UrlNode newUrl;
      // while still in section 2
      while (strcmp(tmpString, "#end") != 0) {
         normalise(tmpString);
         newUrl = newUrlNode(url);
         tmpWordNode = findWord(words, tmpString);
         // if word is not in words list
         if (tmpWordNode == NULL) {
            // add word to list of words since it is not already in the list
            tmpWordNode = newWordNode(tmpString);
            addWordAlphabetically(words, tmpWordNode);
         }
         // add current url to word in the list
         appendUrl(tmpWordNode->list, newUrl);
         // get next word
         fscanf(f2, "%s", tmpString);
      }
      fclose(f2);
   }

   // print index to a file
   FILE *invertedIndexFile;
   invertedIndexFile = fopen("invertedIndex.txt", "w");
   assert(invertedIndexFile != NULL);

   WordNode q;
   UrlNode r;
   // for every word in words
   for (q = words->first; q != NULL; q = q->next) {
      fprintf(invertedIndexFile, "%s", q->word);
      // for every url in current word
      for (r = q->list->first; r != NULL; r = r->next) {
         fprintf(invertedIndexFile, " %s", r->word);
      }
      fprintf(invertedIndexFile, "\n");
   }
   fclose(invertedIndexFile);

   freeWordList(words);

   return EXIT_SUCCESS;
}

// Make lowercase and remove specified
// punctuation if they are at the end of a word
void normalise(char *str) {
   int i;
   for (i = 0; i < strlen(str); i++) {
      // uppercase to lowercase
      if (str[i] >= 'A' && str[i] <= 'Z') {
         str[i] = str[i] + ('a' - 'A');
      }
   }
   // checking for specified punctuation at the end of a word
   i--;
   if (str[i] == '.' || str[i] == ',' || str[i] == ';' || str[i] == '?') {
      str[i] = '\0';
   }
   return;
}

// return pointer to the node of the given word in the given list
WordNode findWord(WordList l, char *word) {
   assert(l != NULL);
   // return NULL if list is empty
   if (l->first == NULL) {
      return NULL;
   }
   WordNode tmp;
   for (tmp = l->first; tmp != NULL; tmp = tmp->next) {
      // return pointer to the node contatining the specified word
      if (strcmp(tmp->word, word) == 0) {
         return tmp;
      }
   }
   return NULL;
}

// add url node to the end of the list
void appendUrl(UrlList l, UrlNode node) {
   assert (l != NULL);
   if (l->first == NULL) {
      l->first = node;
      l->last = node;
   } else {
      l->last->next = node;
      l->last = node;
   }
   l->numUrls++;
}

// add node to the list in alphabetical order
void addWordAlphabetically(WordList l, WordNode node) {
   assert (l != NULL);
   // Empty case
   if (l->first == NULL) {
      l->first = node;
      l->last = node;
      l->numWords++;
      return;
   }
   // Insert at start if the first word is alphabetically after node
   if (strcmp(l->first->word, node->word) > 0) {
      l->first->prev = node;
      node->next = l->first;
      l->first = node;
      l->numWords++;
      return;
   }
   // Insert in middle
   WordNode curr;
   for (curr = l->first; curr != NULL; curr = curr->next) {
      // if current is alphabetically after node insert before
      if  (strcmp(curr->word, node->word) > 0) { 
         curr->prev->next = node;
         node->prev = curr->prev;
         node->next = curr;
         curr->prev = node;
         l->numWords++;
         return;
      }   
   }
   // If no node in the list is alphabetically after node - append
   l->last->next = node;
   node->prev = l->last;
   l->last = node;
   l->numWords++;
}

UrlNode newUrlNode(char *url) {
   UrlNode new = malloc(sizeof(urlNode));
   new->word = calloc(sizeof(char), strlen(url) + 1);
   strcpy(new->word, url);
   new->next = NULL;
   return new;
}

UrlList newUrlList() {
   UrlList new = malloc(sizeof(urlList));
   new->first = NULL;
   new->last = NULL;
   new->numUrls = 0;
   return new;
}

WordList newWordList() {
   WordList new = malloc(sizeof(wordList));
   new->first = NULL;
   new->last = NULL;
   new->numWords = 0;
   return new;
}

WordNode newWordNode(char *word) {
   WordNode new = malloc(sizeof(wordNode));
   new->word = calloc(sizeof(char), strlen(word) + 1);
   strcpy(new->word, word);
   new->list = newUrlList();
   new->next = NULL;
   new->prev = NULL;
   return new;
}

// frees entire word list including the url lists within
void freeWordList(WordList l) {
   WordNode currWord = l->first;
   WordNode nextWord;
   UrlNode currUrl;
   UrlNode nextUrl;
   while (currWord != NULL) {
      currUrl = currWord->list->first;
      while (currUrl != NULL) {
         nextUrl = currUrl->next;
         free(currUrl->word);
         free(currUrl);
         currUrl = nextUrl;
      }
      free(currWord->list);

      nextWord = currWord->next;
      free(currWord->word);
      free(currWord);
      currWord = nextWord;
   }
   free(l);
}
