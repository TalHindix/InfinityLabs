#include "ex.h"

/******************************
author: Tal Hindi
reviewer: Menny Markovich
status:
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

int AddOne ( unsigned int n )
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

void PrintWithThreeBits (unsigned int *arr)
{
		
	
	
	

}



























