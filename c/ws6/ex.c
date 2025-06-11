#include "ex.h"
#include <stdlib.h>
#include <math.h>

/******************************
author: Tal Hindi
reviewer: Menny Markovich
status: Approved
*****************************/

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


unsigned int ByteMirrorLoop(unsigned int n)
{
	unsigned int res = 0;	
	while(n)
	{
		res = ( res << 1 ) + ( n % 2);
		n = n >> 1; 
	}	
	return res;
}





























