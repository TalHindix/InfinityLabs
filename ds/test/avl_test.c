/**************************************
Exercise:   DS - AVL
Date:       18/8/2025
Developer:  Tal Hindi
Reviewer:   Dana Chesner
Status:     Approved
**************************************/

#include <stdio.h>   /* printf */
#include <stdlib.h>  /* malloc */

#include "avl.h"     /* AVLCreate */


static int  IntCmp           (const void *lhs, const void *rhs);
static int  PrintIntAction   (void *data, void *param);
static void PrintPre         (const avl_t *tree);
static void PrintIn          (const avl_t *tree);
static void PrintPost        (const avl_t *tree);
static void PrintFind        (const avl_t *tree, int key);
static void RemoveKey        (avl_t *tree, int key);
static int  SumAction        (void *data, void *param);
static int  StopAtAction     (void *data, void *param);

typedef struct stop_param
{
    int    target;
    size_t visited;
} stop_param_t;

static void TestForEachAll    (const avl_t *tree);
static void TestForEachStopAt (const avl_t *tree, int stop_at);

/* ==== MAIN ======================================================= */
int main(void)
{
    avl_t *tree = NULL;
    int values[] = { 8, 3, 10, 1, 6, 14, 4, 7, 13 };
    size_t i = 0;
    size_t n = 0;

    printf("== AVL tests ==\n");

    tree = AVLCreate(IntCmp);
    if (!tree)
    {
        printf("AVLCreate failed\n");
        return 1;
    }

    printf("IsEmpty: %s\n", AVLIsEmpty(tree) ? "YES" : "NO");

    n = sizeof(values) / sizeof(values[0]);
    for (i = 0; i < n; ++i)
    {
        AVLInsert(tree, (void *)&values[i]);
        printf("Inserted %d   (height=%lu)\n",values[i], (unsigned long)AVLHeight(tree));
    }

    printf("Size: %lu  |  Height: %lu\n",(unsigned long)AVLCount(tree), (unsigned long)AVLHeight(tree));

    PrintPre(tree);
    PrintIn(tree);
    PrintPost(tree);

    PrintFind(tree, 6);
    PrintFind(tree, 2);
    PrintFind(tree, 13);
    PrintFind(tree, 42);

    TestForEachAll(tree);
    TestForEachStopAt(tree, 6);

    RemoveKey(tree, 7);
    printf("Size: %lu  |  Height: %lu\n", (unsigned long)AVLCount(tree),(unsigned long)AVLHeight(tree));
    PrintIn(tree);

    RemoveKey(tree, 14);
    printf("Size: %lu  |  Height: %lu\n", (unsigned long)AVLCount(tree), (unsigned long)AVLHeight(tree));
    PrintIn(tree);

    RemoveKey(tree, 3);
    printf("Size: %lu  |  Height: %lu\n", (unsigned long)AVLCount(tree), (unsigned long)AVLHeight(tree));
    PrintIn(tree);

    RemoveKey(tree, 8);
    printf("Size: %lu  |  Height: %lu\n", (unsigned long)AVLCount(tree), (unsigned long)AVLHeight(tree));
    PrintIn(tree);

    TestForEachAll(tree);
    TestForEachStopAt(tree, 10);

    AVLDestroy(tree);
    printf("Destroyed tree.\n");

    return 0;
}


static int IntCmp(const void *lhs, const void *rhs)
{
    int a = *(const int *)lhs;
    int b = *(const int *)rhs;
    return (a > b) - (a < b);
}

static int PrintIntAction(void *data, void *param)
{
    (void)param;
    printf("%d ", *(int *)data);
    return 0;
}

static void PrintPre(const avl_t *tree)
{
    printf("Pre-order:  ");
    AVLForEach((avl_t *)tree, AVL_PRE_ORDER, PrintIntAction, NULL);
    printf("\n");
}

static void PrintIn(const avl_t *tree)
{
    printf("In-order:   ");
    AVLForEach((avl_t *)tree, AVL_IN_ORDER, PrintIntAction, NULL);
    printf("\n");
}

static void PrintPost(const avl_t *tree)
{
    printf("Post-order: ");
    AVLForEach((avl_t *)tree, AVL_POST_ORDER, PrintIntAction, NULL);
    printf("\n");
}

static void PrintFind(const avl_t *tree, int key)
{
    void *res = AVLFind(tree, &key);
    if (res)
    {
        printf("Find %d: found (%d)\n", key, *(int *)res);
    }
    else
    {
        printf("Find %d: not found\n", key);
    }
}

static void RemoveKey(avl_t *tree, int key)
{
    if (AVLFind(tree, &key))
    {
        AVLRemove(tree, &key);
        printf("Removed %d\n", key);
    }
    else
    {
        printf("Remove %d: not found\n", key);
    }
}

static int SumAction(void *data, void *param)
{
    long *psum = (long *)param;
    *psum += *(int *)data;
    return 0;
}

static void TestForEachAll(const avl_t *tree)
{
    long sum = 0;
    AVLForEach((avl_t *)tree, AVL_IN_ORDER, SumAction, &sum);
    printf("ForEach sum (all): %ld\n", sum);
}

static int StopAtAction(void *data, void *param)
{
    stop_param_t *p = (stop_param_t *)param;
    p->visited += 1;
    if (*(int *)data == p->target)
    {
        return 1;
    }
    return 0;
}

static void TestForEachStopAt(const avl_t *tree, int stop_at)
{
    stop_param_t p = {0};
    int rc = 0;

    p.target  = stop_at;
    p.visited = 0;

    rc = AVLForEach((avl_t *)tree, AVL_IN_ORDER, StopAtAction, &p);
    printf("ForEach stop_at=%d: rc=%d, visited=%lu\n", stop_at, rc, (unsigned long)p.visited);
}

