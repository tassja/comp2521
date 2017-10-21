#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"

Graph creategraph(char *collection){

    Graph webGraph = newGraph(999);
    FILE *fp = fopen("collection.txt", "r"); // give "collection.txt"

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
            char * strA = "url.txt", strC[50];
            int x = 3;
            strncpy(strC,strA,x);
            strC[x] = '\0';
            strcat(strC,num);
            strcat(strC,strA+x);
            char c;

            FILE *f2 = fopen(strC, "r"); //open specified array
            c = getc(f2);
            c = getc(f2);

            while (c != '#' && c != EOF) {
                if (c == 'l') {
                    c = getc(f2);
                    int u = 0;
                    while (c != ' ' && c != '#' && c != '\n') {
                        arr3[k][u] = c;
                        u++;
                        c = getc(f2);
                        arr3[k][u] = '\0';
                    }

                    if (strcmp(arr3[k],num) != 0) {//check for self loops
                        addEdge(webGraph,num,arr3[k]);
                        //printf("inserting edge between %s and %s\n", num, arr3[k]);
                        }
                 }
                 c = getc(f2);
            }
            k++;
            fclose(f2);
            j++;
        }
        //showGraph(webGraph, 1);
        //showGraph(webGraph, 0);
return webGraph;
}
