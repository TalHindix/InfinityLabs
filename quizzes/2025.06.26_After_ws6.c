#include <stdlib.h>
#include <stdio.h>



size_t CountSetBit(unsigned char byte)
{
	size_t count = 0;
	size_t i = 0;
	
	for (i = 0; i < 7; ++i)
	{
		count += (0 != (byte & (1 << i))) && (0 != (byte & (1 << (i + 1))));
	}
	
	return count;
}

void StandardSwap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void XorSwap(int* a, int* b)
{
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *b ^ *a;
}

void ArithmeticSwap(int* a, int* b)
{
	*a = *a - *b;
	*b = *a + *b;
	*a = *b - *a;
}

size_t CountBitsOn(unsigned long num)
{
	size_t count = 0;
	
	while (num)
	{
		num &= (num - 1);
		++count;
	}
	
	return count;
}



void PrintInts(int* a, int* b)
{
	printf("a: %d	b: %d\n", *a, *b); 
}


int main()
{
	int a = 1;
	int b = 2;
	
	PrintInts(&a, &b);
	StandardSwap(&a, &b);
	PrintInts(&a, &b);
	printf("**************************\n");
	PrintInts(&a, &b);
	ArithmeticSwap(&a, &b);
	PrintInts(&a, &b);
	printf("**************************\n");
	PrintInts(&a, &b);
	XorSwap(&a, &b);
	PrintInts(&a, &b);

	return 0;
}




