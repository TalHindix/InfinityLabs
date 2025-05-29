#include <stdio.h> /*printf*/
/****************************
author:Tal Hindi
reviewer:Baruch Haimson
status:

*****************************/


void Swap(int *x , int *y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}



int main()
{
	int x,y;
	x=5,y=7;
	printf("Values before swap X:%d Y:%d\n",x,y);
	Swap(&x,&y);
	printf("Values after swap X:%d Y:%d\n",x,y);

	return 0;
}

