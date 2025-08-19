/**************
Exercise:   DS – Recursion
Date:       17/08/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     
**************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen */
#include <assert.h> /* assert */
#include <stdio.h>  /* printf */

#include "recursion.h" /* Fibonacci */
#include "stack.h" /* stack_t */

typedef struct fib_case_s
{
    int n;
    int expected;
} fib_case_t;

typedef struct strstr_case_s
{
    const char* text;
    const char* pat;
    int expect_found;
} strstr_case_t;

typedef struct strcat_case_s
{
    char* text;
    const char* to_con;
    const char* expected_str;
    int expect_found;
} strcat_case_t;

#define CHECK(COND, MSG)                                               \
    do                                                                 \
    {                                                                  \
        if (COND)                                                      \
        {                                                              \
            ++g_pass;                                                  \
            printf("PASS: %s\n", MSG);                                 \
        }                                                              \
        else                                                           \
        {                                                              \
            ++g_fail;                                                  \
            printf("FAIL: %s  (line %d)\n", MSG, __LINE__);            \
        }                                                              \
    } while (0)


/*================ PROTOTYPES ===============*/
static void TestFibonacci(void);
static void TestStrLenCmp(void);
static void TestStrStr(void);
static void TestStrCat(void);
static void TestFlipNode(void);
static void TestSortStack(void);
static int  IsStackSortedAsc(stack_t* s);

/*================ GLOBALS ==================*/

static unsigned g_pass = 0;
static unsigned g_fail = 0;

int main(void)
{
    printf("@@= Recursion Unit Tests =@@\n\n");

    TestFibonacci();
    TestStrLenCmp();
    TestStrStr();
    TestFlipNode();
    TestSortStack();
    TestStrCat();

    printf("\nTotal: PASS=%u, FAIL=%u\n", g_pass, g_fail);

    return (g_fail == 0) ? 0 : 1;
}

static void TestFibonacci(void)
{
    static fib_case_t cases[] =
    {
        { 0,  0 },
        { 1,  1 },
        { 5,  5 },
        { 10, 55 },
        { 25, 75025 }
    };

    size_t i = 0;

    printf(">> Fibonacci\n");

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        int n = cases[i].n;
        int expected = cases[i].expected;

        CHECK(Fibonacci(n) == expected, "Iterative value");
        CHECK(RecFibonacci(n) == expected, "Recursive value");
        CHECK(RecFibonacciMemo(n) == expected, "Memoized  value");
    }
}

static void TestStrLenCmp(void)
{
    printf("\n>> StrLenRec / StrCmpRec\n");

    CHECK(StrLenRec("tal") == 3, "StrLenRec len=3");
    CHECK(StrLenRec("")    == 0, "StrLenRec empty");

    CHECK(StrCmpRec("abc", "abc") == 0,  "StrCmpRec equal");
    CHECK(StrCmpRec("ab",  "ac")  <  0,  "StrCmpRec lt");
    CHECK(StrCmpRec("b",   "a")   >  0,  "StrCmpRec gt");
}

static void TestStrStr(void)
{
    static strstr_case_t cases[] =
    {
        { "recursion rocks", "rock",   1 },
        { "recursion rocks", "zzz",    0 },
        { "aaaa", "aa", 1 },
        { "aaaa", "aaaaa", 0 }
    };

    size_t i = 0;

    printf("\n>> StrStrRec\n");

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        const char* res = StrStrRec(cases[i].text, cases[i].pat);

        CHECK((res != NULL) == cases[i].expect_found,
              "StrStrRec presence");

        if (res != NULL)
        {
            CHECK(strncmp(res, cases[i].pat, strlen(cases[i].pat)) == 0, "StrStrRec correct pointer");
        }
    }
}

static void TestFlipNode(void)
{
    node_t node3;
    node_t node2;
    node_t node1;
    node_t* head = NULL;

    node3.data = 3;
    node3.next = NULL;

    node2.data = 2;
    node2.next = &node3;

    node1.data = 1;
    node1.next = &node2;

    head = &node1;

    printf("\n>> FlipNode\n");

    head = FlipNode(head);

    CHECK(head->data == 3 &&
          head->next->data == 2 &&
          head->next->next->data == 1 &&
          head->next->next->next == NULL,
          "FlipNode reverse 3-nodes");
}

static void TestSortStack(void)
{
    const int sample[] = { 3, 1, 4, 2, 5 };
    const size_t sample_sz = sizeof(sample) / sizeof(sample[0]);
    stack_t* s = NULL;
    size_t i = 0;

    printf("\n>> SortStack recursion\n");

    s = StackCreate(10, sizeof(int));
    CHECK(s != NULL, "StackCreate success");

    if (s == NULL)
    {
        return;
    }

    for (i = 0; i < sample_sz; ++i)
    {
        StackPush(s, &sample[i]);
    }

    SortedStack(s);

    CHECK(IsStackSortedAsc(s), "Stack sorted ascending");

    StackDestroy(s);

    s = StackCreate(5, sizeof(int));
    CHECK(s != NULL, "Empty stack create");

    if (s != NULL)
    {
        SortedStack(s);
        CHECK(StackIsEmpty(s), "SortStack on empty OK");
        StackDestroy(s);
    }
}

static int IsStackSortedAsc(stack_t* s)
{
    stack_t* tmp = NULL;
    int prev_val = 0;
    int curr_val = 0;
    int first = 1;
    int sorted = 1;

    tmp = StackCreate(StackCapacity(s), sizeof(int));
    if (tmp == NULL)
    {
        return 0;
    }

    while (!StackIsEmpty(s))
    {
        curr_val = *(int*)StackPeek(s);
        StackPop(s);

        if (!first && curr_val > prev_val)
        {
            sorted = 0;
        }

        first    = 0;
        prev_val = curr_val;

        StackPush(tmp, &curr_val);
    }

    while (!StackIsEmpty(tmp))
    {
        curr_val = *(int*)StackPeek(tmp);
        StackPop(tmp);
        StackPush(s, &curr_val);
    }

    StackDestroy(tmp);

    return sorted;
}

static void TestStrCat(void)
{
    static strcat_case_t cases[] =
    {
        { "tal", "hin","talhin" , 1 },
        { "blabla", "zzz","blablazzz",    1 },
        { "aaaa", "aa","aaaaaaa", 0 }
    };

    size_t i = 0;
    char* res = NULL;

    printf("\n>> StrCatRec\n");

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        char buffer[256];

        strcpy(buffer, cases[i].text);

        res = StrCatRec(buffer, cases[i].to_con);

        CHECK((strcmp(res, cases[i].expected_str) == 0) == cases[i].expect_found,
            "StrCat Success");
    }

}

