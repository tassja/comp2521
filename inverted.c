// Code appropriated from Week 10 lab

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "inverted.h"
#include "graph.h"
#include "queue.h"
#include "set.h"


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

void normalise(char *str);


int main(int argc, char *argv[]) {

   WordList words = newWordList();

   FILE *fp = fopen("Sample1/collection.txt", "r");

   fscanf(fp,"\n");

   int size;
   int array[99];
   for (size = 0; fscanf(fp,"url%d " , &array[size]) != EOF; ++size);

   fclose(fp);

   int j;
   for (j = 0; j < size; ++j) {

      char filename[50];
      char url[50];
      sprintf(url, "url%d", array[j]);
      sprintf(filename, "Sample1/url%d.txt", array[j]);
      printf("%s\n", filename);


      FILE *f2 = fopen(filename, "r"); //open specified array

      char str1[1000];
      fscanf(f2, "%s", str1);
      while (strcmp(str1, "Section-2") != 0) {
         fscanf(f2, "%s", str1);
      }
      fscanf(f2, "%s", str1);
     

      WordNode tmp1;
      while (strcmp(str1, "#end") != 0) {
         // Do Stuff here!!
         //str1 = the current word
         //strC = the currnet url
         normalise(str1);
         // add word to list of words (dont add duplicates)
         // add current url to word in the list
         UrlNode newUrl = newUrlNode(url);
         tmp1 = findWord(words, str1);
         if (tmp1 == NULL) {
            tmp1 = newWordNode(str1);
            addWordAlphabetically(words, tmp1);
         }
         appendUrl(tmp1->list, newUrl);

         // get next word
         fscanf(f2, "%s", str1);
      }
      fclose(f2);
   }

   WordNode q;
   UrlNode r;
   for (q = words->first; q != NULL; q = q->next) {
      printf("%s", q->word);
      for (r = q->list->first; r != NULL; r = r->next) {
         printf(" %s", r->word);
      }
      printf("\n");
   }

   freeWordList(words);

   return EXIT_SUCCESS;
}

void normalise(char *str) {
   int i;
   for (i = 0; i < strlen(str); i++) {
      if (str[i] >= 'A' && str[i] <= 'Z') {
         str[i] = str[i] + ('a' - 'A');
      } else if (str[i] < 'a' || str[i] > 'z') {
         str[i] = '\0';
         return;
      }
   }
   return;
}

WordNode findWord(WordList l, char *word) {
   assert(l != NULL);
   if (l->first == NULL) {
      return NULL;
   }
   WordNode tmp;
   for (tmp = l->first; tmp != NULL; tmp = tmp->next) {
      if (strcmp(tmp->word, word) == 0) {
         return tmp;
      }
   }
   return NULL;
}

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
