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
#endif