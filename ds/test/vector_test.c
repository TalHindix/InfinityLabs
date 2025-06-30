/****************
author: Tal Hindi
reviewer:
status:

****************/


#include <stdio.h>
#include <string.h>
#include <stddef.h>  
#include "../include/vector.h"


static void TestCreate();
static void TestPushAndAccess();
static void TestAutoGrowth();
static void TestPopAndShrink();
static void TestReserveAndShrink();
static void PrintResult(const char *name, int condition);


int main()
{
    TestCreate();
    TestPushAndAccess();
    TestAutoGrowth();
    TestPopAndShrink();
    TestReserveAndShrink();

    return 0;
}


static void PrintResult(const char *name, int condition)
{
    if (condition)
    {
        printf("%s : PASS\n", name);
    }
    else
    {
        printf("%s : FAIL\n", name);
    }
}


static void TestCreate()
{
    vector_t *vec = VectorCreate(0, sizeof(int));

    PrintResult("Create-capacity default",VectorCapacity(vec) == DEFAULT_CAPACITY);
    PrintResult("Create-size zero",0 == VectorSize(vec));

    VectorDestroy(vec);
}

static void TestPushAndAccess()
{
    vector_t *vec = VectorCreate(4, sizeof(int));
    int is_ok = 1;
    size_t i = 0;

    for (i = 0; i < 4; ++i)
    {
        VectorPushBack(vec, &i);
    }
    if (VectorSize(vec) != 4)
    {
        is_ok = 0;
    }

    for (i = 0; i < 4; ++i)
    {
        int *p = (int *)VectorGetAccessToElement(vec, i);
        if (*p != (int)i)
        {
            is_ok = 0;
        }
    }

    PrintResult("Push + Access", is_ok);
    VectorDestroy(vec);
}


static void TestAutoGrowth()
{
    vector_t *vec = VectorCreate(0, sizeof(int));
    int a = 1, b = 2, c = 3;
    size_t before_cap, after_cap;

    VectorPushBack(vec, &a);
    VectorPushBack(vec, &b);
    VectorPushBack(vec, &b);
    VectorPushBack(vec, &b);
    VectorPushBack(vec, &b);
    VectorPushBack(vec, &b);
    VectorPushBack(vec, &b);
	VectorPushBack(vec, &b);
    before_cap = VectorCapacity(vec);

    VectorPushBack(vec, &c);          
    after_cap = VectorCapacity(vec);

    PrintResult("Auto-growth doubled",after_cap == before_cap * 2);
    VectorDestroy(vec);
}

static void TestPopAndShrink()
{
    vector_t *vec = VectorCreate(8, sizeof(int));
    size_t i = 0;

    for (i = 0; i < 8; ++i)
    {
        VectorPushBack(vec, &i);
    }
    for (i = 0; i < 6; ++i)         
    {
        VectorPopBack(vec);
    }

    PrintResult("Pop-shrink halved",VectorCapacity(vec) == 8);
    VectorDestroy(vec);
}

static void TestReserveAndShrink()
{
    vector_t *vec = VectorCreate(10, sizeof(int));
    int is_ok = 1;

    VectorReserve(vec, 20);                 
    if (VectorCapacity(vec) != 20)
    {
        is_ok = 0;
    }

    VectorReserve(vec, 5);                     
    if (VectorCapacity(vec) < DEFAULT_CAPACITY)
    {
        is_ok = 0;
    }

    VectorShrink(vec);                    
    if (VectorCapacity(vec) != DEFAULT_CAPACITY)
    {
        is_ok = 0;
    }

    PrintResult("Reserve + Shrink", is_ok);
    VectorDestroy(vec);
}

