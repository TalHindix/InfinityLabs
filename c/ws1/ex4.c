#include <stdio.h> /* printf */

/*****************

author:tal hindi
reviewer:baruch haimson
status:

*****************/

double Pow(int n)
{
	double result = 1.0;
	int i;
	
	int abs_n = ( n < 0 ? -n : n)
	for (i = 0; i < abs_n; i++ )
	{
		result = result * 10;
	}
	
	if (n<0)
	{
		result = 1 / result;
	}
	
	return result;
}

int main() {

	printf("%f\n",Pow(5));
	printf("%f\n",Pow(-7)); 	
	printf("%f\n",Pow(0)); 
    return 0;
}


