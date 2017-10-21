#include <stdio.h>
#include <stdlib.h>

void permutate(int permutation[], int length) {

   int i;
   int *array = calloc(length, sizeof(int));
   for (i = 0; i < length; i++) {
      array[i] = 0;
      printf("%d ", permutation[i]);
   }
   printf("\n");
   //output

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
         for (j = 0; j < length; j++) {
            printf("%d ", permutation[j]);
         }
         printf("\n");
         //output
         array[i]++;
         i = 0;
      } else {
         array[i] = 0;
         i++;
      }
   }
}

int main() {

   int array[5] = {8, 4, 2, 7};

   permutate(array, 4);
   
   return EXIT_SUCCESS;

}



