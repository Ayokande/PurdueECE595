#include<stdio.h>
#include<stdlib.h>

struct Node  {
	int data;
	struct Node* next;
	struct Node* prev;
};

struct Node* head; // global variable - pointer to head node.

//Creates a new Node and returns pointer to it. 
struct Node* GetNewNode(int x) {
	struct Node* newNode
		= (struct Node*)malloc(sizeof(struct Node));
	newNode->data = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

//Inserts a Node at head of doubly linked list
void InsertAtHead(int x) {
	struct Node* newNode = GetNewNode(x);
	if(head == NULL) {
		head = newNode;
		return;
	}
	head->prev = newNode;
	newNode->next = head; 
	head = newNode;
}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(int x) {
	struct Node* temp = head;
	struct Node* newNode = GetNewNode(x);
	if(head == NULL) {
		head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;
}

//Prints all the elements in linked list in forward traversal order
void Print() {
	struct Node* temp = head;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%d ",temp->data);
		temp = temp->next;
	}
	printf("\n");
}

//Prints all elements in linked list in reverse traversal order. 
void ReversePrint() {
	struct Node* temp = head;
	if(temp == NULL) return; // empty list, exit
	// Going to last Node
	while(temp->next != NULL) {
		temp = temp->next;
	}
	// Traversing backward using prev pointer
	printf("Reverse: ");
	while(temp != NULL) {
		printf("%d ",temp->data);
		temp = temp->prev;
	}
	printf("\n");
}

struct Node* sortV(struct Node* head){
    if (head == NULL || head->next == NULL)
    {
        return head;
    }
    struct Node *sorted, *preSorted, *unsorted, *next;
    unsorted = head->next;
    head->next = NULL;
    while (unsorted != NULL) {
        sorted = head;
        preSorted = head;
        next = unsorted->next;
        int cnt = 0;
        while (sorted != NULL && sorted->data <= unsorted->data){
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

int main() {

	/*Driver code to test the implementation*/
	head = NULL; // empty list. set head as NULL. 
	
	// Calling an Insert and printing list both in forward as well as reverse direction. 
	InsertAtTail(2); 
	InsertAtTail(4); 
	InsertAtHead(6); 
	InsertAtTail(8); 
    Print();
    head = sortV(head);
    Print();
   
	
}
