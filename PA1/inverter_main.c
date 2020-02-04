#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inverter.h"

int main (int argc, char* argv[]){
    if (argc != 9){
		return EXIT_FAILURE;
	}
    FILE* in1_ptr = fopen(argv[2], "r"); //in_name1
    FILE* in2_ptr = fopen(argv[3], "r"); //in_name2
    FILE* in3_ptr = fopen(argv[4], "r"); //in_name3

    if (in1_ptr == NULL || in2_ptr == NULL || in3_ptr == NULL){
		return EXIT_FAILURE;
	}
    FILE* out1_ptr = fopen(argv[5], "w"); //out_name1
	FILE* out2_ptr = fopen(argv[6], "w"); //out_name1
    FILE* out3_ptr = fopen(argv[7], "w"); //out_name1
	FILE* out4_ptr = fopen(argv[8], "w"); //out_name1

    //double timeConstraint = atof(argv[1]);
    double inCap = 0;
    double outCap = 0;
    double outRes = 0;
    double wireRes = 0;
    double wireCap = 0;
    // int lines = getLines(in3_ptr);
    // fprintf(stdout, "%d",lines);
    

    getInverterValues(in1_ptr, &inCap, &outCap, &outRes);
    getWireValues(in2_ptr, &wireRes, &wireCap);
    //fprintf(stdout,"%.10le %.10le %.10le %.10le %.10le", inCap, outCap, outRes, wireRes, wireCap);
    struct Node* head = NULL;
    head = createNodeList(in3_ptr);
    struct Node* curr = head;
    struct Tnode* root = curr->self;
    root->top = true;
    curr = curr-> next;
    buildTree(root, &curr);
    printPreorderTree(out1_ptr, root);
    calcCapacitance(root, wireCap, outCap, ' ');
    calcElmore(root, wireCap, wireRes, outRes, ' ');
    printPreorderElmore(out2_ptr, root);

    printPostorderInverter(out3_ptr, root);

    //Close all file pointers
    fclose(in1_ptr);
    fclose(in2_ptr);
    fclose(in3_ptr);
    fclose(out1_ptr);
    fclose(out2_ptr);
    fclose(out3_ptr);
    fclose(out4_ptr);
    
    //Free all nodes to avoid memory issues
    destroyTree(root);
    destroyList(head);

    return EXIT_SUCCESS;
}

