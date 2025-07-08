/**************************************
Exercise: 	DS - DLL
Date:		8/7/2025
Developer:	Tal Hindi
Reviewer: 	Menny Markovich
Status:		
**************************************/

#include <stdio.h>   /* printf  */
#include <string.h>  
#include <assert.h>  /* assert  */
#include "dll.h"    /* DLLCreate */

/* ------------- global counters + macro ----------- */
static unsigned g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                      \
    do {                                                     \
        if (cond) {                                          \
            ++g_pass;                                        \
            printf("PASS: %s\n", msg);                      \
        } else {                                             \
            ++g_fail;                                        \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);   \
        }                                                    \
    } while (0)

/* -------------- helpers ----------- */
static size_t ManualCount(const dll_t* list)
{
    size_t count         = 0;
    dll_iter_t iter      = DLLBegin(list);
    dll_iter_t end_iter  = DLLEnd(list);

    while (!DLLIsEqual(iter, end_iter))
    {
        ++count;
        iter = DLLNext(iter);
    }
    return count;
}

static int MatchInt(const void *data, const void *param)
{
    return *(const int*)data == *(const int*)param;
}

static int SumAction(void* data,void* param)
{ 
	*(int*)param+=*(int*)data;
	return 0;
}


/* -------------------- tests implementation -------------------- */
static void TestCreateDestroy(void)
{
    dll_t *list;

    printf("-- TestCreateDestroy --");
    list = DLLCreate();
    CHECK(list != NULL, "DLLCreate returned non-NULL");
    CHECK(DLLIsEqual(DLLBegin(list), DLLEnd(list)), "begin == end after create");
    CHECK(ManualCount(list) == 0, "ManualCount == 0 on empty list");
    CHECK(DLLCount(list)==0,"DLLCount == 0 on empty list");
    DLLDestroy(list);
}

static void TestPushFrontBack(void)
{
    dll_t *list;
    int data_a = 1, data_b = 2, data_c = 3;

    printf("-- TestPushFrontBack --");
    list = DLLCreate();

    DLLPushFront(list, &data_a); 
    DLLPushBack (list, &data_b); 
    DLLPushFront(list, &data_c); 

    CHECK(ManualCount(list) == 3, "Three elements after pushes");
    CHECK(*(int*)DLLPopFront(list) == 3, "PopFront returned 3");
    CHECK(*(int*)DLLPopBack (list) == 2, "PopBack returned 2");
    CHECK(*(int*)DLLPopFront(list) == 1, "PopFront returned 1");
    CHECK(ManualCount(list) == 0, "List empty after pops");

    DLLDestroy(list);
}

static void TestInsertRemove(void)
{
    dll_t *list;
    dll_iter_t second_iter;
    dll_iter_t mid_iter;
    int data_a = 10, data_b = 20, data_mid = 99;

    printf("-- TestInsertRemove --");
    list = DLLCreate();

    DLLPushBack(list, &data_a);
    DLLPushBack(list, &data_b);          

    second_iter = DLLNext(DLLBegin(list));
    DLLInsert(list, second_iter, &data_mid);

    CHECK(ManualCount(list) == 3, "Count after insert");

    mid_iter = DLLNext(DLLBegin(list));
    CHECK(*(int*)DLLGetData(mid_iter) == 99, "Middle node is 99");

    DLLRemove(mid_iter);
    CHECK(ManualCount(list) == 2, "Count after remove");

    DLLDestroy(list);
}

static void TestFind(void)
{
    dll_t *list;
    dll_iter_t result_iter;
    int data_a = 5, data_b = 6, data_c = 7;
    int key_hit = 6, key_miss = 42;

    printf("-- TestFind --");
    list = DLLCreate();

    DLLPushBack(list, &data_a);
    DLLPushBack(list, &data_b);
    DLLPushBack(list, &data_c);

    result_iter = DLLFind(DLLBegin(list), DLLEnd(list), MatchInt, &key_hit);
    CHECK(!DLLIsEqual(result_iter, DLLEnd(list)) && *(int*)DLLGetData(result_iter) == 6, "Found existing element");

    result_iter = DLLFind(DLLBegin(list), DLLEnd(list), MatchInt, &key_miss);
    CHECK(DLLIsEqual(result_iter, DLLEnd(list)), "Return end when not found");

    DLLDestroy(list);
}

static void TestCountAndForEach(void)
{
    dll_t *list;
    int a=1,b=2,c=3,sum=0;
    
    printf("-- TestCountAndForEach --");
    
    list=DLLCreate();
    
    CHECK(DLLCount(list)==0,"DLLCount empty list");
    DLLPushBack(list,&a);DLLPushBack(list,&b);DLLPushBack(list,&c);
    CHECK(DLLCount(list)==3,"DLLCount three nodes");
    DLLForEach(DLLBegin(list),DLLEnd(list),SumAction,&sum);
    CHECK(sum==6,"DLLForEach summed values");
    DLLPopBack(list);
    CHECK(DLLCount(list)==2,"DLLCount after pop");
    DLLDestroy(list);
}


/* -------------------- main -------------------- */
int main(void)
{
    printf("\n===! DLL Tests !===\n");

    TestCreateDestroy();
    TestPushFrontBack();
    TestInsertRemove();
    TestFind();
    TestCountAndForEach();

    printf("\nSummary: %u passed, %u failed\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}





