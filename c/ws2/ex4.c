#include <stdio.h>

/*******************
author: Tal Hindi
reviewer: Baruch Haimson
status: approved

********************/

void SwapSizeT(size_t *a, size_t *b)
{
	size_t temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void SwapSizeTPointers(size_t **a, size_t **b)
{
	SwapSizeT((size_t*)a,(size_t*)b);
}



int main()
{
	size_t x = 5;
	size_t y = 3;
	
	size_t* ppx = &x;
	size_t* ppy = &y;
	
	printf("The addresses before swap x:%lu , y:%lu\n", (unsigned long)ppx, (unsigned long)ppy);
	SwapSizeTPointers(&ppx,&ppy);
	printf("The addresses after swap x:%lu , y:%lu\n", (unsigned long)ppx, (unsigned long)ppy);
	return 0;
}
