#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "packing.h"

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
    return lineCt;
}

//Get the maximum between two values
int getMax(int x, int y){
    return x > y ? x : y;
}

//Create a ListNode
struct Lnode* createLnode(int x, int y){
    struct Lnode* node = (struct Lnode*) malloc(sizeof(*node));
    if(node == NULL){
        return NULL;
    }
    node->width = x;
    node->height = y;
    node->lposs = 0;
    node->rposs = 0;
    node->prev = NULL;
    node-> next = NULL;
    return node;
}

//Create a TreeNode 
struct Tnode* createTnode(int l, char orient){
    struct Tnode* node = (struct Tnode*) malloc(sizeof(*node));
    if(node == NULL){
        return NULL;
    }
    node->orientation = orient;
    node->label = l;
    node->width = 0;
    node->height = 0;
    node->xcoor = 0;
    node->ycoor = 0;
    node->possibilities = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

//Initialize the stack
struct Stack* initStack(struct Tnode* root){
    struct Stack* top = (struct Stack*) malloc(sizeof(*top));
    if(top == NULL){
        return NULL;
    }
    top->self = root;
    return top;
}

//Merge both the left and the right subtrees 
struct Tnode* merge(struct Tnode* root, struct Tnode* left, struct Tnode* right){
    root->left = left;
    root->right = right;
    return root;
}

void destroyStack(struct Stack* head){
    if(head == NULL){
        return;
    }
    destroyStack(head->next);
    free(head);
}


//Read from file and put everything on to stack
struct Tnode* createNodeList(FILE* fp){
    struct Stack* head = NULL;
    struct Tnode* tree = NULL;
    int numLines = getLines(fp);
    fseek(fp, 0, SEEK_SET);
    int ct = -1;
    int label;
    char orientation;
    int xdiff;
    int ydiff;
    while(ct++ < numLines){
        struct Tnode* root = NULL;
        if(fscanf(fp, "%d", &label) == 1){
            fgetc(fp);
            struct Lnode* llist = createLnode(0, 0);
            struct Lnode* back = llist;
            root = createTnode(label,' ');
            while(fscanf(fp, "(%d,%d)", &xdiff, &ydiff) == 2){
                struct Lnode* curr = createLnode(xdiff, ydiff);
                back->next = curr;
                curr->prev = back;
                back = curr;
            }
            llist->prev = back;
            root->possibilities = llist->next;
            free(llist);
            root->width = root->possibilities->width;
            root->height = root->possibilities->height;
            fgetc(fp);
        } else{
            fscanf(fp, "%c", &orientation);
            struct Stack* lead = head;
            struct Stack* follow = lead->next;
            struct Tnode* ntree = createTnode(0, orientation);
            if(follow == NULL){
                break;
            }
            struct Stack* sentinel = follow->next;
            root = merge(ntree, follow->self, lead->self);
            if(root == NULL){
                return NULL;
            }
            head = sentinel;
            free(lead);
            free(follow);
        }
        struct Stack* top = initStack(root);
        top->next = head;
        head = top;
    }
    tree = head->self;
    destroyStack(head);
    return tree;
}

//Print possibilities for the leaf node
void printPossibilities(FILE* fp , struct Lnode* head){
    struct Lnode* curr = head;
    while(curr != NULL){
        fprintf(stdout, "(%d,%d)", curr->width, curr->height);
        curr = curr->next;
    }
}

//Print the post order traversal of the tree
void postOrder(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }
    postOrder(fp, root->left);
    postOrder(fp, root->right);
    if(root->orientation != ' '){
        fprintf(fp, "%c\n", root->orientation);
    }else{
        fprintf(fp, "%d", root->label);
        fprintf(fp, "(");
        printPossibilities(stdout, root->possibilities);
        fprintf(fp, ")\n");

    }
    return;
}

void getPacking(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }
    getPacking(fp, root->left);
    getPacking(fp, root->right);
    
    if(root->orientation == 'H'){
        root->height = root->left->height + root->right->height;
        root->width = getMax(root->left->width, root->right->width);
    }
    if(root->orientation == 'V'){
        root->width = root->left->width + root->right->width;
        root->height = getMax(root->left->height, root->right->height);
    }
}

void out1(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }
    fprintf(fp, "(%d,%d)\n", root->width, root->height);
    return;
}

void getCoor(struct Tnode* root, int x, int y){
    if(root == NULL){
        return;
    }
    root->xcoor = x;
    root->ycoor = y;
    if(root->orientation == 'H'){
        getCoor(root->left, x, y + root->right->height);
        getCoor(root->right, x, y);
    }
    if(root->orientation == 'V'){
        getCoor(root->left, x, y);
        getCoor(root->right, x + root->left->width, y);
    }
    if(root->orientation == ' '){
        getCoor(root->left, x, y);
        getCoor(root->right, x, y);
    }

}

void out2(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }
    out2(fp, root->left);
    out2(fp, root->right);
    if(root->orientation == ' '){
        fprintf(fp, "%d((%d,%d)(%d,%d))\n", root->label, root->width, root-> height, root->xcoor, root->ycoor);
    }
}

// struct Lnode* sortV(struct Lnode* head){
//     if (head == NULL || head->next == NULL)
//     {
//         return head;
//     }
//     struct Lnode *sorted, *preSorted, *unsorted, *next;
//     unsorted = head->next;
//     head->next = NULL;
//     while (unsorted) {
//         sorted = head;
//         preSorted = head;
//         next = unsorted->next;
//         int cnt = 0;
//         while (sorted && sorted->width <= unsorted->width){
//             preSorted = sorted;
//             sorted = sorted->next;
//             cnt++;
//         }
//         if (sorted){
//             if (cnt == 0){
//                 head = unsorted;
//             }else
//             {
//                 preSorted->next = unsorted;
//             }
//             unsorted->next = sorted;
//         }else{
//             unsorted->next = NULL;
//             preSorted->next = unsorted;
//         }
//         unsorted = next;
//     }
//     return head;
// } 

// struct Lnode* sortH(struct Lnode* head){
//     if (head == NULL || head->next == NULL)
//     {
//         return head;
//     }
//     struct Lnode *sorted, *preSorted, *unsorted, *next;
//     unsorted = head->next;
//     head->next = NULL;
//     while (unsorted) {
//         sorted = head;
//         preSorted = head;
//         next = unsorted->next;
//         int cnt = 0;
//         while (sorted && sorted->height <= unsorted->height){
//             preSorted = sorted;
//             sorted = sorted->next;
//             cnt++;
//         }
//         if (sorted){
//             if (cnt == 0){
//                 head = unsorted;
//             }else
//             {
//                 preSorted->next = unsorted;
//             }
//             unsorted->next = sorted;
//         }else{
//             unsorted->next = NULL;
//             preSorted->next = unsorted;
//         }
//         unsorted = next;
//     }
//     return head;
// } 

// void sortAll(struct Tnode* root, struct Tnode* parent){
//     if(root == NULL){
//         return;
//     }
//     sortAll(root->left,root);
//     sortAll(root->right,root);
//     if(root->orientation == ' '){
//         if(parent->orientation == 'V'){
//             sortV(root->possibilities);
//         }else{
//             sortH(root->possibilities);
//         }
//     }
// }

//Free all linked list nodes
void destroyList(struct Lnode* head){
    if(head == NULL){
        return;
    }
    destroyList(head->next);
    free(head);
}

//Free all tree nodes
void destroyTree(struct Tnode* root){
    if(root != NULL){
        destroyTree(root->left);
        destroyTree(root->right);
        destroyList(root->possibilities);
        free(root);
    }
    return;
}