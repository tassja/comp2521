#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

struct url{
    char name[50];
    double tfidf;
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
    int i = 0;
    char *lines[1000];
    int count = 0;
    int idf = 0;
    char dir[100] = "";
    char curUrl[100] = "";
    strcpy(curUrl, url);
    double tf = 0.0;
    double tfidf = 0.0;
    FILE *file_handle = fopen ("collection.txt", "r");

    for (i = 0; i < 1000; ++i) {
        lines[i] = malloc(128);
        int testing = fscanf(file_handle, "%127s", lines[i]);
        if( testing == 1) count++;
        //printf("%d\n", testing);
        
    }
    
    for(i = 0; i < count; ++i){
        strcat(dir, lines[i]);
        strcat(dir, ".txt");
        //printf("%s\n", dir);
        if(termf(term, dir) > 0){ //Perform tf
            ++idf;
            //printf("%d\n", idf);
        }
        strcpy(dir, "");
        if(!strcmp(lines[i], "")) break;
    }
    fflush(stdout);
    for(i = 0; i < 1000; ++i){
        free (lines[i]); /* remember to deallocated the memory allocated */
    }
    strcat(curUrl, ".txt");
    tf = termf(term, curUrl);
    double ln = roundf(log(count*1.0/idf*1.0)*1000000)/1000000; //Round to 6 decimal places
    //printf("ln(%d/%d) = %lf\n", count, idf, ln);
    
    tfidf = tf*1.0*ln; //perform calculations
    return tfidf;
}

int main (int argc, char *argv[]){
    FILE * fp;
    int i = 0;
    int j = 0;
    //printf("%s\n", argv[1]);
    struct url Urls[100];
    for(i = 0; i < 100; ++i){
        Urls[i].tfidf = -1.0;
    }
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("invertedIndex.txt", "r");
    if (fp == NULL)
        fprintf(stderr, "No invertedIndex.txt found\n");
    
    //For each URL, perform TFIDF for each URL FOR EACH TERM
    while ((read = getline(&line, &len, fp)) != -1) {
        char *splitLine = strtok(line, " \n");
        for(i = 0; i < argc; ++i){
            if(splitLine != NULL && !strcmp(argv[i], splitLine)){
                splitLine = strtok(NULL," \n"); //Get next URL
                while(splitLine != NULL){
                    for(j = 0; j < 100; ++j){
                        if(Urls[j].tfidf == -1.0){
                            strcpy(Urls[j].name, splitLine);
                            Urls[j].tfidf = getIdf(splitLine, argv[i]);
                            break;break;
                        } else if (!strcmp(Urls[j].name, splitLine)){
                            Urls[j].tfidf += getIdf(splitLine, argv[i]);
                            //printf("current: %s, %lf\n", Urls[j].name, Urls[j].tfidf);
                            break;
                        }
                    }
                    splitLine = strtok(NULL," \n"); //Get next URL
                }
            }
        }
    }
    
    //Sort each URL by TFIDF (Bubble sort in DESC order)
    for (i = 0 ; i < 100 && Urls[i].tfidf > 0; ++i) {
        for (j = 0 ; j < 100 - i - 1 && Urls[j].tfidf > 0; ++j) {
            if (Urls[j].tfidf < Urls[j+1].tfidf){
                struct url swap = Urls[j];
                Urls[j] = Urls[j+1];
                Urls[j+1] = swap;
            }
        }
    }
    
    //Print the top 30 URLS
    for(i = 0; i < 30 && Urls[i].tfidf > 0; i++){
        printf("%s: %lf\n", Urls[i].name, Urls[i].tfidf);
    }
    fclose(fp);
    return 0;
}
