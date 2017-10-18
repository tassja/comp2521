#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "queue.h"
#include "set.h"

int main(int argc, char *argv[]) {     

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
         printf("%s\n", str1); 
         fscanf(f2, "%s", str1);
      }





      fclose(f2);
      j++;
   }





   return EXIT_SUCCESS;
}
