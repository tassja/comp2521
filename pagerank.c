#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"


int main(int argc, char const *argv[]) {

     FILE *fp = fopen("Sample1/collection.txt", "r");


     int array[99];
     int i = 0;

        while (fscanf(fp,"url%d " , &array[i]) != EOF) {
            printf("%d\n", array[i]);
            i++;
        }
        fclose(fp);

    return 0;
}
