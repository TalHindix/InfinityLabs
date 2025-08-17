/**************************************
Exercise:   DS - Recursion
Date:       17/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     
**************************************/

#include <stdlib.h> /* malloc */
#include <string.h> /* strlen */

#include "rec.h" /* Fibonacci */
#include "stack.h" /* StackCreate */

#define MAX_FIB 93

static int g_fib_memo[MAX_FIB];

/* Helper function */
static int PopInts(stack_t* s);
static void PushInt(stack_t* s, int val);
static void InsertSorted(stack_t* s, int val);
static void FibMemoInit(void);

int Fibonacci (size_t n)
{
    int previous__previous_number = 0;
    int previous_number = 1;
    int current_number = 0;
    size_t i = 0;

    if (n == 0)
    {
        return 0;
    }

    if (n == 1)
    {
        return 1;
    }

    for(i = 2; i <= n; ++i)
    {
        current_number = previous_number + previous__previous_number;
        previous__previous_number = previous_number;
        previous_number = current_number;
    }

    return current_number;
}

int RecFibonacci(size_t n)
{
    if(n <= 1)
    {
        return n;
    }

    return RecFibonacci(n - 1) + RecFibonacci (n - 2);

}

int RecFibonacciMemo(size_t n)
{
    static int memo_ready = 0;

    if (!memo_ready)
    {
        FibMemoInit();
        memo_ready = 1;
    }

    if (n >= MAX_FIB)
    {
        return RecFibonacci(n);
    }

    if (-1 != g_fib_memo[n])
    {
        return g_fib_memo[n];
    }

    if (n <= 1)
    {
        g_fib_memo[n] = (int)n;
        return g_fib_memo[n];
    }

    g_fib_memo[n] = RecFibonacciMemo(n - 1) + RecFibonacciMemo(n - 2);
    return g_fib_memo[n];
}

node_t* FlipNode(node_t* head)
{
    node_t *new_head = NULL;

    if (NULL == head || NULL == head->next)
    {
        return head; 
    }

    new_head = FlipNode(head->next);

    head->next->next = head;
    head->next = NULL;

    return new_head;
}

size_t StrLenRec(const char* str)
{
    if ('\0' == *str)
    {
        return 0;
    }

    return 1 + StrLenRec(str + 1);
}

int StrCmpRec(const char *s1, const char *s2)
{
    if ('\0' == *s1 && '\0' == *s2)
    {
        return 0;
    }

    if (*s1 != *s2)
    {
        return (unsigned char)*s1 - (unsigned char)*s2;
    }

    return StrCmpRec(s1 + 1, s2 + 1);
}

char* StrCpyRec(char* dst, const char* src)
{

    *dst = *src;

    if ('\0' == *src)
    {
        return dst;
    }

    StrCpyRec(dst+1,src+1);

    return dst;
}

char* StrStrRec(const char* haystack, const char* needle)
{
    size_t needle_len = strlen(needle);

    if('\0' == *needle)
    {
        return (char*)haystack;
    }

    if('\0' == *haystack)
    {
        return NULL;
    }

    if (!strncmp(haystack, needle, needle_len))
    {
        return (char*)haystack;
    }


    return StrStrRec(haystack + 1, needle);
}

static void InsertSorted(stack_t* s, int val)
{
    if (StackIsEmpty(s) || val >= *(int*)StackPeek(s))
    {
        PushInt(s,val);
        return;
    }
    else
    {
        int temp = PopInts(s);
        InsertSorted(s,val);
        PushInt(s,temp);
    }
}

stack_t* SortedStack(stack_t* s)
{
    if(StackIsEmpty(s))
    {
        return NULL;
    }
    else
    {
        int temp = PopInts(s);
        SortedStack(s);
        InsertSorted(s, temp);
    }

    return s;
}

static int PopInts(stack_t* s)
{
    int value = 0;

    memcpy(&value,StackPeek(s),sizeof(int));

    StackPop(s);

    return value;
}

static void PushInt(stack_t* s, int val)
{
    StackPush(s,(const void*)&val);
}

static void FibMemoInit(void)
{
    size_t i = 0;
    for (i = 0; i < MAX_FIB; ++i)
    {
        g_fib_memo[i] = -1;
    }
}
