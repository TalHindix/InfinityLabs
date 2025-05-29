#include <stdio.h> /* printf */

/*************************

author:Tal Hindi
reviewer:Baruch Haimson
status:

*************************/


int FlipOrdDigits(int num)
{
	int result;
	int digit;
	
	result = 0;
	
	while(num)
	{
		digit = num % 10;
		result = (result * 10) + digit;
		num /= 10;
	}
	
	return result;
}

int main() 
{	
	printf("The flip order number is: %d\n",FlipOrdDigits(12342)); /*ODD*/
	printf("The flip order number is: %d\n",FlipOrdDigits(123400)); /*EVEN Include 0*/
	printf("The flip order number is: %d\n",FlipOrdDigits(-12340)); /*Negative*/
    return 0;
}

