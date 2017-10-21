////////////////////////////////////////////////////////////////////////
// nameofgroup - Tom Day, Julian Jankowski, Tassja Kriek              //
// F09B, Henry Zhao                                                   //
// 21/10/2017                                                         //
// COMP2521 assignment 2, Part 3                                      //
//                                                                    //
// Hybrid/Meta Search Engine using Rank Aggregation                   //
////////////////////////////////////////////////////////////////////////

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
   float numUrls;
   float W;
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
void scaledFootrule(UrlList list[], int numFiles, UrlList permutation);
int factorial(int a);
float absVal(float a);
void permutate(UrlList list[], int numFiles, UrlList ranked, int permutation[], float length);

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
   /* 
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
   */ 
   
   // printing answer
   //UrlNode s;
   UrlList rankedList = newUrlList();
   float W = scaledFootrule(list, numFiles, rankedList, urlUnion);

   printf("%.6f\n", W);
   //for (s = rankedList->first; s != NULL; s = s->next) {
   //   printf("%s\n", s->word);
   //}


   int array[5] = {1, 2, 3, 4, 5};
   permutate(array, 5);

   // free lists
   //freeUrlList(rankedList);
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

void scaledFootrule(UrlList list[], int numFiles, UrlList permutation) {

   int i;
   float pos;
   float ans = 0;
   UrlNode tmp1;
   UrlNode tmp2;
   float rank;

   rank = 1;
   // foreach url in union
   for (tmp1 = permutation->first; tmp1 != NULL; tmp1 = tmp1->next) {
      // find the url in each file
      for (i = 0; i < numFiles; i++) {
         pos = 1;
         for (tmp2 = list[i]->first; tmp2 != NULL; tmp2 = tmp2->next) {
            if (strcmp(tmp1->word, tmp2->word) == 0) {
               //apply formula on url
               ans += absVal(pos/list[i]->numUrls - rank/permutation->numUrls);
            }
            pos++;
         }
      }
      rank++;
   }
   permutation->W = 0;
}

int factorial(int a) {
   int i;
   int ans = 1;
   for (i = 1; i <= a; i++) {
      ans = ans*i;
   }
   return ans;
}

float absVal(float a) {
   if (a >= 0) return a;
   return -a;
}

void permutate(UrlList list[], int numFiles, UrlList ranked, int permutation[], float length) {


   int count = 0;
   int numPerms = factorial(length); 

   UrlList array = calloc(numPerms, sizeof(urlList));
   int i;

   for (i = 0; i < length; i++) array[i] = 0;
   //output
   scaledFootrule(list, numFiles, ranked, array[count]);
   count++;

   int tmp;
   int j;

   i = 0;
   while (i < length) {
      if (array[i] < i) {
         if (i % 2 == 0) {
            //swap A[0] and A[i]
            tmp = permutation[0];
            permutation[0] = permutation[i];
            permutation[i] = tmp;
         } else {
            // swap A[array[i]] and A[i])
            tmp = permutation[array[i]];
            permutation[array[i]] = permutation[i];
            permutation[i] = tmp;
         }
         //output
         scaledFootrule(list, numFiles, array[count]);
         count++;
         array[i]++;
         i = 0;
      } else {
         array[i] = 0;
         i++;
      }
   }

   printf("TEST: %f\n", array[0]->W);

}
