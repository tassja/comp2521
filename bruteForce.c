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

// typedefs
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

// function definitions
void appendUrl(UrlList l, UrlNode node);
UrlNode newUrlNode(char *url);
UrlList newUrlList();
void freeUrlList(UrlList l);
void addUrlAlphabetically(UrlList l, UrlNode node);
float scaledFootrule(UrlList list[], int numFiles, UrlNode permutation[], float numUls);
int factorial(int a);
float absVal(float a);
void permutate(UrlList list[], int numFiles, UrlNode permutation[], float length);


int main(int argc, char *argv[]) {
   // checkin that there is atleat one input list
   // will return W = 0 for a single list and the minimum
   // W otherwise
   if (argc < 2) {
      printf("Too few arguments!\n");
      return EXIT_FAILURE;
   }
   
   // adding going through all of the files and storing their
   //contents in an array of lists call list. The union of these files
   // is also stored in urlUnion in alphabetical order
   int i; 
   FILE *fp;
   char tmp[BUFF];
   int numFiles = argc - 1;
   UrlList list[numFiles];
   UrlList urlUnion = newUrlList();
   // for each file
   for (i = 0; i < numFiles; i++) {
      list[i] = newUrlList();
      fp = fopen(argv[i + 1], "r");
      // for every url listed in the file
      while (fscanf(fp, "%s", tmp) == 1) {         
         appendUrl(list[i], newUrlNode(tmp));
         addUrlAlphabetically(urlUnion, newUrlNode(tmp));
      }
      fclose(fp);
   }
   
   // loading urlUnion list int and array call urlUnionArray
   UrlNode *urlUnionArray = calloc(urlUnion->numUrls, sizeof(urlNode));
   int k;
   UrlNode tmpNode = urlUnion->first;
   for (k = 0; k < urlUnion->numUrls; k++) {
      urlUnionArray[k] = tmpNode;
      tmpNode = tmpNode->next;
   }

   // calculate W for every permutation and print the minumum
   permutate(list, numFiles, urlUnionArray, urlUnion->numUrls);

   // free lists
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

float scaledFootrule(UrlList list[], int numFiles, UrlNode permutation[], float numUrls) {

   int i;
   int j;
   float pos;
   float rank = 1; // start from the first rank (position) in the union
   float ans = 0; // initialise the answer sum to 0
   UrlNode unionCurr;
   UrlNode fileCurr;

   // foreach url in union
   for (i = 0; i < numUrls; i++) {
      unionCurr = permutation[i];
      // for each file
      for (j = 0; j < numFiles; j++) {
         pos = 1;
         // for each url in the file (find the url in the file)
         for (fileCurr = list[j]->first; fileCurr != NULL; fileCurr = fileCurr->next) {
            // checking if you have found the url
            if (strcmp(unionCurr->word, fileCurr->word) == 0) {
               //apply formula on url
               ans += absVal(pos/list[j]->numUrls - rank/numUrls);
            }
            pos++; // increment to the next position (rank) in the file
         }
      }
      rank++; // increment to the next rank (position) in the union
   }
   return ans;
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

void permutate(UrlList list[], int numFiles, UrlNode permutation[], float length) {

   int count = 0;
   float min;
   int minLoc;
   int numPerms = factorial(length); 

   int *array = calloc(length, sizeof(int));
   float *ans = calloc(numPerms, sizeof(int));
   UrlList *permList = calloc(numPerms, sizeof(urlList));

   int i;
   int j;
   UrlNode tmp;
   
   // setting array[length] = {0};
   for (i = 0; i < length; i++) array[i] = 0;
   
   // calculating scaledFootrule for initial (alphabetical) permutation
   // and setting that as the minimum that we have seen so far
   ans[count] = scaledFootrule(list, numFiles, permutation, length);
   min = ans[count];
   minLoc = count;
   permList[count] = newUrlList();
   // loading inital permutation into permlist[0]
   for (i = 0; i < length; i++) {
      appendUrl(permList[count], newUrlNode(permutation[i]->word));
   }
   count++;

   i = 0;
   while (i < length) {
      if (array[i] < i) {
         if (i % 2 == 0) {
            //swap permutation[0] and permutation[i]
            tmp = permutation[0];
            permutation[0] = permutation[i];
            permutation[i] = tmp;
         } else {
            // swap permutation[array[i]] and permutation[i])
            tmp = permutation[array[i]];
            permutation[array[i]] = permutation[i];
            permutation[i] = tmp;
         }
         // New permutation
         // calculate the scaledFootrule for this permutationand store
         // the answer in ans[count]. If this is the minimus also store
         // the permutation in permList[count]
         permList[count] = newUrlList();
         ans[count] = scaledFootrule(list, numFiles, permutation, length);
         if (ans[count] < min) {
            min = ans[count];
            minLoc = count;
            for (j = 0; j < length; j++) {
               appendUrl(permList[count], newUrlNode(permutation[j]->word));
            }
         }
         count++;
         array[i]++;
         i = 0;
      } else {
         array[i] = 0;
         i++;
      }
   }

   // print the minimum
   printf("%f\n", min);
   for (tmp = permList[minLoc]->first; tmp != NULL; tmp = tmp->next) {
      printf("%s\n", tmp->word);
   }

   // free lists
   for (i = 0; i < numPerms; i++) {
      freeUrlList(permList[i]);
   }
   free(ans);
   free(array);
}
