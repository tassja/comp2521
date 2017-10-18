#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUFF 1000

typedef struct urlNode *UrlNode;
typedef struct urlList *UrlList;

typedef struct urlList {
   UrlNode first;
   UrlNode last;
   int numUrls;
} urlList;

typedef struct urlNode {
   char *word;
   UrlNode next;
   UrlNode prev;
} urlNode;

void appendUrl(UrlList l, UrlNode node);
UrlNode newUrlNode(char *url);
UrlList newUrlList();
void freeUrlList(UrlList l);
void addUrlAlphabetically(UrlList l, UrlNode node);


int main(int argc, char *argv[]) {
   if (argc < 3) {
      printf("Too few arguments!\n");
      return EXIT_FAILURE;
   }
   int i;
   FILE *fp;
   int numFiles = argc - 1;
   char tmp[BUFF];
   UrlList list[numFiles];
   UrlList urlUnion = newUrlList();

   for (i = 0; i < numFiles; i++) {
      list[i] = newUrlList();
      fp = fopen(argv[i + 1], "r");
      while (fscanf(fp, "%s", tmp) == 1) {         
         appendUrl(list[i], newUrlNode(tmp));
         addUrlAlphabetically(urlUnion, newUrlNode(tmp));
      }
      fclose(fp);
   }
   //print lists
   UrlNode q;
   for (i = 0; i < numFiles; i++) {
      printf("list[%d]:\n", i);
      for (q = list[i]->first; q != NULL; q = q->next) {
         printf("%s\n", q->word);
      }
   }

   printf("urlUnion:\n");
   for (q = urlUnion->first; q != NULL; q = q->next) {
      printf("%s\n", q->word);
   }

   freeUrlList(urlUnion);
   for (i = 0; i < numFiles; i++) {
      freeUrlList(list[i]);
   }


   return EXIT_SUCCESS;
}

void appendUrl(UrlList l, UrlNode node) {
   assert (l != NULL);
   if (l->first == NULL) {
      l->first = node;
      l->last = node;
   } else {
      l->last->next = node;
      node->prev = l->last;
      l->last = node;
   }
   l->numUrls++;
}

UrlNode newUrlNode(char *url) {
   UrlNode new = malloc(sizeof(urlNode));
   new->word = calloc(sizeof(char), strlen(url) + 1);
   strcpy(new->word, url);
   new->next = NULL;
   new->prev = NULL;
   return new;
}

UrlList newUrlList() {
   UrlList new = malloc(sizeof(urlList));
   new->first = NULL;
   new->last = NULL;
   new->numUrls = 0;
   return new;
}

void freeUrlList(UrlList l) {
   assert(l != NULL);
   UrlNode curr = l->first;
   UrlNode next;
   while (curr != NULL) {
      next = curr->next;
      free(curr->word);
      free(curr);
      curr = next;
   }
   free(l);
}

void addUrlAlphabetically(UrlList l, UrlNode node) {
   assert (l != NULL);
   // Empty case
   if (l->first == NULL) {
      l->first = node;
      l->last = node;
      l->numUrls++;
      return;
   }
   // Insert at start if the first word is alphabetically after node
   if (strcmp(l->first->word, node->word) > 0) {
      l->first->prev = node;
      node->next = l->first;
      l->first = node;
      l->numUrls++;
      return;
   }
   // Insert in middle
   UrlNode curr;
   for (curr = l->first; curr != NULL; curr = curr->next) {
      // if current is alphabetically after node insert before
      if  (strcmp(curr->word, node->word) > 0) { 
         curr->prev->next = node;
         node->prev = curr->prev;
         node->next = curr;
         curr->prev = node;
         l->numUrls++;
         return;
      }
      // don't insert duplicates
      if (strcmp(curr->word, node->word) == 0) {
         return;
      }
   }
   // If no node in the list is alphabetically after node - append
   l->last->next = node;
   node->prev = l->last;
   l->last = node;
   l->numUrls++;
}
