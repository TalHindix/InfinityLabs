#include <stdio.h>
#include "../include/sll.h"

#define SUCCESS 1
#define FAILURE 0

static int PrintElement(void *data, void *unused)
{
    (void)unused;
    printf("%d -> ", *(int *)data);
    return 0;
}

static int CompareInt(const void *data, void *target)
{
    return (*(int *)data == *(int *)target);
}

static void ShowResult(const char *desc, int expected, int actual)
{
    printf("%-35s | Expected: %d | Actual: %d | %s\n", desc, expected, actual,(expected == actual) ? "SUCCESS" : "FAILURE");
}

static void RunSLLTests()
{
    sll_t *list = NULL;
    sll_iter_t iter = NULL;
    sll_iter_t result = NULL;

    int x = 5, y = 15, z = 25, w = 35, target = 15;
    int total = 0;

    printf("\n==X Create and Insert X==\n");
    list = SLLCreate();
    iter = SLLBegin(list);
    iter = SLLInsert(iter, &x);      
    SLLInsert(SLLNext(iter), &y);       
    SLLInsert(SLLEnd(list), &z);       

    total = SLLCount(list);
    ShowResult("Total Nodes after Insert", 3, total);

    printf("Current Single list: ");
    SLLForEach(SLLBegin(list), SLLEnd(list), PrintElement, NULL);
    printf(" (E)\n");

    printf("\n==X GetData and SetData X==\n");
    iter = SLLBegin(list);
    ShowResult("First node (GetData):", x, *(int *)SLLGetData(iter));

    SLLSetData(iter, &w);
    ShowResult("First node after SetData:", w, *(int *)SLLGetData(iter));

    printf("\n==X Find an node X==\n");
    result = SLLFind(SLLBegin(list), SLLEnd(list), CompareInt, &target);
    if (result != SLLEnd(list))
    {
        ShowResult("Found target value:", target, *(int *)SLLGetData(result));
    }
    else
    {
        printf("Find result: Not Found (FAILURE)\n");
    }

    printf("\n==X Remove Node X==\n");
    SLLRemove(SLLBegin(list));
    total = SLLCount(list);
    ShowResult("Nodes after one removal", 2, total);

    printf("Single List now: ");
    SLLForEach(SLLBegin(list), SLLEnd(list), PrintElement, NULL);
    printf("NULL\n");

    printf("\n==X Final Clean-Up Test X==\n");
    SLLRemove(SLLBegin(list));
    SLLRemove(SLLBegin(list));

    ShowResult("IsEmpty Func check", 1, SLLIsEmpty(list));
    ShowResult("Final count check", 0, SLLCount(list));

    SLLDestroy(list);
    printf("Single List successfully destroyed.\n");
}

int main()
{
    RunSLLTests();
    return 0;
}

