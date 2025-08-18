/**************************************
Exercise:   DS - AVL
Date:       18/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avl.h"

typedef enum
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
} children;

struct avl_node
{
    size_t height;
    void* data;
    struct avl_node* children[NUM_OF_CHILDREN];
};

typedef struct avl_node avl_node_t;

struct avl
{
   avl_node_t* root;
   cmp_func_t cmp;
};


/* ============== PROTOTYPES ===========================*/
static int IntCmp(const void *lhs, const void *rhs);
static void PrintInOrder(const avl_t *tree);
static void PrintSubtree(const avl_node_t* node);

int main(void)
{
    avl_t* tree = NULL;
    int values[] = { 8, 3, 10, 1, 6, 14, 4, 7, 13 };
    size_t i = 0;
    size_t n = 0;

    n = sizeof(values) / sizeof(values[0]);

    printf("== AVL tests ==\n");

    tree = AVLCreate(IntCmp);
    if (tree == NULL)
    {
        printf("AVLCreate failed\n");
        return 1;
    }

    printf("IsEmpty: %s\n", AVLIsEmpty(tree) ? "YES" : "NO");

    
    for (i = 0; i < n; ++i)
    {
        AVLInsert(tree, (void *)&values[i]);
        printf("Inserted %d\n", values[i]);
    }

    printf("IsEmpty: %s\n", AVLIsEmpty(tree) ? "YES" : "NO");

    printf("Size: %lu\n", (unsigned long)AVLCount(tree));
    PrintInOrder(tree);
/*
    PrintFind(tree, 6);
    PrintFind(tree, 2);
    PrintFind(tree, 13);
    PrintFind(tree, 42);

    TestForEachAll(tree);
    TestForEachStopAt(tree, 6);

    RemoveKey(tree, 7);
    printf("Size: %lu\n", (unsigned long)AVLCount(tree));
    PrintInOrder(tree);

    RemoveKey(tree, 14);
    printf("Size: %lu\n", (unsigned long)AVLCount(tree));
    PrintInOrder(tree);

    RemoveKey(tree, 3);
    printf("Size: %lu\n", (unsigned long)AVLCount(tree));
    PrintInOrder(tree);

    RemoveKey(tree, 8);
    printf("Size: %lu\n", (unsigned long)AVLCount(tree));
    PrintInOrder(tree);

    TestForEachAll(tree);
    TestForEachStopAt(tree, 10);

    AVLDestroy(tree);
    printf("Destroyed tree.\n");
    */
    return 0;
}

/* ============== HELPER FUNCTIONS ===========================*/
static int IntCmp(const void *lhs, const void *rhs)
{
    int a = 0;
    int b = 0;
    a = *(const int *)lhs;
    b = *(const int *)rhs;
    if (a < b) { return -1; }
    if (a > b) { return  1; }
    return 0;
}

static void PrintInOrder(const avl_t *tree)
{
    assert(tree);

    printf("In-order: ");
    PrintSubtree(tree->root);
    printf("\n");
}

static void PrintSubtree(const avl_node_t* node)
{
    if (node == NULL)
    {
        return;
    }

    PrintSubtree(node->children[LEFT]);
    printf("%d ", *(const int *)node->data);   
    PrintSubtree(node->children[RIGHT]);    
}