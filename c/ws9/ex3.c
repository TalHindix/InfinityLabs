/***************
author: Tal Hindi
reviewer: 
status: 

****************/

#include <stdio.h>



int Is_Little_Endian()
{
	int x = 1; /* 0x00000001 */
	char *ptr = (char *)&x; /* Look at the first byte */
	
	return (*ptr == 1);

}


int main()
{

	if (Is_Little_Endian())
    {
        printf("System is little-endian.\n");
    }
    else
    {
        printf("System is big-endian.\n");
    }
	
	


	return 0;
}
