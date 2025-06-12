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

void TestEx5()
{
	unsigned char num = 1;
	unsigned char result = ByteMirror(num);
	
	printf("******************Running TestEx5_NOLOOP******************\n");
	printf("%d\n",result);
}

void TestEx5_Loop()
{
	unsigned char num = 1;
	unsigned char result = ByteMirrorLoop(num);
	
	printf("******************Running TestEx5_LOOP******************\n");
	printf("%d\n",result);
}

void TestEx6()
{
	unsigned char n = 67;
	int result = Is_2And6_On(n);
	printf("******************Running TestEx6_2_AND_6******************\n");
	printf("%d \n",result);
}

void TestEx6_Swap()
{
	unsigned char n = 8;
	int result = Swap3And5(n);
	printf("******************Running TestEx6_SWAP******************\n");
	printf("%d \n",result);
}

void TestEx7()
{
	unsigned char n = 33;
	int result = RoundDownTo16(n);
	printf("******************Running TestEx7******************\n");
	printf("%d \n",result);
}

void TestEx8()
{
	int a = 3;
	int b = 5;
	printf("******************Running TestEx8******************\n");
	
	printf("before swap ->  a:%d , b:%d\n",a,b);
	SwapInPlace(&a,&b);
	printf("before swap ->  a:%d , b:%d\n",a,b);
}

void TestEx9()
{
	int num = 16;
	size_t result = CountBitsLoop(num);
	printf("******************Running TestEx9******************\n");
	
	printf("There is %lu bits SET (ON) in number:%d!\n",result,num);
}

void TestEx9_2()
{
	int num = 16;
	size_t result = CountBits(num);
	printf("******************Running TestEx9_LUT ******************\n");
	printf("There is %lu bits SET (ON) in number:%d!\n",result,num);
}

void TestEx10()
{
	float f = 3.0;
	size_t result = FloatAnalsys(&f);
	printf("******************Running TestEx10 ******************\n");
	printf("Ex10: %lx \n",result); /* Print In Hexadecimal */
}


int main()
{
/*
    TestEx1();
    TestEx2_Loop();
    TestEx2_NoLoop();
    TestEx3_AddOne();
    TestEx4_PrintWithThreeBits();
    TestEx5_Loop();
    TestEx5();       
    TestEx6();
	TestEx6_Swap();
	TestEx7();
	TestEx8();
	TestEx9();
	TestEx9_2();	
*/
	TestEx10();
	
    return 0;
}

