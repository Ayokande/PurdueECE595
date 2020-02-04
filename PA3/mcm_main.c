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
    FILE *out2_ptr = fopen(argv[3], "w");
    int V;
    int E;

    struct Graph* graph = buildGraph(in_ptr, &V , &E);
    //float** arr = createTable(graph);
    
    printf("HII\n");
    //CLose all file pointers and free nodes
    fclose(in_ptr);
    fclose(out1_ptr);
    fclose(out2_ptr);
    return EXIT_SUCCESS;
}