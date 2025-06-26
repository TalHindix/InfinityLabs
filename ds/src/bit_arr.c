/********************
author: Tal Hindi
reviewer: 
status: 
********************/

#include "../include/bit_arr.h"

#define BITS_IN_BYTE 8

static unsigned char mirror_lut[BYTE_VALUES_LUT_SIZE];
static unsigned char counton_lut[BYTE_VALUES_LUT_SIZE];


/* fill the luts table only once */

static void InitLuts()
{
	static int is_Init = 0; /* flag to ensure only once !*/
	size_t byte_index = 0;
	
	if (is_Init)
	{
		return;
	}
	
	for (byte_index = 0; byte_index < BYTE_VALUES_LUT_SIZE; ++byte_index)
	{
		unsigned char original_byte = (unsigned char)byte_index;
		unsigned char bit_index = 0;
		unsigned char reversed_byte = 0;
		unsigned char ones_in_byte = 0;
		
		for (bit_index = 0; bit_index < BITS_IN_BYTE; ++bit_index)
		{
			reversed_byte = (unsigned char)((reversed_byte << 1) | (original_byte & 1));
            ones_in_byte  = (unsigned char)(ones_in_byte + (original_byte & 1));
            original_byte >>= 1;
		}
		
		mirror_lut[byte_index]  = reversed_byte;
        counton_lut[byte_index] = ones_in_byte;		
	}
	
	is_Init = 1;	
	
}

bit_arr_t BitArrMirrorLUT(bit_arr_t bit_arr)
{
	bit_arr_t mirrored = (bit_arr_t)0;
	size_t byte_index = 0;
	size_t total_bytes = sizeof(bit_arr_t);
	
	InitLuts();
	
	for (byte_index = 0; byte_index < total_bytes; ++byte_index)
	{
		/* extract one byte from original word */
		unsigned char curr_byte = (unsigned char)(bit_arr >> (CHAR_BIT * byte_index));
		/* Look up its bits-mirrored form */
		unsigned char part_mirrored = mirror_lut[curr_byte];
		
		size_t dest_shift = CHAR_BIT * (total_bytes - 1 - byte_index);
		
		mirrored |= (bit_arr_t)part_mirrored << dest_shift;
	}
	
	return mirrored;
}

size_t BitArrCountOnLUT(bit_arr_t value)
{
    size_t ones_total = 0;
    size_t byte_index = 0;
    size_t total_bytes = sizeof(bit_arr_t);

    InitLuts();

    for (byte_index = 0; byte_index < total_bytes; ++byte_index)
    {
        unsigned char curr_byte = (unsigned char)(value >> (CHAR_BIT * byte_index));
        ones_total += counton_lut[curr_byte];
    }

    return ones_total;
}

bit_arr_t BitArrSetAllOn(bit_arr_t bit_arr)
{
	(void)bit_arr;
	return (bit_arr_t)~0;
}


bit_arr_t BitArrSetAllOff(bit_arr_t bit_arr)
{
	(void)bit_arr;
	return (bit_arr_t)0;
}

bit_arr_t BitArrSetBit(bit_arr_t bit_arr, size_t index, int value)
{

	bit_arr_t mask = (bit_arr_t)1 << index;
	
	assert(index < BITARR_BITS);		
	assert(value == 1 || value == 0);
	
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
	
	assert(index < BITARR_BITS);
	
	mask = (bit_arr_t)1 << index;
	
	return (bit_arr & mask) ? 1 : 0;
}

bit_arr_t BitArrSetOn(bit_arr_t bit_arr, size_t index)
{
	bit_arr_t mask = (bit_arr_t)0;
	
	assert(index < BITARR_BITS);
	
	mask = (bit_arr_t)1 << index;
	
	return bit_arr | mask;
}

bit_arr_t BitArrSetOff(bit_arr_t bit_arr, size_t index)
{
	bit_arr_t mask = (bit_arr_t)0;
	
	assert(index < BITARR_BITS);
	
	mask = (bit_arr_t)1 << index;
	
	return bit_arr & ~mask;
}

bit_arr_t BitArrRotateLeft(bit_arr_t bit_arr, size_t shift)
{
	bit_arr_t left_part = 0;
	bit_arr_t wrap_part = 0;
	bit_arr_t rotated_completed = 0;
	
	shift %= BITARR_BITS;
	
	if (shift == 0)
	{
		return bit_arr;
	}
	
	/* Shifting left */
	left_part = bit_arr << shift;
	
	/* wrap part */
	wrap_part = bit_arr >> (BITARR_BITS - shift);
	
	/* rotated */
	rotated_completed = left_part | wrap_part;
		
	return rotated_completed; 
}

bit_arr_t BitArrRotateRight(bit_arr_t bit_arr, size_t shift)
{
	bit_arr_t right_part = 0;
	bit_arr_t wrap_part = 0;
	bit_arr_t rotated_completed = 0;
	
	shift %= BITARR_BITS;
	
	if (shift == 0)
	{
		return bit_arr;
	}
	
	/* Shifting right */
	right_part = bit_arr >> shift;
	
	/* wrap part */
	wrap_part = bit_arr << (BITARR_BITS - shift);
	
	/* rotated */
	rotated_completed = right_part | wrap_part;
		
	return rotated_completed; 
}
bit_arr_t BitArrFlipBit(bit_arr_t bit_arr, size_t index)
{
	bit_arr_t mask = (bit_arr_t)0;

    assert(index < BITARR_BITS);

    mask = (bit_arr_t)1 << index;

    return bit_arr ^ mask;
}

size_t BitArrCountOn(bit_arr_t bit_arr)
{
	size_t count = 0;
	
	while (bit_arr > 0)
	{
		++count;
		bit_arr &= (bit_arr - 1); /* turn off the lowest number thats the bit on */
	}
	
	return count;
}

size_t BitArrCountOff(bit_arr_t bit_arr)
{
	return BITARR_BITS - BitArrCountOn(bit_arr);
}

bit_arr_t BitArrMirror(bit_arr_t bit_arr)
{
    bit_arr_t mirror_bit_arr = (bit_arr_t)0;
    size_t i;

    for (i = 0; i < BITARR_BITS; ++i)
    {

		if (BitArrGetBit(bit_arr, i))
		{
			mirror_bit_arr = BitArrSetOn(mirror_bit_arr, BITARR_BITS - 1 - i);
		}
     
    }
    return mirror_bit_arr;
}

char *BitArrToString(bit_arr_t bit_arr, char *str)
{
    size_t i;
	char bit = ' ';
	
    if (NULL == str)
    {
        return NULL;                      
    }
	
	/* Walk from MSB to LSB and write '1' or '0' */
    for (i = 0; i < BITARR_BITS; ++i)
    {
    
     	bit = BitArrGetBit(bit_arr,i) ? '1' : '0';              
        str[BITARR_BITS - 1 - i] = bit;
    
    }   
     
    str[BITARR_BITS] = '\0';                    
    
    return str;
}


