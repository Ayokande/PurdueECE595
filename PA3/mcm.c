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
    Graph* adjList = malloc((*V) * sizeof(*adjList));
    if(adjList == NULL)
    {
    	printf("ERROR");
    	return NULL;
    }
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
        Edge* temp = adjList[i].edges;
        while(temp != NULL){
            printf("(%d, %f) ", temp->dest, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
    return;
}

float getLambda(float** arr, int** parent, int* cycleDetector, int* clearArr, int* storeRow, int V, int E, int i, int j, int* col){
    float lambda;
    float prev = INFINITY;
    int ct = 0;
    int lcv;
    bool flag = false;
    while(i >= 0){
        if(cycleDetector[j] == 0){
            cycleDetector[j] = 1;
            storeRow[j] = i;
            clearArr[ct] = j;
            j = parent[i][j] -1;
            --i;
            ++ct;
        }else{
            lambda = (arr[storeRow[j]][j] - arr[i][j]) / (storeRow[j] - i);
            if(lambda < prev){
                *col = j;
            }
            prev = lambda;
            flag = true;
            break;
        }   
    }
    if(!flag){
        lambda = INFINITY;
    }

    for(lcv = 0; lcv < ct; ++lcv){
        cycleDetector[clearArr[lcv]] = 0;
    }
    return lambda;
}

float** createTable(FILE* fp1, FILE* fp2, Graph* graph, int V, int E){
    int i;
    int j;
    int v, k;
    int r = V + 1;
    int c = V;
    int col = 0;
    int row = 0;
    int val = 0;
    float lambda = INFINITY;
    float** arr = (float **)calloc(r, sizeof(float *)); 
    int** parent = (int**) calloc(r, sizeof(int*));
    int* cycleDetector = (int*)calloc(c, sizeof(int));
    int* clearArr = (int*)calloc(c, sizeof(int));
    int* storeRow = (int*)calloc(c, sizeof(int));
    float* piValues = (float*)calloc(c, sizeof(float));
    int* cycle = (int*)calloc(c, sizeof(int));
  
    arr[0] = (float *)calloc (c, sizeof(float));
    parent[0] = (int *)calloc (c, sizeof(int));

    for (j = 0; j < c; ++j){
        arr[0][j] = INFINITY;
        parent[0][j] = 0;
        if (j == 0){
            arr[0][j] = 0;
        }
    }    
    for (i = 1; i < r; ++i){
        arr[i] = (float *)calloc(V, sizeof(float));
        parent[i] = (int *)calloc(V, sizeof(int));
        for (j = 0; j < c; ++j){
            arr[i][j] = INFINITY;
            parent[i][j] = 0;
        }
        for (j = 0; j < c; ++j){ 
            Edge* temp = graph[j].edges;
            int src = graph[j].src;
            while(temp != NULL){
                int dest = temp->dest;
                if(arr[i - 1][src - 1] != INFINITY){
                    if( temp->weight + arr[i - 1][src - 1] < arr[i][dest - 1]){
                        arr[i][dest - 1] = arr[i - 1][src - 1] + temp->weight;
                        parent[i][dest - 1] = src;
                    }                
                }
                temp = temp->next;
            }      
        }
        float temp;
        if(((i > 1) && ((i & (i - 1)) == 0)) || (i == V)){
            for (j = 0; j < c; ++j){
                if(arr[i][j] != INFINITY){
                    temp = getLambda(arr, parent, cycleDetector, clearArr, storeRow, V, E, i, j, &col);
                    if(temp < lambda){
                        lambda = temp;
                        val = col;
                    }
                }
            }
            int a;
            for(a = 0; a < c; a++) piValues[a] = INFINITY;
            for(v = 0; v < c; ++v){
                for(k = 0; k <= i; ++k){
                    float tempPi = arr[k][v] - (k * lambda);
                    if(tempPi < piValues[v]){
                        piValues[v] = tempPi;
                    }
                }
            }

            bool terminate = true;
            if (lambda > 0){
                lambda *= 0.99999;
            }else{
                lambda *= 1.00001;
            }
            int var;
            for(var = 0; var < c; ++var){
                Edge* tempEdge = graph[var].edges;
                while(tempEdge != NULL){
                    float tempVar = piValues[var] + tempEdge->weight - lambda;
                    
                    if(piValues[tempEdge->dest - 1] > tempVar){
                        terminate = false;
                        break;
                    }
            
                    tempEdge = tempEdge->next;
                }
            }
            if(terminate){
                row = i;
                int cyc = 0;
                int start = val +1;
                cycle[cyc++] = start;
                while(row > 0){
                    if(start == parent[row][val]){
                        break;
                    }
                    cycle[cyc] = parent[row][val];
                    val = parent[row][val] - 1;
                    row--;
                    cyc++;
                }
                i = r;
                break;
            }
        }
    }
    //printf("Minimum Cycle Mean: %f\n", lambda);
    fwrite(&lambda , 1 , sizeof(float) , fp1);
    for(i = 0; i < c; ++i){
        if(cycle[i] == 0){
            break;
        }
        fprintf(fp2, "%d ", cycle[i]);
    }
    fprintf(fp2, "\n");
   
    freeParents(parent, V, E);
    free(cycleDetector);
    free(clearArr);
    free(storeRow);
    free(piValues);
    free(cycle);
    return arr;
}

void freeTable(float** arr, int V, int E){
    int i;
    for (i = 0; i <= V; ++i){
        float* curr = arr[i];
        free(curr);
    }
    free(arr);
    return;
}

void freeParents(int** arr, int V, int E){
    int i;
    for (i = 0; i <= V; ++i){
        int* curr = arr[i];
        free(curr);
    }
    free(arr);
    return;
}

void freeList(Edge* list){
    Edge* temp;
    while(list != NULL){
        temp = list;
        list = list->next;
        free(temp);
    }
}

void freeGraph(Graph* graph, int V, int E){
    int i;
    for(i = 0; i < V; ++i){
        freeList(graph[i].edges);
    }
    free(graph);
}


