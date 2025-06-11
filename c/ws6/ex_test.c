#include <stdio.h>
#include "ex.c"
#include "ex.h"

#define SIZE 10

void TestEx1() /* Test Pow2() */
{
    printf("************Running TestEx1**************\n");

    if (Pow2(0, 10) == 0) /* 0 case */
    {
    	printf("Test 1: SUCCESS\n");
    }
    else
    {
    	printf("Test 1: FAIL\n");
    }
    
    if (Pow2(1, 0) == 1)
    {
    	printf("Test 2: SUCCESS\n");
    }
    else
    {
    	printf("Test 2: FAIL\n");
    }
    
    if (Pow2(3, 2) == 12)
    {
    	printf("Test 3: SUCCESS\n");
    }   
    else
    {
    	printf("Test 3: FAIL\n");
    }
    
    if (Pow2(7, 1) == 14)
    {
    	printf("Test 4: SUCCESS\n");
    }
    else 
    {
    	printf("Test 4: FAIL\n");
    }
    
    if (Pow2(1, 31) == 2147483648UL)
    {
    	printf("Test 5: SUCCESS\n");
    }
    else 
    {
    	printf("Test 5: FAIL\n");
    }

    printf("\n");
}

void TestEx2_Loop() /* Test IsPowerOfTwoLoop() */
{
    printf("******************Running TestEx2******************\n");

    if (IsPowerOfTwoLoop(0) == 0)
    {
    	printf("Test 1: SUCCESS\n");
    }
    else
    {
    	printf("Test 1: FAIL\n");
    }
    
    if (IsPowerOfTwoLoop(1) == 1)
    {
    	printf("Test 2: SUCCESS\n");
    }
    else
    {
    	printf("Test 2: FAIL\n");
    }
    
    if (IsPowerOfTwoLoop(8) == 1)
    {
    	printf("Test 3: SUCCESS\n");
    }
    else
    { 
    	printf("Test 3: FAIL\n");
    }
    
    if (IsPowerOfTwoLoop(9) == 0)
    {
    	printf("Test 4: SUCCESS\n");
    }
    else
    {
    	printf("Test 4: FAIL\n");
    }
    
    printf("\n");
}


void TestEx2_NoLoop() /* Test IsPowerOfTwo() */
{
    printf("******************Running TestEx3******************\n");

    if (IsPowerOfTwo(0) == 0)
    {
    	printf("Test 1: SUCCESS\n");
    }
    else
    {
    	printf("Test 1: FAIL\n");
    }
    
    if (IsPowerOfTwo(1) == 1)
    {
    	printf("Test 2: SUCCESS\n");
    }
    else
    {
    	printf("Test 2: FAIL\n");
    }
    
    if (IsPowerOfTwo(8) == 1)
    {
    	printf("Test 3: SUCCESS\n");
    }
    else
    {
    	printf("Test 3: FAIL\n");
    }
    
    if (IsPowerOfTwo(9) == 0)
    {
    	printf("Test 4: SUCCESS\n");
    }
    else
    {
    	printf("Test 4: FAIL\n");
    }
    printf("\n");
}

void TestEx3_AddOne() /* Test AddOne */
{
    printf("******************Running TestEx3******************\n");

    if (AddOne(0) == 1)
    {
    	printf("Test 1: SUCCESS\n");
    }
    else
    {
    	printf("Test 1: FAIL\n");
    }
    
    if (AddOne(5) == 6)
    {
    	printf("Test 2: SUCCESS\n");
    }
    else
    {
    	printf("Test 2: FAIL\n");
    }
    
    if (AddOne(-1) == 0)
    {
    	printf("Test 3: SUCCESS\n");
    }
    else
    {
    	printf("Test 3: FAIL\n");
    }
    
    if (AddOne(2147483646) == 2147483647)
    {
    	printf("Test 4: SUCCESS\n");
    }
    else
    {
    	printf("Test 4: FAIL\n");
    }

    printf("\n");
}

void TestEx4_PrintWithThreeBits()
{
    unsigned int arr1[] = {7, 11, 13, 14, 28, 56, 224}; /* Expected to print */
    unsigned int arr2[] = {1, 2, 3, 15, 255, 0};        /* Should not print */

    printf("******************Running TestEx4****************** (should print only values with 3 bits on settt)\n");

    printf("Array 1 (should print):\n");
    PrintWithThreeBits(arr1, sizeof(arr1) / sizeof(arr1[0]));

    printf("\nArray 2 (should not print):\n");
    PrintWithThreeBits(arr2, sizeof(arr2) / sizeof(arr2[0]));

    printf("\n");
}

int main()
{
    TestEx1();
    TestEx2_Loop();
    TestEx2_NoLoop();
    TestEx3_AddOne();
    TestEx4_PrintWithThreeBits();

    return 0;
}

