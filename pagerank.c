////////////////////////////////////////////////////////////////////////
// nameofgroup - Tom Day, Julian Jankowski, Tassja Kriek              //
// F09B, Henry Zhao                                                   //
// 21/10/2017                                                         //
// COMP2521 assignment 2, Part 1 - A                                  //
//                                                                    //
// Calculate Weighted PageRanks                                       //
// graph.c ADT courtesy of John Shepherd, September 2015              //
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "readData.h"

typedef struct Ranks{
    char *url;
    float pRank;
    float prevpRank;
    float outlinks;
    float inlinks;
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

//function prototypes
void sort(RankCollection,int);
void calculatePageRank(Graph,RankCollection,float,float,int,int,int inlinks[99][99]);
void exportPageRank(Graph,RankCollection,int);
double win(int, int, RankCollection,Graph);
double wout(int, int, RankCollection,Graph);

int main(int argc, char **argv) {
    int i = 0; //iterator

    if(argc != 4) {
        fprintf(stderr, "Usage: %s d diffPR maxIterations\n", argv[0]);
        return 1; //do not pass go do not collect $200
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
        //urldata.data[i].url = findurlstring(urldata, i);
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
        //printf("outlinks:%lf\n", urldata.data[i].outlinks);
    }

    //calculate inlinks for each url
    int N = nVertices(urlgraph);
    //printf("N is: %d\n", N);
    int inlinks[N][N];
    for (j = 0; j < N; j++) {
        //int b = 0;
        for (i = 0; i < N; i++) {
            if (urlgraph->edges[i][j] == 1) {
                urldata.data[j].inlinks++;
                inlinks[j][i] = 1;
                //printf("j: [%d] i: [%d] b: [%d]\n",j,i,b);

                //b++;
            }else{
                inlinks[j][i] = 0;
                //b++;
            }
            //printf("inlinks[%d][%d]:%d\n", j,i,inlinks[j][i]);
        }
        //printf("inlinks:%lf\n", urldata.data[j].inlinks);
    }

    calculatePageRank(urlgraph, urldata, d, diffPR, maxIteration,N,inlinks);
    //to do: writeRanksFile(seturls);
    exportPageRank(urlgraph,urldata,N);
    for (int c = 0; c < nVertices(urlgraph); c++) {
        //printf("url, %.0f, %.7f\n", urldata.data[c].outlinks,urldata.data[c].pRank);
    }
    return 0;
}

void calculatePageRank(Graph urlgraph, RankCollection urldata, float d, float diffPR, int maxIteration,int N, int kk[N][N]) {
    N = nVertices(urlgraph);
    int c,k,w;
    for (c = 0; c < N; c++) {
        urldata.data[c].pRank = 1/(float)N;
    }

    float prePR;
    int iteration = 0;
    float sum = 0;
    float diff = diffPR;

    while ((iteration < maxIteration) && diff >= (diffPR)) {
            diff = 0;
            for (k = 0; k < N; k++) {
                prePR = urldata.data[k].pRank;
                sum = 0;
                for (w = 0; w < N; w++) {
                    if (kk[k][w] == 1) {
                        //printf("win[%d][%d]: %lf wout[%d][%d]: %lf\n",k,w,win(k,w,urldata,urlgraph),k,w,wout(k,w,urldata,urlgraph) );
                        //printf("sum: %lf\n", sum);
                        sum += urldata.data[w].pRank*(win(k,w,urldata,urlgraph))*(wout(k,w,urldata,urlgraph));
                        //printf("sum: %lf\n", sum);
                        if (k == 1) {
                            //assert(1==2);
                        }
                    }
                }

                for (w = 0; w < N; w++) {
                    //printf("win[%d][%d]: %lf wout[%d][%d]: %lf\n",k,w,win(k,w,urldata,urlgraph),k,w,wout(k,w,urldata,urlgraph) );
                        //sum += urldata.data[w].pRank*(win(k,w,urldata,urlgraph))*(wout(k,w,urldata,urlgraph));

                }
                // printf("sum: %.7f\n", sum);
                // printf("prev prank: %.7f\n", prePR);
                // printf("prank: %f\n", urldata.data[k].pRank);
                 urldata.data[k].pRank = ((1 - d) / N) + (d * sum);
                 diff += fabs(urldata.data[k].pRank-prePR);
                // printf("diff:%lf\n", diff);
            }
        //diff = 0;
        iteration++;
    }
    return;
}

double win(int x, int y, RankCollection urldata,Graph g){
    int N = nVertices(g);
    float temp = 0;
    for (int b = 0; b < N; b++) {
        if (g->edges[y][b]) {
            temp = temp + urldata.data[b].inlinks;
            //printf("temp: %f\n", temp);
        }
        //printf("win temp: %f\n", temp);
    }
    float total = 0;
    //printf("x inlinks: %f\n", urldata.data[x].inlinks);
    total = ((urldata.data[x].inlinks)/(temp));
    //printf("%f\n", total);
    return total;
}

double wout(int x, int y, RankCollection urldata, Graph g){
    int N = nVertices(g);
    float temp = 0;
    for (int b = 0; b < N; b++) {
        if (g->edges[y][b]) {
            if (urldata.data[b].outlinks == 0) {
                temp = temp + 0.5;
            }else{
                temp = temp + urldata.data[b].outlinks;
            }
            //printf("wout temp: %f\n", temp);
        }
        //printf("%f\n", temp);
    }
    float total = 0;
    if (urldata.data[x].outlinks == 0) {
        urldata.data[x].outlinks = 0.5;
    }
    total = ((urldata.data[x].outlinks)/temp);
    return total;
}

void exportPageRank(Graph g, RankCollection urldata,int N){

    char src[50];
    src[0] = 'u';
    src[1] = 'r';
    src[2] = 'l';
    src[3] = '\0'; //lmao what is wrong with me

    for (int i = 0; i < g->nV; i++) {
        strcat(src, g->vertex[i]);
        urldata.data[i].url = strdup(src);
        src[3] = '\0';
    }

    //sort these puppies
    sort(urldata, N);

    FILE *fp;
    if((fp = fopen("pagerankList.txt", "w")) != NULL) {
        for (int i = 0; i < N; i++) {
            fprintf(fp, "%s, %.0f, %.7f\n", urldata.data[i].url, urldata.data[i].outlinks, urldata.data[i].pRank);
            }
        fclose(fp);
    }
}

void sort(RankCollection urldata, int n) {
    for(int i = 1; i < n; i++)
    {
        for(int j = 0; j < n-i; j++)
        {
            if(urldata.data[j].pRank < urldata.data[j+1].pRank)
            {
                Ranks temp = urldata.data[j];
                urldata.data[j] = urldata.data[j+1];
                urldata.data[j+1] = temp;
            }
        }
    }
}
