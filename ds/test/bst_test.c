/**************************************
Exercise:  DS - BST
Date:      7/8/2025
Developer: Tal Hindi
Reviewer:  
Status:    
**************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bst.h"    /* bst_t, cmp_func_t, BSTCreate */


/* Simple int comparator: returns <0, 0, >0 */
static int IntCmp(const void *lhs, const void *rhs)
{
    int a = *(const int *)lhs;
    int b = *(const int *)rhs;
    return (a > b) - (a < b);
}

/* Test utilities */
#define CHECK(cond) do {                                            \
    ++g_total;                                                      \
    if (cond) { ++g_pass; } else {                                  \
        printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);      \
    }                                                               \
} while (0)

static int g_total = 0;
static int g_pass  = 0;

static void Test_BSTCreate_EmptyTree(void)
{
    bst_t *tree;

    tree = BSTCreate(IntCmp);
    CHECK(tree != NULL);
}

int main(void)
{
    Test_BSTCreate_EmptyTree();

    printf("\nBSTCreate tests: %d/%d passed\n", g_pass, g_total);
    return (g_pass == g_total) ? 0 : 1;
}
