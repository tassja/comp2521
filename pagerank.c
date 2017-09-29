#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"


int main(int argc, char const *argv[]) {

     FILE *fp = fopen("Sample1/collection.txt", "r");

     int array[99];
     int i = 0;

        fscanf(fp,"\n");
        while (fscanf(fp,"url%d " , &array[i]) != EOF) {

            printf("%d\n", array[i]);
            i++;
        }
        fclose(fp);
        int size = i;

        while (size >= 0) {
            fp = fopen("Sample1/url"array[size]".txt", "r");
            fscanf(fp,"#start Section-1");
            fscanf(fp,"\n");
            size--;
        }

    return 0;
}
