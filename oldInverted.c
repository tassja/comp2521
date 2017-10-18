// Code appropriated from Week 10 lab

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "inverted.h"
#include "graph.h"
#include "queue.h"
#include "set.h"


typedef struct BSTNode {
   char *word;
   List urlList;
   BSTLink left, right;
} BSTNode;

typedef struct urlList {
   urlNode first;
   int numNodes;
}

typedef struct urlNode {
   char *word;
   urlNode next;
} urlNode;

void normalise(char *str);


int main(int argc, char *argv[]) {

   BSTree t = newBSTree();

   FILE *fp = fopen("Sample1/collection.txt", "r");

   int array[99];
   int i = 0;

   fscanf(fp,"\n");
   while (fscanf(fp,"url%d " , &array[i]) != EOF) {
      i++;
   }

   fclose(fp);

   int size = i-1;
   int j = 0;
   char arr3[999][999];
   int k = 0;


   while (j <= size) {
      char num[3];
      sprintf(num,"%d",array[j]);

      char * strA = "Sample1/url.txt", strC[50];
      int x = 11;
      strncpy(strC,strA,x); //goo.gl/rXuFz3
      strC[x] = '\0';
      strcat(strC,num);
      strcat(strC,strA+x);
      printf("%s\n",strC);


      FILE *f2 = fopen(strC, "r"); //open specified array

      char str1[1000];
      fscanf(f2, "%s", str1);
      while (strcmp(str1, "Section-2") != 0) {
         fscanf(f2, "%s", str1);
      }
      fscanf(f2, "%s", str1);
      
      while (strcmp(str1, "#end") != 0) {
         // Do Stuff here!! printf("%s\n", str1); 
         //t = BSTreeInsert(t, newBSTNode(str1));
         //str1 = the current word
         //strC = the currnet url
         normalise(str1);
         // add word to list of words (dont add duplicates)
         if (BSTreeFind(t, str1) != NULL) {
            t = BSTreeInsert(t, newBSTNode(str1);
         }
         // add current url to word in the list
         addUrlToList(t, strC);         


         printf("%s\n", str1);

         fscanf(f2, "%s", str1);
      }





      fclose(f2);
      j++;
   }





   return EXIT_SUCCESS;
}

BSTLink newBSTNode(char *v) {

   // making a BSTNode
   BSTLink new = malloc(sizeof(BSTNode));
   assert(new != NULL);
   new->word = v;
   new->left = new->right = NULL;
   new->urlList = NULL;
   return new;
}

ListNode newListNode(char *url) {
   // making a URLnode to start the url list
   ListNode new = malloc(sizeof(urlNode));
   assert(new != NULL);
   new->url = url;
   new->next = NULL;
   return new;
}

// create a new empty BSTree
BSTree newBSTree() {
   return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t) {
   if (t == NULL) return;

   ListNode curr = t->urlList->first;
   ListNode prev = NULL;
   while (curr != NULL) {
      prev = curr;
      curr = curr->next;
      free(prev);
   }
   free(t->urlList);
   dropBSTree(t->left);
   dropBSTree(t->right);
   free(t);
}

int BSTreeNumNodes(BSTree t) {
   if (t == NULL)
      return 0;
   else
      return 1 + BSTreeNumNodes(t->left)
         + BSTreeNumNodes(t->right);
}

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree t, char *v) {
   if (t == NULL)
      return newBSTNode(v);
   else if (strcmp(v, t->word) < 0)
      t->left = BSTreeInsert(t->left, v);
   else if (strcmp(v, t->word) > 0)
      t->right = BSTreeInsert(t->right, v);
   else // (v == t->word)
      // add url to t
   return t;
}

// return the node of the foundword or NULL if it was not found
ListNode BSTreeFind(BSTree t, char *v) {
   if (t == NULL)
      return NULL;
   else if (strcmp(v, t->word) < 0)
      return BSTreeFind(t->left, v);
   else if (strcmp(v, t->word) > 0)
      return BSTreeFind(t->right, v);
   else // (v == t->word)
      return t->urlList;
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

void addURL(BSTree t, char* url) {
   BSTLink = node;
   node = BSTreeFind(t, url);

   assert(node != NULL);
   if (node->word == NULL) {
      node->urlList = newUrlList();
      node->urlList->first = newUrlNode(url);
   } else {
      node->urlList = appendToUrlList(node->urlList, url);
   }
}

void appendToUrlList(List l, ListNode node) {
   assert (l != NULL);
   if (l->first == NULL) {
      l->first = node;
   } else {
      ListNode tmp;
      for (tmp = l->first; tmp->next != NULL; tmp = tmp->next) {
      }
      tmp->next = node;
   }
   l->numNodes++;
}



ListNode newUrlNode(char *url) {
   ListNode new = malloc(sizeof(ListNode));
   new->word = url;
   new->next = NULL;
   return new;
}

List newUrlList() {
   List new = malloc(sizeof(urlList));
   new->first = NULL;
   new->numNodes = 0;
   return new;
}
