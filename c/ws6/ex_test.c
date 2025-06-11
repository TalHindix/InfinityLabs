#include <stdio.h>


#include "ex.c"
#include "ex.h"
#include <Math.h>

void TestEx1(int x, int y)
{
	unsigned long result = Pow2(x,y);
	printf("---------ex1-------------Result of ex1 :%lu---------ex1-------------\n",result);
}



int main()
{
	unsigned int n = 7;
	int n1 = 16;
	
	TestEx1(2,3);
	
	
	if ( IsPowerOfTwoLoop(n) )
	{
		printf("--------ex2-----------The number %d is a power of 2------------ex2------------\n",n);
	}
	else
	{
		printf("--------ex2-----------The number %d is NOT a power of 2-------------ex2-----------\n",n);
	}
	
	if ( IsPowerOfTwoLoop(n1) )
	{
		printf("--------ex2-----------The number %d is a power of 2------------ex2------------\n",n1);
	}
	else
	{
		printf("--------ex2-----------The number %d is NOT a power of 2-------------ex2-----------\n",n1);
	}
	
	if ( IsPowerOfTwo(n) )
	{
		printf("--------ex2-----------The number %d is a power of 2------------ex2------------\n",n);
	}
	else
	{
		printf("--------ex2-----------The number %d is NOT a power of 2-------------ex2-----------\n",n);
	}
	
	if ( IsPowerOfTwo(n1) )
	{
		printf("--------ex2-----------The number %d is a power of 2------------ex2------------\n",n1);
	}
	else
	{
		printf("--------ex2-----------The number %d is NOT a power of 2-------------ex2-----------\n",n1);
	}
	
	printf("Before add one: %d\n",n);
	AddOne(&n);
	printf("after add one: %d\n",n);
	
		
	
	
	
	
	
	return 0;
}
