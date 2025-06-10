/**********************
author: Tal Hindi
reviewer: Menny Markovich
status:
**********************/

#include <stdio.h>



void Print(int value)
{
	printf("%d\n",value);
}


struct print_me
{
	int myNum;
	void (*PrintPtr)(int); 
};



int main()
{
	struct print_me arr[10];
	size_t i = 0;
	
	for (i = 0; i < 10 ; i++)
	{
		arr[i].myNum = i * 2;
		arr[i].PrintPtr = Print;
		arr[i].PrintPtr(arr[i].myNum);
	}
	
	return 0;
}
