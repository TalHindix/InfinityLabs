/**************************************
Exercise: 	Exe - Merge Sorted lists
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc */

typedef struct Node
{
    int data;
    struct Node* next;
} Node;

static Node* MergeSort(Node* head);
static Node* Merge(Node* a, Node* b);
static Node* Split(Node* src);
static Node* Push(Node* head, int val);
static void PrintList(const Node* head);
static void FreeList(Node* head);

int main(void)
{
    Node* head1 = NULL;
    Node* head2 = NULL;
    Node* merged = NULL;

    head1 = Push(head1, 5);
    head1 = Push(head1, 10);
    head1 = Push(head1, 15);

    head2 = Push(head2, 20);
    head2 = Push(head2, 3);
    head2 = Push(head2, 2);

    printf("Original list-1: ");
    PrintList(head1);
    printf("\nOriginal list-2: ");
    PrintList(head2);

    head1 = MergeSort(head1);
    head2 = MergeSort(head2);
    merged = Merge(head1, head2);

    printf("\n\nMerged & Sorted: ");
    PrintList(merged);
    printf("\n");

    FreeList(merged);
    return 0;
}

static Node* MergeSort(Node* head)
{
    Node* tail = NULL;

    if ((NULL == head) || (NULL == head->next))
    {
        return head;
    }

    tail = Split(head);    
    head = MergeSort(head);
    tail = MergeSort(tail);

    return Merge(head, tail);
}

static Node* Merge(Node* a, Node* b)
{
    if (NULL == a) return b;
    if (NULL == b) return a;

    if (a->data <= b->data)
    {
        a->next = Merge(a->next, b);
        return a;
    }
    b->next = Merge(a, b->next);
    return b;
}

static Node* Split(Node* src)
{
    Node* slow = src;
    Node* fast = src->next;
    Node* tail = NULL;

    while ((NULL != fast) && (NULL != fast->next))
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    tail = slow->next;
    slow->next = NULL;
    return tail;
}

static Node* Push(Node* head, int val)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (NULL == node)
    {
        printf("malloc failed\n");
        return NULL;
    }

    node->data = val;
    node->next = head;
    return node;
}

static void PrintList(const Node* head)
{
    while (NULL != head)
    {
        printf("%d ", head->data);
        head = head->next;
    }
}

static void FreeList(Node* head)
{
    while (NULL != head)
    {
        Node* next = head->next;
        free(head);
        head = next;
    }
}
