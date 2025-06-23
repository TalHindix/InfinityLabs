/*******************
author: Tal Hindi
reviewer: 
status:

*******************/

#include <stdio.h>

static const unsigned int one = 1;

#define IS_LITTLE_ENDIAN() (*((unsigned char *)&one) == 1)


int main()
{

	if (IS_LITTLE_ENDIAN())
    {
        printf("System is little-endian.\n");
    }
    else
    {
        printf("System is big-endian.\n");
    }


	return 0;
}
