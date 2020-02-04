#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "packing.h"

int main(int argc, char* argv[]){
    if(argc != 6){
        return EXIT_FAILURE;
    }
    FILE* in_ptr = fopen(argv[1], "r"); //Input file

    if(in_ptr == NULL){
        return EXIT_FAILURE;
    }
    FILE* out_ptr1 = fopen(argv[2], "w");
    FILE* out_ptr2 = fopen(argv[3], "w");
    FILE* out_ptr3 = fopen(argv[4], "w");
    FILE* out_ptr4 = fopen(argv[5], "w");


    struct Tnode* root = createNodeList(in_ptr);
    getPacking(stdout, root);
    out1(out_ptr1, root);
    getCoor(root, 0, 0);
    out2(out_ptr2, root);
    sortAll(root, root);
    dynamicProg(root,root);
    out3(out_ptr3, root);   
    getBestCoor(root);
    out4(out_ptr4, root);
    destroyTree(root);

    //CLose all file pointers
    fclose(in_ptr);
    fclose(out_ptr1);
    fclose(out_ptr2);
    fclose(out_ptr3);
    fclose(out_ptr4);
    return EXIT_SUCCESS;
}
