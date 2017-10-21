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
typedef struct pageRankNode *PageRankNode;
typedef struct pageRankList *PageRankList;

typedef struct urlList {
   UrlNode first;
   UrlNode last;
   float numUrls;
} urlList;

typedef struct urlNode {
   char *word;
   UrlNode next;
   UrlNode prev;
} urlNode;

typedef struct pageRankNode {
   char *word;
   float W;
   int rank;
   PageRankNode next;
   PageRankNode prev;
} pageRankNode;

typedef struct pageRankList {
   PageRankNode first;
   PageRankNode last;
   int numNodes;
} pageRankList;

// function declarations
void appendUrl(UrlList l, UrlNode node);
UrlNode newUrlNode(char *url);
UrlList newUrlList();
void freeUrlList(UrlList l);
void addUrlAlphabetically(UrlList l, UrlNode node);
float scaledFootrule(UrlList list[], int numFiles, PageRankList ranked, UrlList urlUnion);
float absVal(float a);
PageRankNode newPageRankNode(char *url, int rank);
PageRankList newPageRankList();
void priorityInsert(PageRankList q, PageRankNode node);
void appendRank(PageRankList l, PageRankNode node);
int seen(PageRankList l, char *url, int rank);
void freePageRankList(PageRankList l);

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
   
   // print answer
   PageRankNode s;
   PageRankList rankedList = newPageRankList();
   float W = scaledFootrule(list, numFiles, rankedList, urlUnion);

   printf("%.6f\n", W);
   int rank;
   for (rank = 1; rank <= rankedList->numNodes; rank++) {
      for (s = rankedList->first; s != NULL; s = s->next) {
         if (s->rank == rank) {
            printf("%s\n", s->word);
            break;
         }
      }
   }

   // free lists
   freePageRankList(rankedList);
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

float scaledFootrule(UrlList list[], int numFiles, PageRankList ranked, UrlList urlUnion) {
   int i;
   float posInFile;
   float ans = 0;
   UrlNode tmp1;
   UrlNode tmp2;
   float rank;

   PageRankList q = newPageRankList();

   // for each spot in the url union
   for (rank = 1; rank <= urlUnion->numUrls; rank++) {
      // foreach url in union
      for (tmp1 = urlUnion->first; tmp1 != NULL; tmp1 = tmp1->next) {
         // find the url in each file
         PageRankNode tmpNode = newPageRankNode(tmp1->word, rank);
         for (i = 0; i < numFiles; i++) {
            posInFile = 1;
            for (tmp2 = list[i]->first; tmp2 != NULL; tmp2 = tmp2->next) {
               if (strcmp(tmp1->word, tmp2->word) == 0) {
                  //apply formula on url
                  tmpNode->W += absVal(posInFile/list[i]->numUrls - rank/urlUnion->numUrls);
               }
               posInFile++;
            }
         }
         priorityInsert(q, tmpNode);
         //printf("Url: %s Rank: %d W: %f\n", tmpNode->word, tmpNode->rank, tmpNode->W);
      }
   }

   PageRankNode tmp;
   for (tmp = q->first; tmp != NULL; tmp = tmp->next) {
      if (seen(ranked, tmp->word, tmp->rank) == 0) {
         PageRankNode tmpRanked = newPageRankNode(tmp->word, tmp->rank);
         tmpRanked->W = tmp->W;
         appendRank(ranked, tmpRanked);
         ans += tmp->W;
      }
   }
   freePageRankList(q);
   
   return ans;
}

PageRankNode newPageRankNode(char *url, int rank) {
   PageRankNode new = malloc(sizeof(pageRankNode));
   new->word = calloc(sizeof(char), strlen(url) + 1);
   strcpy(new->word, url);
   new->next = NULL;
   new->prev = NULL;
   new->rank = rank;
   new->W = 0;
   return new;
}

PageRankList newPageRankList() {
   PageRankList new = malloc(sizeof(pageRankList));
   new->first = NULL;
   new->last = NULL;
   new->numNodes = 0;
   return new;
}

float absVal(float a) {
   if (a >= 0) return a;
   return -a;
}

void priorityInsert(PageRankList q, PageRankNode node) {
   assert (q != NULL);
   // Empty case
   if (q->first == NULL) {
      q->first = node;
      q->last = node;
      q->numNodes++;
      return;
   }
   // Insert at start if the first W is bigger than node
   if (q->first->W > node->W) {
      q->first->prev = node;
      node->next = q->first;
      q->first = node;
      q->numNodes++;
      return;
   }
   // Insert in middle
   PageRankNode curr;
   for (curr = q->first; curr != NULL; curr = curr->next) {
      // if current is bigger than node insert before
      if (curr->W > node->W) {
         curr->prev->next = node;
         node->prev = curr->prev;
         node->next = curr;
         curr->prev = node;
         q->numNodes++;
         return;
      }
   }
   // If no node in the list is bigger than node - append
   q->last->next = node;
   node->prev = q->last;
   q->last = node;
   q->numNodes++;
}

void appendRank(PageRankList l, PageRankNode node) {
   assert(l != NULL);
   if (l->first == NULL) {
      l->first = l->last = node;
      l->numNodes++;
      return;
   }
   l->last->next = node;
   node->prev = l->last;
   l->last = node;
   l->numNodes++;
}

int seen(PageRankList l, char *url, int rank) {
   assert(l != NULL);
   PageRankNode tmp;
   for (tmp = l->first; tmp != NULL; tmp = tmp->next) {
      if (strcmp(url, tmp->word) == 0 || rank == tmp->rank) {
         return 1;
      }
   }
   return 0;
}

void freePageRankList(PageRankList l) {
   assert(l != NULL);
   PageRankNode curr = l->first;
   PageRankNode next;
   while (curr != NULL) {
      next = curr->next;
      free(curr->word);
      free(curr);
      curr = next;
   }
   free(l);
}
