#include <stdio.h>


#include "ex.c"
#include "ex.h"
#include <math.h>

#define SIZE 10


void TestEx1()
{
	double x = 0;
    double y = 0;
    
    x=2;
    y=3;
       
    if(Pow2(x,y) == x*pow(2,y))
    {
    	printf("SUCCESS\n");
    }
    else
    {
    	printf("FAIL\n");
    }

	x = 0;
	y = 0;
	
	if(Pow2(x,y) == x*pow(2,y))
    {
    	printf("SUCCESS\n");
    }
    else
    {
    	printf("FAIL\n");
    }
	
	
}

void TestEx2_loop()
{
	
}

void TestEx2_NoLoop()
{

}

void TestEx3()
{

}


int main()
{
	unsigned int num;
	/*
	unsigned int *arr = (unsigned int*)malloc(sizeof(unsigned int) * SIZE);

	if (arr == NULL)
	{
		printf("Memory not allocated \n");
		return 1;
	}
	
	arr[0] = 7;
	arr[1] = 3;
	arr[2] = 11;
	arr[3] = 23;
	
	

	PrintWithThreeBits(arr,SIZE);
	
	free(arr);
	*/
	TestEx1();
	
	num = ByteMirrorLoop(1);
	
	printf("%d\n",num);
	
	

	
	
	
	return 0;
}
