#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "queue.h"
#include "set.h"

typedef struct Ranks{
    char *url;
    double pRank;
    double prevpRank;
    int outlinks;
    int inlinks;
    char *outies;
    char *innies;
} Ranks;

typedef struct RankCollection{
    Ranks *all
    int nurls;
} RankCollection

void calculatePageRank(WebGraph, RankCollection, float, float, int);
void outputRanks(RankCollection);

int main(int argc, char **argv) {
    int i; //iterator

    if(argc != 4) {
        fprintf(stderr, "Usage: %s d diffPR maxIterations\n", argv[0]);
        return 1; //unsuccesful
    }

    //extract information from cmd line
    float d = atof(argv[1]);
    float diffPR = atof(argv[2]);
    float maxIteration = atof(argv[3]);

    // Read "web pages" from the collection in file "collection.txt"
    //       and build a graph structure using Adjacency List Representation
    //
    //       N = number of urls in the collection
    //
    //       For each url pi in the collection
    //
    //       End For
    //
    //       iteration = 0;
    //       diff = diffPR;   // to enter the following loop
    //
    //       While (iteration < maxIteration AND diff >= diffPR)
    //           iteration++;
    //    End While

    //get set from readdata.c and graph, seperate functions??

    RankCollection urldata;
    urldata->nurls = nElems(urls);
    urldata->all = malloc(sizeof(Ranks)*urldata->nurls);
    while (i < urldata->nurls) { //populate url collection with the entered urls
        urldata->data[i]->url = findurlstring(urldata, i)
        urldata->data[i]->pRank = 0;
        urldata->data[i]->prevpRank = 0;
        urldata->data[i]->outlinks = 0;
        urldata->data[i]->inlinks = 0;
        i++;
    }

    calculatePageRank(urlgraph, seturls, d, diffPR, maxIteration);
    //to do: writeRanksFile(seturls);

    //free up memroy


    //calculate outlinks for each url
    for (i = 0; i < urlgraph->nV; i++) {
        for (j = 0; j < urlgraph->nV; j++) {
            if (urlgraph->edges[i][j]) {
                urldata->data[i]->outlinks++;
            }
        }
    }

    //calculate inlinks for each url
    for (j = 0; j < urlgraph->nV; j++) {
        for (i = 0; i < urlgraph->nV; i++) {
            if (urlgraph->edges[i][j]) {
                urldata->data[i]->inlinks++;
            }
        }
    }

    return 0;
}


void calculatePageRank(WebGraph urlgraph, RankRep urldata, float d, float diffPR, int maxIteration) {
    int N = nVertices(urlgraph);
    float *prevRanks = malloc(N*sizeof(int));
    int c;
    for (c = 0; c < N; c++) {
        urldata->data[c]->pRank = 1/(float)N;
        urldata->data[c]->pprevRank = 1/(float)N;
        prevRanks[c] = 1/(float)N;     //for calculating diff
    }

    int iteration = 0;
    float diff = diffPR
    float sum = 0;

    while ((iteration < maxIteration) && diff >= (diffPR)) {
        int k,w;
            for (k = 0; i < N; k++) {
                urldata->data[k]->prevpRank = urldata->data[k]->pRank;
                for (w = 0; w < count; w++) {
                    if (urlgraph->edges[k][w]) {
                        sum += urldata->data[w]->pRank*win(k,w,urldata)*wout(k,w,urldata);
                    }
                }
                urldata->data[k]->pRank = ()(1-d)/N)+(d*sum);
                diff= abs((urldata->data[k]->pRank = ()(1-d)/N)+(d*sum))-(urldata->data[k]->prevpRank);
            }
        iteration++;
    }
}


double win(int x, int y, RankRep urllists, WebGraph urlgraph){
    int N = nVertices(urlgraph);
    int temp = 0;
    for (y = 0; y < N; y++) {
        if (array[x][y]) {
            temp += urldata->data[y]->outlinks; //not right
        }
    }

    double total = ((urldata->data[x]->inlinks)/temp);

    return total;
}
