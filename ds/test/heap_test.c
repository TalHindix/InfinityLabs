/**************************************
Exercise:   DS - Heap
Date:       25/8/2025
Developer:  Tal Hindi
Reviewer:   Baruchi Haimson   
Status:     
**************************************/

#include <stdio.h>   /* printf */

#include "heap.h"    /* HeapCreate */

#define CHECK(cond, msg)                                          \
    do {                                                          \
        if (cond) {                                               \
            ++g_pass;                                             \
            printf("[PASS] %s\n", msg);                           \
        } else {                                                  \
            ++g_fail;                                             \
            printf("[FAIL] %s  (line %d)\n", msg, __LINE__);      \
        }                                                         \
    } while (0)

/* ---------- GLOBALS ---------- */
static unsigned g_pass = 0;
static unsigned g_fail = 0;

/* ---------- PROTOTYPES ---------- */
static int IntCmp(const void *data1, const void *data2);

static void TestCreateDestroy(void);
static void TestPushPeekPop(void);
static void TestRemove(void);

/* ---------- MAIN ---------- */
int main(void)
{
    TestCreateDestroy();
    TestPushPeekPop();
    TestRemove();

    printf("\nSUMMARY: %u passed, %u failed\n", g_pass, g_fail);
    return (g_fail != 0); 
}

/* ---------- TEST CASES ---------- */
static void TestCreateDestroy(void)
{
    heap_t* heap = HeapCreate(IntCmp);
    CHECK(NULL != heap, "HeapCreate returned non-NULL");
    CHECK(HeapIsEmpty(heap), "New heap is empty");
    CHECK(0 == HeapSize(heap), "HeapSize == 0 on new heap");

    HeapDestroy(heap);
    CHECK(1, "HeapDestroy executed (no crash)");
}

static void TestPushPeekPop(void)
{
    heap_t* heap = HeapCreate(IntCmp);

    int a = 5, b = 3, c = 10, d = 15, e = 8;
    int expected_peek = 0;

    /* Push #1 */
    CHECK(0 == HeapPush(heap, &a), "HeapPush a");
    expected_peek = 5;
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek == 5");
    CHECK(1 == HeapSize(heap), "HeapSize == 1");

    /* Push #2 */
    CHECK(0 == HeapPush(heap, &b), "HeapPush b");
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek still 5");
    CHECK(2 == HeapSize(heap), "HeapSize == 2");

    /* Push #3 */
    CHECK(0 == HeapPush(heap, &c), "HeapPush c");
    expected_peek = 10;
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek == 10");
    CHECK(3 == HeapSize(heap), "HeapSize == 3");

    /* Push #4 */
    CHECK(0 == HeapPush(heap, &d), "HeapPush d");
    expected_peek = 15;
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek == 15");
    CHECK(4 == HeapSize(heap), "HeapSize == 4");

    /* Push #5 */
    CHECK(0 == HeapPush(heap, &e), "HeapPush e");
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek still 15");
    CHECK(5 == HeapSize(heap), "HeapSize == 5");

    /* Pop sequence: 15,10,8,5,3 */
    HeapPop(heap);
    CHECK(4 == HeapSize(heap), "HeapSize after 1st pop == 4");
    expected_peek = 10;
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek == 10");

    HeapPop(heap);
    expected_peek = 8;
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek == 8");

    HeapPop(heap);
    expected_peek = 5;
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek == 5");

    HeapPop(heap);
    expected_peek = 3;
    CHECK(*(int *)HeapPeek(heap) == expected_peek, "HeapPeek == 3");

    HeapPop(heap);
    CHECK(HeapIsEmpty(heap), "Heap empty after 5 pops");
    CHECK(0 == HeapSize(heap), "HeapSize == 0 after pops");

    HeapDestroy(heap);
}

static void TestRemove(void)
{
    heap_t* heap = HeapCreate(IntCmp);

    int a = 5, b = 3, c = 10, d = 15, e = 8;
    int to_remove = 10;

    HeapPush(heap, &a);
    HeapPush(heap, &b);
    HeapPush(heap, &c); 
    HeapPush(heap, &d);
    HeapPush(heap, &e);

    CHECK(5 == HeapSize(heap), "HeapSize == 5 before remove");
    CHECK(*(int *)HeapPeek(heap) == 15, "HeapPeek == 15 before remove");

    HeapRemove(heap, &to_remove, IntCmp);

    CHECK(4 == HeapSize(heap), "HeapSize == 4 after remove");
    CHECK(*(int *)HeapPeek(heap) == 15, "HeapPeek still 15 after remove");

    HeapRemove(heap, &to_remove, IntCmp);
    CHECK(4 == HeapSize(heap), "HeapSize unchanged when removing missing key");

    HeapDestroy(heap);
}

/* ---------- HELPERS ---------- */
static int IntCmp(const void* data1, const void* data2)
{
    return (*(const int *)data1 - *(const int *)data2);
}
