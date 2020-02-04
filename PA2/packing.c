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
    node->left = NULL;
    node->right = NULL;
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
    node->xbest = node->ybest = 0;
    node->possibilities = NULL;
    node->tail = NULL;
    node->left = NULL;
    node->right = NULL;
    node->best = NULL;
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
            struct Lnode* x= root->possibilities;
            while(x != NULL){x = x->next;}
            root->tail = x; 
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

struct Lnode* reverseList(struct Lnode* head){
    struct Lnode *temp = NULL;   
    struct Lnode *current = head; 
    while (current !=  NULL) { 
       temp = current->prev; 
       current->prev = current->next; 
       current->next = temp;               
       current = current->prev; 
    }
    if(temp != NULL ){
        head = temp->prev;
    }
    return head;
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

void backTrack(struct Tnode* root, struct Lnode* curr){
    if(root == NULL || curr == NULL) return;
    root->best = curr;
    //printf("%d Best (%d,%d)\n", root->label, (curr->width), curr->height);
    backTrack(root->left, curr->left);
    backTrack(root->right, curr->right);
}
void out3(FILE* fp, struct Tnode* root){
    if(root == NULL) return;
    int getArea = INT_MAX;
    struct Lnode* curr = root->possibilities;
    while(curr != NULL){
        //printf("(%d,%d)\n", (curr->width), curr->height);
        if(curr->height * curr->width < getArea){
            getArea = curr->height * curr->width;
            root->best = curr;
        }
        curr = curr -> next;
    }
    fprintf(fp, "(%d,%d)\n", root->best->width, root->best->height);
    backTrack(root, root->best);
}

void getBestCoor(struct Tnode* root){
    if(root == NULL || root->orientation == ' ') return;
    getBestCoor(root->left);
    getBestCoor(root->right);
    if(root->orientation == 'V'){
        root->right->xbest += root->left->best->width;
    }
    if(root->orientation == 'H'){
        root->left->ybest += root->right->best->height;      
    }
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

struct Lnode* sortV(struct Lnode* head){
    if (head == NULL || head->next == NULL)
    {
        return head;
    }
    struct Lnode *sorted, *preSorted, *unsorted, *next;
    unsorted = head->next;
    head->next = NULL;
    while (unsorted != NULL) {
        sorted = head;
        preSorted = head;
        next = unsorted->next;
        int cnt = 0;
        while (sorted != NULL && sorted->width <= unsorted->width){
            preSorted = sorted;
            sorted = sorted->next;
            cnt++;
        }
        if (sorted){
            if (cnt == 0){
                head = unsorted;
            }else
            {
                preSorted->next = unsorted;
            }
            unsorted->next = sorted;
        }else{
            unsorted->next = NULL;
            preSorted->next = unsorted;
        }
        unsorted = next;
    }
    return head;
} 

struct Lnode* sortH(struct Lnode* head){
    if (head == NULL || head->next == NULL)
    {
        return head;
    }
    struct Lnode *sorted, *preSorted, *unsorted, *next;
    unsorted = head->next;
    head->next = NULL;
    while (unsorted != NULL) {
        sorted = head;
        preSorted = head;
        next = unsorted->next;
        int cnt = 0;
        while (sorted != NULL && sorted->height <= unsorted->height){
            preSorted = sorted;
            sorted = sorted->next;
            cnt++;
        }
        if (sorted){
            if (cnt == 0){
                head = unsorted;
            }else
            {
                preSorted->next = unsorted;
            }
            unsorted->next = sorted;
        }else{
            unsorted->next = NULL;
            preSorted->next = unsorted;
        }
        unsorted = next;
    }
    return head;
} 

void sortAll(struct Tnode* root, struct Tnode* parent){
    if(root == NULL){
        return;
    }
    sortAll(root->left,root);
    sortAll(root->right,root);
    if(root->orientation == ' '){
        if(parent->orientation == 'V'){
            root->possibilities = sortV(root->possibilities);
            root->possibilities = removeSuboptimalsV(root->possibilities);
        }else{
            root->possibilities = sortH(root->possibilities);
            root->possibilities = removeSuboptimalsH(root->possibilities);
        }
    }
}

struct Lnode* removeSuboptimalsV(struct Lnode* head){
    if(head == NULL || head->next == NULL){
        return head;
    }
    struct Lnode* back = head;
    struct Lnode* front = head->next;
    struct Lnode* temp;
    while(front != NULL){
        if(back->width == front->width){
            if(back->height >= front->height){
                temp = back;
                if(back->prev == NULL){
                    back = back->next;
                    front = front->next;
                }else{
                    back->prev->next = front;
                    front->prev = back->prev;
                    back = back->prev;
                }
                free(temp);
            }else{
            temp = front;
            front = front->next;
            back->next = front;
            front->prev = back;
            free(temp);
            }
        }else if(back->height <= front->height){
            temp = back;
            back->prev->next = front;
            front->prev = back->prev;
            back = back->prev; 
            free(temp);
        }else{
            back = back->next;
            front = front->next;
        }
    }
    return head;
}

struct Lnode* removeSuboptimalsH(struct Lnode* head){
    if(head == NULL || head->next == NULL){
        return head;
    }
    struct Lnode* back = head;
    struct Lnode* front = head->next;
    struct Lnode* temp;
    while(front != NULL){
        if(back->height == front->height){
            if(back->width >= front->width){
                temp = back;
                if(back->prev == NULL){
                    back = back->next;
                    front = front->next;
                }else{
                    back->prev->next = front;
                    front->prev = back->prev;
                    back = back->prev;
                }
                free(temp);
            }else{
            temp = front;
            front = front->next;
            back->next = front;
            front->prev = back;
            free(temp);
            }
        }else if(back->width <= front->width){
            temp = back;
            back->prev->next = front;
            front->prev = back->prev;
            back = back->prev; 
            free(temp);
        }else{
            back = back->next;
            front = front->next;
        }
    }
    return head;
}

struct Lnode* mergeListsV(struct Tnode* l1, struct Tnode* l2){
        
    struct Lnode* dummy = createLnode(-1, -1);
    struct Lnode* curr = dummy;
    struct Lnode* newNode;

    l1->possibilities = sortV(l1->possibilities);
    l2->possibilities = sortV(l2->possibilities);

    struct Lnode* curr1 = l1->possibilities;
    struct Lnode* curr2 = l2->possibilities;
    while(curr1 != NULL && curr2 != NULL){
        if(curr1->height > curr2->height){
            newNode = createLnode((curr1->width + curr2->width), curr1->height);
            newNode->left = curr1;
            newNode->right = curr2;
            curr->next = newNode;
            newNode->prev = curr;
            curr = newNode;
            curr1 = curr1->next;
        }else if(curr2->height > curr1->height){
            newNode = createLnode((curr1->width + curr2->width), curr2->height);
            newNode->left = curr1;
            newNode->right = curr2;
            curr->next = newNode;
            newNode->prev = curr;
            curr = newNode;
            curr2 = curr2->next;
        }else{
            newNode = createLnode((curr1->width + curr2->width), curr2->height);
            newNode->left = curr1;
            newNode->right = curr2;
            curr->next = newNode;
            newNode->prev = curr;
            curr = newNode;
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
    }
    curr = dummy->next;
    curr->prev = NULL;
    dummy->next = NULL;
    free(dummy);
    return curr;
}

struct Lnode* mergeListsH(struct Tnode* l1, struct Tnode* l2){
    
    struct Lnode* dummy = createLnode(-1, -1);
    struct Lnode* curr = dummy;
    struct Lnode* newNode;
    
    l1->possibilities = sortH(l1->possibilities);
    l2->possibilities = sortH(l2->possibilities);

    struct Lnode* curr1 = l1->possibilities;
    struct Lnode* curr2 = l2->possibilities;
    while(curr1 != NULL && curr2 != NULL){
        if(curr1->width > curr2->width){
            newNode = createLnode(curr1->width, (curr1->height + curr2->height));
            newNode->left = curr1;
            newNode->right = curr2;
            curr->next = newNode;
            newNode->prev = curr;
            curr = curr->next;
            curr1 = curr1->next;
        }else if(curr2->width > curr1->width){
            newNode = createLnode(curr2->width, (curr1->height + curr2->height));
            newNode->left = curr1;
            newNode->right = curr2;
            curr->next = newNode;
            newNode->prev = curr;
            curr = curr->next;
            curr2 = curr2->next;
        }else{
            newNode = createLnode(curr2->width, (curr1->height + curr2->height));
            newNode->left = curr1;
            newNode->right = curr2;
            curr->next = newNode;
            newNode->prev = curr;
            curr = curr->next;
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
    }
    curr = dummy->next;
    free(dummy);
    // curr = sortH(curr);
    // curr = removeSuboptimalsH(curr);
    return curr;
}

void dynamicProg(struct Tnode* root, struct Tnode* parent){
    if(root == NULL || root->orientation == ' '){
        return;
    }
    dynamicProg(root->left,root);
    dynamicProg(root->right,root);
    if(root->orientation == 'V'){
        root->possibilities = mergeListsV(root->left, root->right);
    }
    if(root->orientation == 'H'){
        root->possibilities = mergeListsH(root->left, root->right);
    }  
}

void inOrder(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }
    inOrder(fp, root->left);
    if(root->orientation != ' '){
        fprintf(fp, "%c\n", root->orientation);
    }else{
        fprintf(fp, "%d", root->label);
        fprintf(fp, "(");
        printPossibilities(stdout, root->possibilities);
        fprintf(fp, ")\n");
    }
    inOrder(fp, root->right);
}

void postOrderVH(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }
    postOrderVH(fp, root->left);
    postOrderVH(fp, root->right);

    if(root->orientation == 'V' || root->orientation == 'H'){
        fprintf(fp, "%c", root->orientation);
        fprintf(fp, "(");
        printPossibilities(stdout, root->possibilities);
        fprintf(fp, ")\n");
    }else{
        fprintf(fp, "%d", root->label);
        fprintf(fp, "(");
        printPossibilities(stdout, root->possibilities);
        fprintf(fp, ")\n");
    }
}

void out4(FILE* fp, struct Tnode* root){
    if(root == NULL){
        return;
    }

    if(root->orientation == 'H' || root->orientation == 'V'){   
        root->left->ybest += root->ybest;
        root->right->ybest += root->ybest;
        root->left->xbest += root->xbest;
        root->right->xbest += root->xbest;
    }

    out4(fp, root->left);
    out4(fp, root->right);
    if(root->orientation == ' '){
        fprintf(fp, "%d((%d,%d)(%d,%d))\n", root->label, root->best->width, root->best->height, root->xbest, root->ybest);
    }
}



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