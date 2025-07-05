/**************************************
Exercise: 	DS - Exercise Single - LinkedList 
Date:		3/7/2025
Developer:	Tal Hindi
Reviewer: 	Lotem Kitaroo	
Status:		Approved
**************************************/

#include <stdio.h>
#include <stdlib.h>


typedef struct node
{
    void *data;
    struct node *next;  
} node_t;


/* helpers */
static node_t *NewNode(int val, node_t *next)
{
    node_t *n = (node_t *)malloc(sizeof(node_t));
    int *p = (int *)malloc(sizeof(int));

    if (NULL == n || NULL == p)
    {
        return NULL;
    }

    *p = val;
    n->data = p;
    n->next = next;
    return n;
}

static void FreeList(node_t *head)
{
    node_t* tmp = NULL;

    while (NULL != head)
    {
        tmp  = head->next;
        free(head->data);
        free(head);
        head = tmp;
    }
}

static int IsEqual(const node_t *head, const int *arr, size_t n)
{
    size_t i = 0;

    for (i = 0; i < n; ++i)
    {
        if (!head || *(int *)head->data != arr[i])
        {
            return 0;
        }

        head = head->next;
    }

    return (head == NULL);
}

static void FlipTests(void)
{
    /* ---------- 3-nodes ---------- */
    {
        int exp[3] = {3, 2, 1};
        node_t *n3 = NewNode(3, NULL);
        node_t *n2 = NewNode(2, n3);
        node_t *head = NewNode(1, n2);

        head = Flip(head);
        printf("Flip – 3 nodes : %s\n",IsEqual(head, exp, 3) ? "PASS" : "FAIL");
        FreeList(head);
    }

    /* ---------- 2-nodes ---------- */
    {
        int exp[2] = {20, 10};
        node_t *n2 = NewNode(20, NULL);
        node_t *head = NewNode(10, n2);

        head = Flip(head);
        printf("Flip – 2 nodes : %s\n",IsEqual(head, exp, 2) ? "PASS" : "FAIL");
        FreeList(head);
    }

    /* ---------- single node ---------- */
    {
        node_t *head = NewNode(42, NULL);
        node_t *original = head;

        head = Flip(head);
        printf("Flip – single node addr  : %s\n",(head == original) ? "PASS" : "FAIL");
        FreeList(head);
    }

    /* ---------- NULL input ---------- */
    {
        node_t *head = NULL;
        head = Flip(head);
        printf("Flip – NULL input : %s\n",(head == NULL) ? "PASS" : "FAIL");
    }
}

static void HasLoopTests(void)
{
    /* ---------- no loop ---------- */
    {
        node_t *c3 = NewNode(3, NULL);
        node_t *c2 = NewNode(2, c3);
        node_t *head = NewNode(1, c2);

        printf("HasLoop – none: %s\n",(!HasLoop(head)) ? "PASS" : "FAIL");
        FreeList(head);
    }

    /* ---------- simple loop ---------- */
    {
        node_t *c3 = NewNode(3, NULL);
        node_t *c2 = NewNode(2, c3);
        node_t *head = NewNode(1, c2);
        c3->next = c2;           

        printf("HasLoop – simple loop: %s\n",(HasLoop(head)) ? "PASS" : "FAIL");
        c3->next = NULL;
        FreeList(head);
    }

    /* ---------- self loop ---------- */
    {
        node_t *head = NewNode(7, NULL);
        head->next = head;

        printf("HasLoop – self loop: %s\n",(HasLoop(head)) ? "PASS" : "FAIL");

        head->next = NULL;
        FreeList(head);
    }

    /* ---------- NULL list ---------- */
    {
        printf("HasLoop – NULL list: %s\n",(!HasLoop(NULL)) ? "PASS" : "FAIL");
    }
}

static void IntersectionTests(void)
{
    /* ---------- common tail ---------- */
    {
        node_t *tail2 = NewNode(8, NULL);
        node_t *tail1 = NewNode(7, tail2); 

        node_t *a2 = NewNode(2, tail1);
        node_t *a1 = NewNode(1, a2);

        node_t *b3 = NewNode(5, tail1);
        node_t *b2 = NewNode(4, b3);
        node_t *b1 = NewNode(3, b2);

        node_t *res = FindIntersection(a1, b1);
        printf("FindIntersection – tail  : %s\n",(res == tail1) ? "PASS" : "FAIL");


        b3->next = NULL; 
        FreeList(b1);
        FreeList(a1);
    }

    /* ---------- none ---------- */
    {
        node_t *a2 = NewNode(2, NULL);
        node_t *a1 = NewNode(1, a2);
        node_t *b2 = NewNode(20, NULL);
        node_t *b1 = NewNode(10, b2);
        node_t *res = FindIntersection(a1, b1);
        printf("FindIntersection – none  : %s\n",(res == NULL) ? "PASS" : "FAIL");
        FreeList(a1);
        FreeList(b1);
    }

    /* ---------- heads equal ---------- */
    {
        node_t *shared = NewNode(99, NULL);
        node_t *res = FindIntersection(shared, shared);
        printf("FindIntersection – heads : %s\n",(res == shared) ? "PASS" : "FAIL");
        FreeList(shared);
    }

    /* ---------- one list NULL ---------- */
    {
        node_t *listB = NewNode(7, NULL);
        node_t *res = FindIntersection(NULL, listB);
        printf("FindIntersection – NULL  : %s\n",(res == NULL) ? "PASS" : "FAIL");
        FreeList(listB);
    }
}


node_t *Flip(node_t *head)
{
    node_t *curr = head;
    node_t *prev = NULL;
    node_t *next = NULL;

    while (curr != NULL)
    {
        next       = curr->next;
        curr->next = prev;
        prev       = curr;
        curr       = next;
    }
    return prev;
}

int HasLoop(const node_t *head)
{
    const node_t *slow = head;
    const node_t *fast = head;

    while (slow && fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            return 1;
        }
    }
    return 0;
}

node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
    node_t *outer = head_2;

    while (outer != NULL)
    {
        node_t *inner = head_1;

        while (inner != NULL)
        {
            if (inner == outer)
            {
                return inner;
            }
            inner = inner->next;
        }
        outer = outer->next;
    }
    return NULL;
}



int main(void)
{
    FlipTests();
    HasLoopTests();
    IntersectionTests();
    return 0;
}






