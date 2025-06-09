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
	void (*print_func)(int); 
};



int main()
{
	struct print_me arr[10];
	size_t i = 0;
	
	for (i = 0; i < 10 ; i++)
	{
		arr[i].myNum = i * 2;
		arr[i].print_func = Print;
		arr[i].print_func(arr[i].myNum);
	}
	
	return 0;
}
