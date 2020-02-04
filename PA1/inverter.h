#ifndef INVERTER_H
#define INVERTER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Tree structure to store each node in BT
struct Tnode{
  bool leaf; //Holds a F or T value to let us know if it is a leaf node or not
  bool top; //Holds a F or T value to let us know if it is the root node or not
  int sinkLabel; //Holds non-zero integer value if it is a leaf node
  double sinkCapacitance; //Holds the sink Capacitance if it is a leaf node
  double wireLeft; //Holds the length of the wire to the left
  double wireRight; //Holds the length of the wire to the right
  double ownCapacitance; //
  double elmoreDelay; //Holds the Elmore Delay at the current TreeNode
  struct Tnode* left; //Pointer to left child
  struct Tnode* right; //Pointer to right child
  struct Tnode* parent; //Pointer to parent node of self
};

//LinkedList structure to store each node as we read from file
struct Node{
  struct Tnode* self;
  struct Node* next;
};

//Function Declarations
int getLines(FILE* fp);
void getInverterValues(FILE* fp, double* inCap, double* outCap, double* outRes);
void getWireValues(FILE* fp, double *wireRes, double *wireCap);
struct Node* makeNode(struct Tnode* newNode);
void push(struct Tnode* element, struct Node** head);
struct Node* pop(struct Node** head);
struct Node* createNodeList(FILE* fp);
void buildTree(struct Tnode* root, struct Node** head);
void printPreorderTree(FILE* fp, struct Tnode* root);
double calcCapacitance(struct Tnode* root, double wireCap, double outCap, char branch);
double calcElmore(struct Tnode* root, double wireCap, double wireRes, double outRes, char branch);
void printPreorderElmore(FILE* fp, struct Tnode* root);
void printPostorderInverter(FILE* fp, struct Tnode* root);
void destroyList(struct Node* head);
void destroyTree(struct Tnode* root);

#endif