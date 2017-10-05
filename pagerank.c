#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"


int main(int argc, char const *argv[]) {

     FILE *fp = fopen("Sample1/collection.txt", "r");

     int array[99];
    // int array2[99];
     int i = 0;

        fscanf(fp,"\n");
        while (fscanf(fp,"url%d " , &array[i]) != EOF) {

            //printf("%d\n", array[i]);
            i++;
        }
        fclose(fp);
        int size = i-1;
        int j = 0;
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

            char c;
            char arr3[999];
              FILE *f2 = fopen(strC, "r"); //open speicific array

            c = getc(f2);
            c = getc(f2);

            while (c != '#' && c != EOF) {
                if (c == 'l') {
                    c = getc(f2);
                    int u = 0;
                    while (c != ' ') {
                        arr3[u] = c;
                        u++;
                        c = getc(f2);
                        arr3[u] = '\n';
                    }
                    printf("%s", arr3);
                }
                c = getc(f2);
            }


            fclose(f2);
            j++;
        }

    return 0;
}
