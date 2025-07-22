/**************************************
Exercise: 	DS - PQ
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		
**************************************/

#include <stdio.h>    /* printf */
#include <stdlib.h>   /* NULL   */
#include <assert.h>   /* assert */

#include "pq.h"      /* PQCreate */

#define CHECK(cond, msg)                                                     \
    do {                                                                     \
        if (cond) {                                                          \
            ++g_pass;                                                        \
            printf("PASS: %s\n", msg);                                       \
        } else {                                                             \
            ++g_fail;                                                        \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);                  \
        }                                                                    \
    } while (0)

static unsigned g_pass = 0; 
static unsigned g_fail = 0;

static int  CompareInts(const void* data1,const void* data2);
static int IsMatchInt(const void *data, const void *param);
static void TestCreateDestroy(); 
static void TestEnqueueOrder();
static void TestPeekAndDequeueEdge();    
static void TestClear();
static void TestErase();
static void TestErase();

int main(void)
{
    printf("=== Priority Queue Tests ===\n");

    TestCreateDestroy();  
    TestEnqueueOrder();      
    TestPeekAndDequeueEdge();    
    TestClear();
    TestErase();
    

    printf("\nSummary: %u passed, %u failed\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}

static int CompareInts(const void* data1,const void* data2)
{
	return *(int*)data1 - *(int*)data2;
}

static int IsMatchInt(const void *data, const void *param)
{
    int d = 0; 
    int p = 0;
    d = *(const int *)data;
    p = *(const int *)param;
    return d == p;
}

static void TestCreateDestroy(void)
{
    pq_t* pq = NULL; 
    int is_empty = 0; 
    size_t size = 0;

    pq = PQCreate(CompareInts);
    CHECK(pq != NULL, "Create returns non-NULL");

    size = PQSize(pq);
    CHECK(size == 0, "New PQ size is 0");

    is_empty = PQIsEmpty(pq);
    CHECK(is_empty == 1, "New PQ is empty");

    PQDestroy(pq);
}

static void TestEnqueueOrder()
{
	int rc = 0;
	size_t i = 0;
	void* peek_ptr = NULL; 
    int* front_val = NULL; 
    pq_t* pq = NULL;
    int values[5] = {5, 8, 3, 2, 1};
	int expected_order[5] = {8, 5, 3, 2, 1};
	
	pq = PQCreate(CompareInts);
    CHECK(pq != NULL, "EnqueueOrder: created PQ");
    
    for (i = 0; i < 5; ++i)
    {
        rc = PQEnqueue(pq, &values[i]);
        CHECK(rc == 0, "EnqueueOrder: PQEnqueue success");
    }
       
    peek_ptr = PQPeek(pq);
    front_val = (int *)peek_ptr;
    CHECK(front_val != NULL && *front_val == 8, "Peek returns highest element");

    for (i = 0; i < 5; ++i)
    {
        front_val = (int *)PQDequeue(pq);
        CHECK(front_val != NULL && *front_val == expected_order[i], "Dequeue returns ascending order");
    }

    CHECK(PQIsEmpty(pq) == 1, "PQ empty after removing all elements");
    PQDestroy(pq);
	
}

static void TestPeekAndDequeueEdge(void)
{
    pq_t *pq = NULL; 
    void* p = NULL; 
    int* int_p = NULL; 
    int x = 42; 
    int rc = 0; 
    int empty_flag = 0;

    pq = PQCreate(CompareInts);
    CHECK(pq != NULL, "Edge: created PQ");

    p = PQPeek(pq);
    CHECK(p == NULL, "Edge: Peek on empty returns NULL");


    rc = PQEnqueue(pq, &x);
    CHECK(rc == 0, "Edge: enqueue single success");
    CHECK(PQSize(pq) == 1, "Edge: size == 1 after single enqueue");

    int_p = (int *)PQPeek(pq);
    CHECK(int_p != NULL && *int_p == 42, "Edge: peek returns inserted value");
    CHECK(PQSize(pq) == 1, "Edge: size unchanged after peek");

    int_p = (int *)PQDequeue(pq);
    CHECK(int_p != NULL && *int_p == 42, "Edge: dequeue returns inserted value");

    empty_flag = PQIsEmpty(pq);
    CHECK(empty_flag == 1, "Edge: empty after dequeue");

    PQDestroy(pq);
}

static void TestClear(void)
{
    pq_t *pq = NULL; 
    int arr[4]; 
    int i = 0; 
    int rc = 0;

    arr[0] = 3; arr[1] = 1; arr[2] = 4; arr[3] = 2;

    pq = PQCreate(CompareInts);
    CHECK(pq != NULL, "Clear: created PQ");

    for (i = 0; i < 4; ++i)
    {
        rc = PQEnqueue(pq, &arr[i]);
        CHECK(rc == 0, "Clear: enqueue success");
    }

    CHECK(PQSize(pq) == 4U, "Clear: size == 4 before clear");
    PQClear(pq);
    CHECK(PQSize(pq) == 0U, "Clear: size == 0 after clear");
    CHECK(PQIsEmpty(pq) == 1, "Clear: empty after clear");
    CHECK(PQPeek(pq) == NULL, "Clear: peek after clear NULL");

    PQDestroy(pq);
}

static void TestErase(void)
{
    pq_t *pq = NULL; 
    int arr[6]; 
    int key = 30; 
    int i = 0; 
    int rc = 0; 
    int size_before = 0; 
    int size_after = 0; 
    int *front = NULL;

    arr[0] = 50; arr[1] = 20; arr[2] = 30; arr[3] = 10; arr[4] = 30; arr[5] = 40;

    pq = PQCreate(CompareInts);
    CHECK(pq != NULL, "Erase: created PQ");

    for (i = 0; i < 6; ++i)
    {
        rc = PQEnqueue(pq, &arr[i]);
        CHECK(rc == 0, "Erase: enqueue success");
    }

    size_before = (int)PQSize(pq);
    PQErase(pq, IsMatchInt, &key); 
    size_after = (int)PQSize(pq);
    CHECK(size_after == size_before - 1, "Erase: size decreased by 1");

    front = (int *)PQPeek(pq); 
    CHECK(front != NULL && *front == 50, "Erase: front still correct (50)");

    PQErase(pq, IsMatchInt, &key);
    CHECK(PQSize(pq) == (size_t)(size_before - 2), "Erase: second occurrence removed");

    PQDestroy(pq);
}

