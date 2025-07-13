/**************************************
Exercise:  DS - SortedL
Date:      8/7/2025
Developer: Tal Hindi
Reviewer:  Yael Keidar
Status:    Approved
**************************************/

#include <stdio.h>   /* printf 			*/
#include <stdlib.h>  /* malloc 			*/
#include <assert.h>  /* assert 			*/

#include "sortedl.h" /* SortedLCreate 	*/

#define CHECK(cond, msg)                                      \
    do {                                                      \
        if (cond) {                                           \
            ++g_pass;                                         \
            printf("PASS: %s\n", msg);                        \
        } else {                                              \
            ++g_fail;                                         \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);    \
        }                                                     \
    } while (0)

static unsigned g_pass = 0, g_fail = 0;

static int CompareInts(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

static int SumAction(void *data, void *param)
{
    *(int *)param += *(int *)data;
    return 0;
}

static int IsMatch(const void *data, const void *param)
{
    return (*(const int *)data == *(const int *)param);
}

/* ---------- tests --------------------------------------------------- */
static void TestCreateDestroy(void)
{
    sortedl_t *list = SortedLCreate(CompareInts);
    printf("\n-- TestCreateDestroy --\n");
    CHECK(NULL != list, "Created list is not NULL");
    CHECK(SortedLIsEmpty(list), "List is empty after create");
    SortedLDestroy(list);
}

static void TestInsertSorted(void)
{
    int nums[]     = {5, 1, 8, 3, 2};
    int expected[] = {1, 2, 3, 5, 8};
    sortedl_t *list = SortedLCreate(CompareInts);
    size_t i       = 0;
    sorted_iter_t it;

    printf("\n-- TestInsertSorted --\n");

    for (i = 0; i < 5; ++i)
    {
        SortedLInsert(list, &nums[i]);
    }

    it = SortedLBegin(list);
    for (i = 0; i < 5; ++i, it = SortedLNext(it))
    {
        CHECK(*(int *)SortedLGetData(it) == expected[i], "Check sorted order");
    }

    SortedLDestroy(list);
}

static void TestPopFrontBack(void)
{
    int nums[]   = {1, 2, 3};
    sortedl_t *list = SortedLCreate(CompareInts);

    printf("\n-- TestPopFrontBack --\n");

    SortedLInsert(list, &nums[1]); 
    SortedLInsert(list, &nums[2]);
    SortedLInsert(list, &nums[0]); 

    CHECK(*(int *)SortedLPopFront(list) == 1, "PopFront returns 1");
    CHECK(*(int *)SortedLPopBack(list)  == 3, "PopBack returns 3");
    CHECK(*(int *)SortedLPopFront(list) == 2, "PopFront returns 2 (last)");

    SortedLDestroy(list);
}

static void TestRemove(void)
{
    int nums[]   = {10, 5, 20};
    sortedl_t *list = SortedLCreate(CompareInts);
    sorted_iter_t it;

    printf("\n-- TestRemove --\n");

    SortedLInsert(list, &nums[0]); 
    SortedLInsert(list, &nums[1]); 
    SortedLInsert(list, &nums[2]); 

    it = SortedLBegin(list); 
    it = SortedLNext(it);   
    it = SortedLRemove(it);    

    CHECK(SortedLSize(list) == 2, "Size after remove");
    CHECK(*(int *)SortedLGetData(it) == 20, "Next after removed 10 is 20");

    SortedLDestroy(list);
}

static void TestFindAndFindIf(void)
{
    int nums[]   	= {5, 3, 9, 2, 8};
    int target   	= 9;
    int not_found 	= 100;
    sortedl_t *list = SortedLCreate(CompareInts);
    size_t i   = 0;
    sorted_iter_t from, to, res;

    printf("\n-- TestFindAndFindIf --\n");

    for (i = 0; i < 5; ++i)
    {
        SortedLInsert(list, &nums[i]);
    }

    from = SortedLBegin(list);
    to   = SortedLEnd(list);

    res = SortedLFind(list, from, to, &target);
    CHECK(*(int *)SortedLGetData(res) == 9, "Found 9");

    res = SortedLFind(list, from, to, &not_found);
    CHECK(SortedLIsEqual(res, to), "Not-found returns end");

    res = SortedLFindIf(from, to, IsMatch, &target);
    CHECK(*(int *)SortedLGetData(res) == 9, "FindIf found 9");

    SortedLDestroy(list);
}

static void TestForEach(void)
{
    int nums[]   = {1, 2, 3};
    sortedl_t *list = SortedLCreate(CompareInts);
    int sum      = 0;
    size_t i     = 0;

    printf("\n-- TestForEach --\n");

    for (i = 0; i < 3; ++i)
    {
        SortedLInsert(list, &nums[i]);
    }

    SortedLForEach(SortedLBegin(list), SortedLEnd(list), SumAction, &sum);
    CHECK(sum == 6, "Sum using ForEach");

    SortedLDestroy(list);
}

static void TestMerge(void)
{
    int a[]      		= {1, 3, 5};
    int b[]     	 	= {2, 4, 6};
    int expected[] 		= {1, 2, 3, 4, 5, 6};
    
    sortedl_t *list1 = SortedLCreate(CompareInts);
    sortedl_t *list2 = SortedLCreate(CompareInts);
    
    size_t i   = 0;
    sorted_iter_t it;

    printf("\n-- TestMerge --\n");

    for (i = 0; i < 3; ++i)
    {
        SortedLInsert(list1, &a[i]);
        SortedLInsert(list2, &b[i]);
    }

    SortedLMerge(list1, list2);

    it = SortedLBegin(list1);
    for (i = 0; i < 6; ++i, it = SortedLNext(it))
    {
        CHECK(*(int *)SortedLGetData(it) == expected[i], "Merged order ok");
    }

    SortedLDestroy(list1);
    SortedLDestroy(list2);
}

int main(void)
{
    printf("=== Sorted List Tests ===\n");

    TestCreateDestroy();
    TestInsertSorted();
    TestPopFrontBack();
    TestRemove();
    TestFindAndFindIf();
    TestForEach();
    TestMerge();

    printf("\nSummary: %u passed, %u failed\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}

