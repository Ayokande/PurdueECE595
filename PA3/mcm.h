#ifndef MCM_H
#define MCM_H

//Edge structure
typedef struct Edge{
    int dest;
    float weight;
    struct Edge* next;
}Edge;

//Graph structure
typedef struct Graph{
    int src;
    Edge *edges;
}Graph;

float min(float a, float b);
Edge* updateEdges(Edge* list, Edge* node);
Edge* createEdge(int d, float w);
Graph* buildGraph(FILE* fp, int* V, int* E);
void printGraph(FILE* fp, Graph* adjList, int* V, int* E);
float getLambda(float** arr, int** parent, int* cycleDetector, int* clearArr, int* storeRow, int V, int E, int i, int j, int* col);
float** createTable(FILE* fp1, FILE* fp2, Graph* graph, int V, int E);
void freeTable(float** arr, int V, int E);
void freeParents(int** arr, int V, int E);
void freeList(Edge* list);
void freeGraph(Graph* graph, int V, int E);
//void printTable(Graph* graph, float** arr, int V, int E)
#endif