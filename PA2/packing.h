#ifndef PACKING_H
#define PACKING_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//LinkedList data structure
struct Lnode{
    int width;
    int height;
    int lposs;
    int rposs;
    struct Lnode* prev;
    struct Lnode* next;
};

//Binary Tree data structure
struct Tnode{
    char orientation;
    int label;
    int width;
    int height;
    int xcoor;
    int ycoor;
    struct Lnode* possibilities;
    struct Tnode* left;
    struct Tnode* right;
};

//Structure to store what we read from input file
struct Stack{
    struct Tnode* self;
    struct Stack* next;
};


int getLines(FILE* fp);
int getMax(int x, int y);
struct Lnode* createLnode(int x, int y);
struct Tnode* createTnode(int l, char orient);
struct Stack* initStack(struct Tnode* root);
struct Tnode* merge(struct Tnode* root, struct Tnode* left, struct Tnode* right);
void destroyStack(struct Stack* head);
struct Tnode* createNodeList(FILE* fp);
void printPossibilities(FILE* fp , struct Lnode* head);
void postOrder(FILE* fp, struct Tnode* root);
void getPacking(FILE* fp, struct Tnode* root);
void out1(FILE* fp, struct Tnode* root);
void getCoor(struct Tnode* root, int x, int y);
void out2(FILE* fp, struct Tnode* root);
struct Lnode* sortV(struct Lnode* head);
struct Lnode* sortH(struct Lnode* head);
void sortAll(struct Tnode* root, struct Tnode* parent);
void destroyList(struct Lnode* head);
void destroyTree(struct Tnode* root);

#endif