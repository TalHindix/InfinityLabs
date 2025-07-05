#include <stdio.h>
#include "sll.h"

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
    printf("%s | Expected: %d | Actual: %d | %s\n", desc, expected, actual,(expected == actual) ? "SUCCESS" : "FAILURE");
}


static void TestSLLAppend()
{
    sll_t *src = NULL;
    sll_t *dst = NULL;
    sll_iter_t iter = NULL;

    int a = 1, b = 2, c = 3;
    int d = 10, e = 20; 
    int expected_after[] = {10, 20, 1, 2, 3};
    size_t i = 0;
    int ok = SUCCESS;
	
	printf("\n==X Append SLL X==\n");
    /* build src list: 1 -> 2 -> 3 */
    src  = SLLCreate();
    iter = SLLBegin(src);
    iter = SLLInsert(iter, &a);            
    iter = SLLNext(iter);
    iter = SLLInsert(iter, &b);              
    SLLInsert(SLLNext(iter), &c);          

    /* build dst list: 10 -> 20 */
    dst  = SLLCreate();
    
    iter = SLLBegin(dst);
    iter = SLLInsert(iter, &d);          
    SLLInsert(SLLNext(iter), &e);
	
    ShowResult("Src count before append", 3, SLLCount(src));
    ShowResult("Dst count before append", 2, SLLCount(dst));

    SLLAppend(src, dst);

    ShowResult("Dst count after append", 5, SLLCount(dst));


    iter = SLLBegin(dst);
    for (i = 0; i < 5; ++i, iter = SLLNext(iter))
    {
        if (iter == SLLEnd(dst) || *(int *)SLLGetData(iter) != expected_after[i])
        {
            ok = FAILURE;
            break;
        }
    }
    
    ShowResult("Order validation", SUCCESS, ok);

    printf("Dst list after append: ");
    SLLForEach(SLLBegin(dst), SLLEnd(dst), PrintElement, NULL);
    printf("(E)NULL\n");

    SLLDestroy(src);
}

static void TestSLL()
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

    TestSLL();
    TestSLLAppend();

    return 0;
}

