/********************
author: Tal Hindi
reviewer: 
status: 
********************/

#include "../include/bit_arr.h"


bit_arr_t BitArrSetAllOn(bit_arr_t bit_arr)
{
	return bit_arr | (bit_arr_t)~0;
}


bit_arr_t BitArrSetAllOff(bit_arr_t bit_arr)
{
	return bit_arr & (bit_arr_t)0;
}

bit_arr_t BitArrSetBit(bit_arr_t bit_arr, size_t index, int value)
{

	bit_arr_t mask = (bit_arr_t)1 << index;
	
	assert (BITARR_BITS > index);		
	
	if (value) /* 1 */
	{
		bit_arr |= mask;
	}
	else       /* 0 */
	{
		bit_arr &= ~mask; /* '~' make the mask index to zero than we make '&' with zero. */
	}
	
	return bit_arr;
}

int BitArrGetBit(bit_arr_t bit_arr, size_t index)
{
	bit_arr_t mask = (bit_arr_t)0;
	
	assert (BITARR_BITS > index);
	
	mask = (bit_arr_t)1 << index;
	
	return (bit_arr & mask) ? 1 : 0;
}

bit_arr_t BitArrSetOn(bit_arr_t bit_arr, size_t index)
{
	bit_arr_t mask = (bit_arr_t)0;
	
	assert (BITARR_BITS > index);
	
	mask = (bit_arr_t)1 << index;
	
	return bit_arr | mask;
}

bit_arr_t BitArrSetOff(bit_arr_t bit_arr, size_t index)
{
	bit_arr_t mask = (bit_arr_t)0;
	
	assert (BITARR_BITS > index);
	
	mask = (bit_arr_t)1 << index;
	
	return bit_arr & ~mask;
}

bit_arr_t BitArrRotateLeft(bit_arr_t bit_arr, size_t shift)
{
		
}
bit_arr_t BitArrRotateRight(bit_arr_t bit_arr, size_t shift)
{
	
}
bit_arr_t BitArrFlipBit(bit_arr_t bit_arr, size_t index)
{

}


