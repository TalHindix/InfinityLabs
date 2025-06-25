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

void TestBitArrMirrorLUT()
{
	bit_arr_t val = 0x00000000000000F0;
	bit_arr_t expected = 0x0F00000000000000;
	bit_arr_t result = BitArrMirrorLUT(val);

	printf("\n------------ TestBitArrMirrorLUT ------------\n");


	if (result == expected)
	{
		printf("Mirror of 0x%lX: PASS\n", val);
	}
	else
	{
		printf("Mirror of 0x%lX: FAIL (got 0x%lX, want 0x%lX)\n", val, result, expected);
	}
}

void TestBitArrCountOnLUT()
{
	bit_arr_t val = 0xF0F0F0F0F0F0F0F0;
	size_t expected = 32;
	size_t result = BitArrCountOnLUT(val);

	printf("\n------------ TestBitArrCountOnLUT -----------\n");


	if (result == expected)
	{
		printf("CountOn of 0x%lX: PASS\n", val);
	}
	else
	{
		printf("CountOn of 0x%lX: FAIL (got %lu, want %lu)\n", val, result, expected);
	}
}



int main()
{
	TestBitArrSetAllOn();
	TestBitArrSetAllOff();
	TestBitArrMirrorLUT();
	TestBitArrCountOnLUT();
	
	return 0;
}
