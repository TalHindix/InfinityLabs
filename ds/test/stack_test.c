/********************
author: Tal Hindi
reviewer: Menachem Markovich 
status: Approved

*******************/

#include "stack.h"

#include <stdio.h>

/*--------------------------------------------------*/
static int TestCreate()
{
    int pass = 1;
    stack_t *st = StackCreate(3, sizeof(int));

    if (st != NULL)
    {
        printf("Create : PASS\n");
    }
    else
    {
        printf("Create : FAIL\n");
        pass = 0;
    }

    if (StackCapacity(st) == 3)
    {
        printf("Capacity : PASS\n");
    }
    else
    {
        printf("Capacity : FAIL\n");
        pass = 0;
    }

    if (StackSize(st) == 0)
    {
        printf("Init size : PASS\n");
    }
    else
    {
        printf("Init size : FAIL\n");
        pass = 0;
    }

    if (StackIsEmpty(st) == 1)
    {
        printf("IsEmpty : PASS\n");
    }
    else
    {
        printf("IsEmpty : FAIL\n");
        pass = 0;
    }

    StackDestroy(st);
    return pass;
}

/*--------------------------------------------------*/
static int TestPushPeek()
{
    int pass = 1;
    stack_t *st = StackCreate(3, sizeof(int));
    int a = 10, b = 20, c = 30;

    StackPush(st, &a);
    if (StackSize(st) == 1)
    {
        printf("Push1 size : PASS\n");
    }
    else
    {
        printf("Push1 size : FAIL\n");
        pass = 0;
    }

    if (*(int *)StackPeek(st) == 10)
    {
        printf("Peek1 : PASS\n");
    }
    else
    {
        printf("Peek1 : FAIL\n");
        pass = 0;
    }

    StackPush(st, &b);
    StackPush(st, &c);

    if (StackSize(st) == 3)
    {
        printf("Push3 size : PASS\n");
    }
    else
    {
        printf("Push3 size : FAIL\n");
        pass = 0;
    }

    if (*(int *)StackPeek(st) == 30)
    {
        printf("Peek3 : PASS\n");
    }
    else
    {
        printf("Peek3 : FAIL\n");
        pass = 0;
    }

    StackDestroy(st);
    return pass;
}

/*--------------------------------------------------*/
static int TestPop()
{
    int pass = 1;
    stack_t *st = StackCreate(3, sizeof(int));
    int a = 10, b = 20;

    StackPush(st, &a);
    StackPush(st, &b);

    StackPop(st);
    if (StackSize(st) == 1)
    {
        printf("Pop1 size : PASS\n");
    }
    else
    {
        printf("Pop1 size : FAIL\n");
        pass = 0;
    }

    if (*(int *)StackPeek(st) == 10)
    {
        printf("Peek after pop : PASS\n");
    }
    else
    {
        printf("Peek after pop : FAIL\n");
        pass = 0;
    }

    StackPop(st);
    if (StackIsEmpty(st) == 1)
    {
        printf("Empty after pops : PASS\n");
    }
    else
    {
        printf("Empty after pops : FAIL\n");
        pass = 0;
    }

    StackDestroy(st);
    return pass;
}

/*--------------------------------------------------*/
int main()
{
    int overall_pass = 1;

    if (TestCreate())
    {
        printf("TestCreate : PASS\n");
    }
    else
    {
        printf("TestCreate : FAIL\n");
        overall_pass = 0;
    }

    if (TestPushPeek())
    {
        printf("TestPushPeek : PASS\n");
    }
    else
    {
        printf("TestPushPeek : FAIL\n");
        overall_pass = 0;
    }

    if (TestPop())
    {
        printf("TestPop : PASS\n");
    }
    else
    {
        printf("TestPop : FAIL\n");
        overall_pass = 0;
    }

    if (overall_pass)
    {
        printf("=== ALL TESTS PASS ===\n");
    }
    else
    {
        printf("=== SOME TESTS FAILED ===\n");
    }

    return overall_pass ? 0 : 1;
}
