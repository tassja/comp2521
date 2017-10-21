////////////////////////////////////////////////////////////////////////
// nameofgroup - Tom Day, Julian Jankowski, Tassja Kriek              //
// F09B, Henry Zhao                                                   //
// 21/10/2017                                                         //
// COMP2521 assignment 2, Part 1 - A                                  //
//                                                                    //
// Calculate Weighted PageRanks                                                    //
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "queue.h"
#include "set.h"
#include "readData.h"


typedef struct Ranks{
    char *url;
    double pRank;
    double prevpRank;
    int outlinks;
    int inlinks;
} Ranks;

typedef struct RankCollection{
    Ranks *data;
    int nurls;
} RankCollection;

typedef unsigned char Num;

typedef struct GraphRep {
	int   nV;
	int   maxV;
	char  **vertex;
	Num   **edges;
} GraphRep;

RankCollection urldata;
void calculatePageRank(Graph,RankCollection,float,float,int);
Ranks *newRank();
void outputRanks(RankCollection);
double win(int, int, RankCollection,Graph);
double wout(int, int, RankCollection,Graph);

int main(int argc, char **argv) {
    int i = 0; //iterator

    if(argc != 4) {
        fprintf(stderr, "Usage: %s d diffPR maxIterations\n", argv[0]);
        return 1; //unsuccesful
    }

    //extract information from cmd line
    float d = atof(argv[1]);
    float diffPR = atof(argv[2]);
    float maxIteration = atof(argv[3]);

    //get set from readdata.c and graph, seperate functions??
    Graph urlgraph = newGraph(999);
    urlgraph = creategraph();
    RankCollection urldata;
    urldata.nurls = nVertices(urlgraph);
    urldata.data = malloc(sizeof(Ranks)*urldata.nurls);
    while (i < urldata.nurls) { //populate url collection with the entered urls
        //urldata.data[i].url = findurlstring(urldata, i)
        urldata.data[i].pRank = 0;
        urldata.data[i].prevpRank = 0;
        urldata.data[i].outlinks = 0;
        urldata.data[i].inlinks = 0;
        i++;
    }
    int j;
    //calculate outlinks for each url
    for (i = 0; i < nVertices(urlgraph); i++) {
        for (j = 0; j < nVertices(urlgraph); j++) {
            if (urlgraph->edges[i][j] == 1) {
                urldata.data[i].outlinks++;
            }
        }
        //printf("outlinks:%d\n", urldata.data[i].outlinks);
    }

    //calculate inlinks for each url
    for (j = 0; j < nVertices(urlgraph); j++) {
        for (i = 0; i < nVertices(urlgraph); i++) {
            if (urlgraph->edges[i][j] == 1) {
                urldata.data[j].inlinks++;
            }
        }
        printf("inlinks:%d\n", urldata.data[j].inlinks);
    }


    calculatePageRank(urlgraph, urldata, d, diffPR, maxIteration);
    //to do: writeRanksFile(seturls);
    return 0;
}


void calculatePageRank(Graph urlgraph, RankCollection urldata, float d, float diffPR, int maxIteration) {
    int N = nVertices(urlgraph);
    //float *prevRanks = malloc(N*sizeof(int));
    int c;
    for (c = 0; c < N; c++) {
        urldata.data[c].pRank = 1/(float)N;
        urldata.data[c].prevpRank = 1/(float)N;

        //prevRanks[c] = 1/(float)N;     //for calculating diff
    }

    int iteration = 0;
    float diff = diffPR;
    float sum = 0;
    while ((iteration < maxIteration) && diff >= (diffPR)) {
        int k,w;
            for (k = 0; k < N; k++) {
                urldata.data[k].prevpRank = urldata.data[k].pRank;
                for (w = 0; w < N; w++) {
                    if (urlgraph->edges[k][w]) {
                        sum += urldata.data[w].pRank*(win(k,w,urldata,urlgraph))*(wout(k,w,urldata,urlgraph));
                    }
                }
                urldata.data[k].pRank = ((1-d)/N)+(d*sum);
                diff += fabs((urldata.data[k].pRank) = ((((1-d)/N)+(d*sum))-(urldata.data[k].prevpRank)));
                printf("%lf\n", diff);
            }
        iteration++;
    }
    return;
}


double win(int x, int y, RankCollection urldata,Graph g){
    int N = nVertices(g);
    int temp = 0;
    for (y = 0; y < N; y++) {
        if (g->edges[x][y]) {
            //printf("y is: %d\n", y);
            //printf("inlinks are: %d\n", urldata.data[i].inlinks);
            temp += urldata.data[y].inlinks;
            //printf("temp is:%d\n", temp);
        }
        //printf("%d\n", temp);

    }
    double total = ((urldata.data[x].inlinks)/temp);
    printf("win total: %lf\n", total);
    return total;
}

double wout(int x, int y, RankCollection urldata, Graph g){
    int N = nVertices(g);
    int temp = 0;
    for (y = 0; y < N; y++) {
        if (g->edges[x][y]) {
            if (!urldata.data[y].outlinks) { //==0
                temp += 0.5;
            }else{
                temp += urldata.data[y].outlinks;
            }
        }
        //printf("%d\n", temp);
    }
    double total = ((urldata.data[x].outlinks)/temp);
    printf("wout total: %lf\n", total);
    return total;
}
