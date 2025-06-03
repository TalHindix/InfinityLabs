#include <stdio.h> /*printf*/


/**********************
author: Tal Hindi
reviewer: Baruch Haimson
status:	approved

***********************/

void Swap(int *x, int *y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}


int main()
{
	int x,y;
	x = 5;
	y = 3;
	
	printf("Values before swap x:%d y:%d \n",x,y);
	Swap(&x,&y);
	printf("Values after swap x:%d y:%d \n",x,y);
	
	return 0;
}
