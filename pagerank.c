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
    int outlinks;
    int inlinks;
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
        urldata->data[i]->outlinks = 0;
        urldata->data[i]->inlinks = 0;
        i++;
    }

    calculatePageRank(urlgraph, seturls, d, diffPR, maxIteration);
    writeRanksFile(seturls);

    //free up memroy

    return 0;
}
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

void calculatePageRank(WebGraph urlgraph, RankRep urldata, float d, float diffPR, int maxIteration) {
    int N = nVertices(urlgraph);
    float *prevRanks = malloc(N*sizeof(int));
    int c;
    for (c = 0; c < N; c++) {
        urldata->data[c]->pRank = 1/(float)N;
        prevRanks[c] = 1/(float)N;     //for calculating diff
    }

    int iteration = 0;
    float diff = diffPR
    float sum = 0;

    while ((iteration < maxIteration) && diff >= (diffPR)) {
        int p,k;
        for (p = 0; p < N; p++) {
            prevRanks[p] = urldata->data[c]->pRank;
            for (k = 0; i < count; i++) {
                if (urlgraph->edges[p][k]) {
                    sum1 += (urldata->data[k]->inlinks)/((urldata->data[k]->inlinks) + (urldata->data[p]->inlinks));
                }
            }
        }







        iteration++;
    }

}




int connections(WebGraph g, char *start, char *end){

    assert(g != NULL);

 	int v = vertexID(start,g->vertex,g->nV);
 	int w = vertexID(end,g->vertex,g->nV);
 	if (v < 0 || w < 0)
 		return 0;
 	else
 		return g->edges[v][w];
   }

   return count;
   return 0;  // to keep the compiler happy
}
