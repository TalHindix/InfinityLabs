/*********************
author: Tal Hindi
reviewer: 
status:
*********************/

#include "../include/bit_arr.h" /* */
#include <stdio.h>


void TestBitArrSetAllOn()
{
	size_t i = 0;
	bit_arr_t bitarr[5] = {0 , 12 , 256 , 0 , 0};
	bit_arr_t expected[5] = {~(bit_arr_t)0,~(bit_arr_t)0,~(bit_arr_t)0,~(bit_arr_t)0,~(bit_arr_t)0};
	
	/* Iterate over bitarr and setallon for each element. */
	for (i = 0; i < 5; ++i)
	{
		bitarr[i] = BitArrSetAllOn(bitarr[i]);
	}
	
	printf("\n------------ TestBitArrSetAllOn -------------\n");	
	
	for (i = 0; i < 5; ++i)
	{
		if (bitarr[i] == expected [i])
		{
			printf("Index %lu : PASS\n",i);	
		}
		else
		{
			printf("Index %lu : FAIL (got 0x%lX, want 0x%lX)\n",i,bitarr[i],expected[i]);
		}
	}
}

void TestBitArrSetAllOff()
{
	size_t i = 0;
	bit_arr_t bitarr[5] = {5 , 12 , 5 , 6 , 259};
	bit_arr_t expected[5] = {0,0,0,0,0};
	
	/* Iterate over bitarr and setallon for each element. */
	for (i = 0; i < 5; ++i)
	{
		bitarr[i] = BitArrSetAllOff(bitarr[i]);
	}
	
	printf("\n------------ TestBitArrSetAllOff -------------\n");	
	
	for (i = 0; i < 5; ++i)
	{
		if (bitarr[i] == expected [i])
		{
			printf("Index %lu : PASS\n",i);	
		}
		else
		{
			printf("Index %lu : FAIL (got 0x%lX, want 0x%lX)\n",i,bitarr[i],expected[i]);
		}
	}
}




int main()
{
	TestBitArrSetAllOn();
	TestBitArrSetAllOff();
	
	return 0;
}
