#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mcm.h"

float min(float a, float b){
    return a < b ? a : b;
}

//Allocate needed space for graph
Edge* updateEdges(Edge* list, Edge* node){
    if(node == NULL){
        return list;
    }   
    if (list == NULL){
        return node;
    }
    node->next = list;
    return node;
}

Edge* createEdge(int d, float w){
    Edge* node = malloc(sizeof(*node));
    if(node == NULL){
        return NULL;
    }
    node->dest = d;
    node->weight = w;
    node->next = NULL;
    return node;
}

//Construct graph
struct Graph* buildGraph(FILE* fp, int* V, int* E){
    int dest = 0;
    int src = 0;
    float weight = 0;
    int v, e;
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "V %d\n", &v);
    fscanf(fp, "E %d\n", &e);
    *V = v;
    *E = e;
    int i;
    Graph* adjList = malloc(*V * sizeof(adjList));
    for(i = 0; i < *V; ++i){
        adjList[i].src = i + 1;
        adjList[i].edges = NULL;
    }

    for(i = 0; i < *E; ++i){
        fscanf(fp, "%d %d %f\n", &dest, &src, &weight);
        Edge* node = createEdge(dest, weight);
        adjList[src - 1].edges = updateEdges(adjList[src - 1].edges, node);
    }
    return adjList;
}

//Print graph
void printGraph(FILE* fp, Graph* adjList, int* V, int* E){
    fprintf(fp, "V %d\n", *V);
    fprintf(fp, "E %d\n", *E);
    int i;
    for(i = 0; i < *V; ++i){
        printf("%d-> ", adjList[i].src);
        Edge* temp = adjList[i].edges ;
        while(temp != NULL){
            printf("(%d, %f) ", temp->dest, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("HIII\n");
    return;
}

/*
float** createTable(struct Graph* graph){
    int i, j, k;
    int r = graph->V + 1;
    int c = graph->V;
    struct Edge* edge = graph->edge;
    float** arr = (float **)malloc(r * sizeof(float *)); 
    for (i = 0; i < r; ++i){
        arr[i] = (float *)malloc(c * sizeof(float));
        arr[0][0] = 0;
        for (j = 0; j < c; ++j){
            
        }
    }
    return arr;
}*/

/*
float** getShortestpath(struct Graph* graph, float** arr){
    int i, j, k;
    i = 0;


    for (i=1; i<=graph->V; i++){ 
        for (j=0; j<graph->V; j++) { 
            for (k=0; k < sizeof(edges[j])/sizeof(edges[j][0]); k++){ 
                if (arr[i-1][edges[j][k].src] != -1){ 
                    int curr_wt = arr[i-1][edges[j][k].src] + edges[j][k].weight; 
                    if(arr[i][j] == -1){
                        arr[i][j] = curr_wt;
                    } 
                    else{
                       arr[i][j] = min(arr[i][j], curr_wt); 
                    }
                } 
            } 
        } 
    } 
    return arr;
} */

/*
void printTable(struct Graph* graph, float** arr){
    int i, j;
    for (i=0; i<=graph->V; i++){ 
        for (j=0; j<graph->V; j++) {
            printf("%f ", arr[i][j]);
        }
        printf("\n");
    }
    return;
}

void freeGraph(struct Graph* graph){
    free(graph->edge);
    free(graph);
    return;
}
*/
