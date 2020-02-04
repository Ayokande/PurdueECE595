#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mcm.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        return EXIT_FAILURE;
    }

    FILE *in_ptr = fopen(argv[1], "r");
    if (in_ptr == NULL)
    {
        return EXIT_FAILURE;
    }

    FILE *out1_ptr = fopen(argv[2], "w");
    if(out1_ptr == NULL)
    {
    	return EXIT_FAILURE;
    }
    FILE *out2_ptr = fopen(argv[3], "w");
    if(out2_ptr == NULL)
    {
    	return EXIT_FAILURE;
    }
    int V;
    int E;
    float** arr;

    struct Graph* graph = buildGraph(in_ptr, &V , &E);
    //printf("GOING TO PRINT GRAPH\n");
    //printGraph(in_ptr, graph, &V, &E);
    //printf("GOING TO CREATE TABLE\n");
    arr = createTable(out1_ptr, out2_ptr, graph, V, E);
    //printTable(graph, arr, V, E);
    freeTable(arr, V, E);
    freeGraph(graph, V, E);
    
    fclose(in_ptr);
    fclose(out1_ptr);
    fclose(out2_ptr);
    return EXIT_SUCCESS;
}