#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

struct url{
    char name[50];
    float pRank;
};

double termf(char *term, char *url) {
    int i = 0;
    int j = 0;
    char *lines[1000];
    int count = 0;
    int tf = 0;
    int numTerms = 0;
    FILE *file_handle = fopen (url, "r");

    for (i = 0; i < 1000; ++i) {
        lines[i] = malloc(128); //Allocate max length of word
        int testing = fscanf(file_handle, "%127s", lines[i]); //scan all words from each line of txt file
        if( testing == 1) count++;
        //printf("%d\n", testing);

    }

    for(i = 0; i < count; ++i){
        if(!strcmp("Section-2", lines[i])){ //All words in section two
            ++i;
            break;
        }
    }

    for(; i < count; ++i){
        for(j = 0; lines[i][j] > 20; j++){ lines[i][j] = tolower(lines[i][j]); } //Convert all letters to lower case
        for(j = 0; j < strlen(lines[i]); ++j){
            if(ispunct(lines[i][j])){ lines[i][j] = 0; } //Remove all punctuation
        }
        //printf("%s %s\n", term, lines[i]);
        if(!strcmp(term, lines[i])){ //Compare the term with each word
            ++tf;
        }
        ++numTerms;
        //printf ("%d: %s\n", i, lines[i]);
        if(!strcmp(lines[i], "#end")) break;
    }

    //printf("%s: %d/%d\n", term, tf, numTerms);
    for(i = 0; i < 1000; ++i){
        free (lines[i]); //Free all lines after finished
    }
    //printf("%lf\n", tf*1.0/numTerms*1.0);
    double ratio = tf*1.0/numTerms*1.0;
    return ratio;
}



double getIdf(char *url, char *term){
    return 999;
}

int main (int argc, char *argv[]){
    FILE * fp;
    //FILE * fp2;
    int i = 0;
    int j = 0;
    //printf("%s\n", argv[1]);
    struct url Urls[100];
    for(i = 0; i < 100; ++i){
        Urls[i].pRank = -1.0;
    }
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("invertedIndex.txt", "r");
    if (fp == NULL)
        fprintf(stderr, "No invertedIndex.txt found\n");

        //fp2 = fopen("pagerankList.txt", "r");
        //if (fp == NULL)
        //    fprintf(stderr, "pagerankList.txt\n");

    //For each URL, find pRank for each URL
    while ((read = getline(&line, &len, fp)) != -1) {
        char *splitLine = strtok(line, " \n");
        for(i = 0; i < argc; ++i){
            if(splitLine != NULL && !strcmp(argv[i], splitLine)){
                splitLine = strtok(NULL," \n"); //Get next URL
                while(splitLine != NULL){
                    for(j = 0; j < 100; ++j){
                        if(Urls[j].pRank == -1.0){
                            strcpy(Urls[j].name, splitLine);
                            Urls[j].pRank = getIdf(splitLine, argv[i]);
                            break;break;
                        } else if (!strcmp(Urls[j].name, splitLine)){
                            Urls[j].pRank += getIdf(splitLine, argv[i]);
                            //printf("current: %s, %lf\n", Urls[j].name, Urls[j].pRank);
                            break;
                        }
                    }
                    splitLine = strtok(NULL," \n"); //Get next URL
                }
            }
        }
    }

    //Sort each URL by pRank (Bubble sort in DESC order)
    for (i = 0 ; i < 100 && Urls[i].pRank > 0; ++i) {
        for (j = 0 ; j < 100 - i - 1 && Urls[j].pRank > 0; ++j) {
            if (Urls[j].pRank < Urls[j+1].pRank){
                struct url swap = Urls[j];
                Urls[j] = Urls[j+1];
                Urls[j+1] = swap;
            }
        }
    }

    //Print the top 30 URLS
    for(i = 0; i < 30 && Urls[i].pRank > 0; i++){
        printf("%s\n", Urls[i].name);
    }
    fclose(fp);
    return 0;
}
