#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inverter.h"

//Get the number of lines in a file so we can read
int getLines(FILE* fp){
    int lineCt = -1;
    char ch;
    fseek(fp, 0, SEEK_SET);
    ch = fgetc(fp);
    while(ch != EOF){
        if(ch == '\n'){
            lineCt++;
        }
        ch = fgetc(fp);
    }
    //printf("YES");
    return lineCt;
}

//Get inverter values from in_name1 (argv[2])
void getInverterValues(FILE* fp, double* inCap, double* outCap, double* outRes){
    fseek(fp, 0, SEEK_SET);
    double i_cap;
    double o_cap;
    double o_res;
    fscanf(fp,"%le %le %le\n",&i_cap, &o_cap, &o_res);
    *inCap = i_cap;
    *outCap = o_cap;
    *outRes = o_res;
    return;
}

//Get wire values from in_name2 (argv[3])
void getWireValues(FILE* fp, double *wireRes, double *wireCap){
    fseek(fp, 0, SEEK_SET);
    double w_res;
    double w_cap;
    fscanf(fp,"%le %le\n",&w_res, &w_cap);
    *wireRes = w_res;
    *wireCap = w_cap;
    return;
}

//Node constructor for linkedlist
struct Node* makeNode(struct Tnode* newNode){
    struct Node* node = (struct Node*) malloc(sizeof(*node));
    if(node == NULL){
        return NULL;
    }
    node->self = (struct Tnode*) newNode;
    node->next = NULL;
    return node;
}

//Function to add an element to the stack 
void push(struct Tnode* element, struct Node** head){
    struct Node* newNode = makeNode(element);
    if(newNode == NULL){
        return;
    }
    newNode->next = *head;
    *head = newNode;
    return;
}

//Function to remove element from the stack
struct Node* pop(struct Node** head){
    if(*head == NULL){
        return NULL;
    }
    struct Node* wanted = *head;
    *head = wanted->next;
    wanted->next = NULL;
    return wanted;
}

//Create a dynamic list(stack) of every node to be used in building the binary tree
struct Node* createNodeList(FILE* fp){
    struct Node* head = NULL; //Initialize the head of the list to be returned to null
    int numLines = getLines(fp);
    fseek(fp, 0, SEEK_SET);
    int ct = 0;
    while(ct++ <= numLines){
        struct Tnode* root = (struct Tnode*) malloc(sizeof(*root));
        if(root == NULL){
            return NULL;
        }
        root->leaf = false;
        root->top = false; 
        root->sinkLabel = 0;
        root->sinkCapacitance = 0; 
        root->wireLeft = 0; 
        root->wireRight = 0; 
        root->ownCapacitance = 0; 
        root->elmoreDelay = 0;
        root->left = NULL;
        root->right = NULL;
        root->parent = NULL;
        char curr = fgetc(fp);
        if(curr == '('){
            fseek(fp, -1, SEEK_CUR);
            fscanf(fp,"(%le %le)\n",&(root->wireLeft), &(root->wireRight));
            //fprintf(stdout, "(%.10le %.10le)\n", root->wireLeft, root->wireRight);
            push(root, &head);
        } else{
            fseek(fp, -1, SEEK_CUR);
            fscanf(fp,"%d(%le)\n",&(root->sinkLabel), &(root->sinkCapacitance));
            //fprintf(stdout, "%d(%.10le)\n", root->sinkLabel, root->sinkCapacitance);
            root->leaf = true; //Set the value to be T indicating this is a leaf node
            push(root, &head);
        }
    }
    return head;
}

//Build the tree from the linked list that was created
void buildTree(struct Tnode* root, struct Node** head){
    if(*head == NULL){
        return; 
    }
    if(root->leaf == true){
        return;
    }
    if(root->right == NULL){
        root->right = (*head)->self;
        root->right->parent = root;
        (*head) = (*head)->next;
        buildTree(root->right, &(*head));
    }
    if(root->left == NULL){
        root->left = (*head)->self;
        root->left->parent = root;
        (*head) = (*head)->next;
        buildTree(root->left, &(*head));
    }
    if(root->right != NULL && root->right->leaf == false){
        root->right->parent = root;
        buildTree(root->right, &(*head));
    }
    if(root->left != NULL && root->left->leaf == false){
        root->left->parent = root;
        buildTree(root->left, &(*head));
    }
    return;
}

//Print pre-order traversal of tree to out_name1
void printPreorderTree(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return; //Nothing to print
    }
    if(root->leaf == true){
        fprintf(fp,"%d(%.10le)\n", (root->sinkLabel), (root->sinkCapacitance));
    } else{
        fprintf(fp,"(%.10le %.10le)\n", (root->wireLeft), (root->wireRight));
    }
    printPreorderTree(fp, root->left);
    printPreorderTree(fp, root->right);
}

//Calculate Capacitance at each node in the tree
double calcCapacitance(struct Tnode* root, double wireCap, double outCap, char branch){
    if(root == NULL){
        return 0; //Nothing to compute (empty (sub)tree)
    } 
    double Cnode = 0;
    if(root->top == true){
        Cnode = (((root->wireLeft * wireCap)/2) + ((root->wireRight * wireCap)/2)) + outCap; //Root of tree
    } else if(root->leaf == false){
        Cnode = (((root->wireLeft * wireCap)/2) + ((root->wireRight * wireCap)/2));
        if(branch == 'l'){
            Cnode += (root->parent->wireLeft * wireCap)/2;
        }
        if(branch == 'r'){
            Cnode += (root->parent->wireRight * wireCap)/2;
        }
    } else if(root->leaf == true){
        Cnode = root->sinkCapacitance;
        if(branch == 'l'){
            Cnode += (root->parent->wireLeft * wireCap)/2;
        }
        if(branch == 'r'){
            Cnode += (root->parent->wireRight * wireCap)/2;
        }
        root->ownCapacitance = Cnode;
        return root->ownCapacitance;
    }
    root->ownCapacitance = Cnode;
    root->ownCapacitance += calcCapacitance(root->left, wireCap, outCap, 'l');
    root->ownCapacitance += calcCapacitance(root->right, wireCap, outCap, 'r');
    return root->ownCapacitance;
}

//Calculate Elmore delay at each node in the tree
double calcElmore(struct Tnode* root, double wireCap, double wireRes, double outRes, char branch){
    if(root == NULL){
        return 0;
    }
    if(root->top == true){
        root->elmoreDelay = root->ownCapacitance * outRes;
    } else{
        double res = 0;
        if(branch == 'l'){
            res = root->parent->wireLeft * wireRes;
        }
        if(branch == 'r'){
            res = root->parent->wireRight * wireRes;
        }
        root->elmoreDelay = root->parent->elmoreDelay + (res * root->ownCapacitance);
    }
    if(root-> leaf == true){
        return root->elmoreDelay;
    }
    calcElmore(root->left, wireCap, wireRes, outRes, 'l');
    calcElmore(root->right, wireCap, wireRes, outRes, 'r');
    return root->elmoreDelay;
}

//Print pre-order of elmore delay values
void printPreorderElmore(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }
    if(root->leaf == true){
        //fprintf(stdout, "%d(%.10le)\n", root->sinkLabel, root->elmoreDelay);
        fwrite(&(root->sinkLabel), sizeof(int), 1, fp);
        fwrite(&(root->elmoreDelay), sizeof(double), 1, fp);
    }
    printPreorderElmore(fp, root->left);
    printPreorderElmore(fp, root->right);
}

void printPostorderInverter(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return; 
    }
    printPostorderInverter(fp, root->left);
    printPostorderInverter(fp, root->right);
    if(root->leaf == true){
        fprintf(fp,"%d(%.10le)\n", (root->sinkLabel), (root->sinkCapacitance));
        //fprintf(stdout,"%d(%.10le)\n", (root->sinkLabel), (root->sinkCapacitance));
    } else{
        fprintf(fp,"(%.10le %.10le)\n", (root->wireLeft), (root->wireRight));
        //fprintf(stdout,"(%.10le %.10le)\n", (root->wireLeft), (root->wireRight));
    }
}

//Destroy the list(stack) by freeing all nodes
void destroyList(struct Node* head){
    if(head == NULL){
        return;
    }
    destroyList(head->next);
    free(head);
}

//Destroy the tree by freeing all Tree nodes
void destroyTree(struct Tnode* root){
  if( root != NULL ){
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
  }
  return;
}