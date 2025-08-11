/**************************************
Exercise:   DS - BST
Date:       7/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     Approved
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/* prototypes */
static int IntCmp(const void *lhs, const void *rhs);
static void PrintInOrder(const bst_t *tree);
static void PrintFind(const bst_t *tree, int key);
static void RemoveKey(bst_t *tree, int key);
static void TestForEachAll(const bst_t *tree);
static void TestForEachStopAt(const bst_t *tree, int stop_at);

/* for-each callbacks */
static int SumAction(void *data, void *param);
typedef struct stop_param
{
    int target;
    size_t visited;
} stop_param_t;

static int StopAtAction(void *data, void *param);

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

static void PrintInOrder(const bst_t *tree)
{
    bst_iter_t it;
    bst_iter_t end;

    it.node = NULL;
    end.node = NULL;

    printf("In-order: ");
    it  = BSTBegin(tree);
    end = BSTEnd(tree);

    while (!BSTIterIsSame(it, end))
    {
        printf("%d ", *(int *)BSTGetData(it));
        it = BSTNext(it);
    }
    printf("\n");
}

static void PrintFind(const bst_t *tree, int key)
{
    bst_iter_t it;
    it.node = NULL;

    it = BSTFind(tree, (void *)&key);
    if (!BSTIterIsSame(it, BSTEnd(tree)))
    {
        printf("Find %d: found (%d)\n", key, *(int *)BSTGetData(it));
    }
    else
    {
        printf("Find %d: not found\n", key);
    }
}

static void RemoveKey(bst_t *tree, int key)
{
    bst_iter_t it;
    it.node = NULL;

    it = BSTFind(tree, (void *)&key);
    if (!BSTIterIsSame(it, BSTEnd(tree)))
    {
        BSTRemove(it);
        printf("Removed %d\n", key);
    }
    else
    {
        printf("Remove %d: not found\n", key);
    }
}

static int SumAction(void *data, void *param)
{
    long *psum = NULL;
    int val = 0;

    psum = (long *)param;
    val = *(int *)data;
    *psum += val;
    return 0;
}

static void TestForEachAll(const bst_t *tree)
{
    long sum = 0;
    int rc = 0;
    bst_iter_t begin_it;
    bst_iter_t end_it;

    begin_it.node = NULL;
    end_it.node = NULL;

    begin_it = BSTBegin(tree);
    end_it = BSTEnd(tree);

    sum = 0;
    rc = BSTForEach(begin_it, end_it, SumAction, &sum);
    printf("ForEach sum: %ld (rc=%d)\n", sum, rc);
}

static int StopAtAction(void *data, void *param)
{
    stop_param_t *p = NULL;
    int val = 0;

    p = (stop_param_t *)param;
    val = *(int *)data;

    p->visited += 1;
    if (val == p->target)
    {
        return 1;
    }
    return 0;
}

static void TestForEachStopAt(const bst_t *tree, int stop_at)
{
    stop_param_t p;
    int rc = 0;
    bst_iter_t begin_it;
    bst_iter_t end_it;

    p.target = stop_at;
    p.visited = 0;
    begin_it.node = NULL;
    end_it.node = NULL;

    begin_it = BSTBegin(tree);
    end_it = BSTEnd(tree);

    rc = BSTForEach(begin_it, end_it, StopAtAction, &p);
    printf("ForEach stop_at=%d: rc=%d, visited=%lu\n", stop_at, rc, (unsigned long)p.visited);
}

int main(void)
{
    bst_t *tree = NULL;
    int values[] = { 8, 3, 10, 1, 6, 14, 4, 7, 13 };
    size_t i = 0;
    size_t n = 0;

    n = sizeof(values) / sizeof(values[0]);

    printf("== BST tests ==\n");

    tree = BSTCreate(IntCmp);
    if (tree == NULL)
    {
        printf("BSTCreate failed\n");
        return 1;
    }

    printf("IsEmpty: %s\n", BSTIsEmpty(tree) ? "YES" : "NO");

    for (i = 0; i < n; ++i)
    {
        BSTInsert(tree, (void *)&values[i]);
        printf("Inserted %d\n", values[i]);
    }

    printf("Size: %lu\n", (unsigned long)BSTSize(tree));
    PrintInOrder(tree);

    PrintFind(tree, 6);
    PrintFind(tree, 2);
    PrintFind(tree, 13);
    PrintFind(tree, 42);

    TestForEachAll(tree);
    TestForEachStopAt(tree, 6);

    RemoveKey(tree, 7);
    printf("Size: %lu\n", (unsigned long)BSTSize(tree));
    PrintInOrder(tree);

    RemoveKey(tree, 14);
    printf("Size: %lu\n", (unsigned long)BSTSize(tree));
    PrintInOrder(tree);

    RemoveKey(tree, 3);
    printf("Size: %lu\n", (unsigned long)BSTSize(tree));
    PrintInOrder(tree);

    RemoveKey(tree, 8);
    printf("Size: %lu\n", (unsigned long)BSTSize(tree));
    PrintInOrder(tree);

    TestForEachAll(tree);
    TestForEachStopAt(tree, 10);

    BSTDestroy(tree);
    printf("Destroyed tree.\n");

    return 0;
}
