#include "ex.h"
#include <stdlib.h>
#include <stdio.h>

/******************************
 Exercise: 	WS6 bitwise ex.
 Date: 		11/06/25
 Developer: Tal Hindi
 Reviewer: 	Menny Markovich
 Status: 	Approved
******************************/

#define CHAR_VALUES 256
#define INT_VALUES 256

long Pow2(unsigned int x,unsigned int y)
{
	return (long)x<<y;
}

int IsPowerOfTwoLoop (unsigned int n)
{
	int count = 0;
	
	while ( n > 0)
	{
		if ( n & 1 ) 
		{
			++count;
		}
		n = n >> 1;
	}
	
	return (count == 1);	
}


int IsPowerOfTwo (unsigned int n )
{
	return ( ( n > 0 ) && ( n & (n - 1) ) == 0 );
}

int AddOne ( int n )
{
	int m = 1;
	
	while (n & m) /* until hit the first 0 from the right */
	{
		n = n ^ m; 
		m = m << 1; /* 0001 -> 0010 */
	}
	
	n = n ^ m;
	return n;
}

void PrintWithThreeBits (unsigned int *arr, size_t size)
{
	int temp; /* to store the value */
	int count = 0;
	size_t i = 0;
	
	for (i = 0; i < size; ++i)
	{
		count = 0;
		temp = arr[i];
		
		while ( arr[i] > 0 )
		{
			if ( arr[i] & 1 ) 
			{
				++count;
			}
			arr[i] = arr[i] >> 1;
		};
		
		if ( count == 3 )
		{
			printf("arr[%lu]:%d with 3 bits ! \n",i,temp);
		}
	}
}

/******************************
 Exercise: 	WS6 bitwise ex.
 Date: 		11/06/25
 Developer: Tal Hindi
 Reviewer: 	Menny Markovich
 Status: 	Approved
******************************/

unsigned char ByteMirrorLoop(unsigned char n)
{
	size_t i = 0;
	unsigned char n_reverse = 0;
	
	
	for (i = 0; i < 7; ++i)
	{
		if (n & (1 << i))
		{
			
			n_reverse = (n_reverse | (1 << (7 - i)));
		}
	}
	
	return n_reverse;
}



static unsigned char s_mirror_lut[CHAR_VALUES] = {0};

static void InitMirrorLUT()
{
	size_t i = 0;
	
	for (i = 0; i < CHAR_VALUES; ++i)
	{
		s_mirror_lut[i] = ByteMirrorLoop(i);
	}
} 


unsigned char ByteMirror(unsigned char n)
{
	static int was_init = 0;
	if(!was_init)
	{
		InitMirrorLUT(&s_mirror_lut);
		was_init = 1;
	}
	return s_mirror_lut[n];
}

int Is_2And6_On(unsigned char n)
{
	
	return ((n & (1<<2) && n & (1<<6)));
}

int Is_2_Or_6_On(unsigned char n)
{
	
	return ((n & (1<<2) || n & (1<<6)));
}

int Swap3And5(unsigned char c)
{
	unsigned char bit3 =  (c >> 3) & 1;
	unsigned char bit5 = (c >> 5) & 1;	
	unsigned char mask = bit3 ^ bit5;
	
	c = c ^ ((mask << 3) | (mask << 5));
	   	
	return c;
}

/******************************
 Exercise: 	WS6 bitwise ex.
 Date: 		11/06/25
 Developer: Tal Hindi
 Reviewer: 	Menny Markovich
 Status: 	Approved
******************************/

int RoundDownTo16(unsigned char n)
{
	return n & (~((1 << 4) - 1));
}

void SwapInPlace(int *x , int *y)
{
	*x = *x ^ *y;
	*y = *x ^ *y; 
	*x = *x ^ *y;
}

size_t CountBitsLoop(int n)
{
	size_t count = 0;
	
	while(0 != n)
	{
		n = n & (n - 1);
		count++;
	}
	
	return count;	
}

static int s_countbit_lut[INT_VALUES] = {0};

static void InitCountBitLUT()
{
	size_t i = 0;
	
	for (i = 0; i < INT_VALUES; ++i)
	{
		s_countbit_lut[i] = (i & 1) + s_countbit_lut [i / 2];
	}
}

size_t CountBits(int n)
{
	static int was_init = 0;
	if(!was_init)
	{
		InitCountBitLUT(&s_countbit_lut);
		was_init = 1;
	}
	
	return (s_countbit_lut[n & 0xff] + 
            s_countbit_lut[(n >> 8) & 0xff] + 
            s_countbit_lut[(n >> 16) & 0xff] + 
            s_countbit_lut[n >> 24]); 
}


size_t FloatAnalsys(float f)
{
	
	return
}



































