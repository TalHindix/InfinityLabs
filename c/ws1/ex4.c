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
	
	for (i = 0; i < ( n < 0 ? -n : n); i++ )
	{
		result = (n >= 0) ? result * 10 : result / 10;
	}
	return result;
}

int main() {

	printf("%f\n",Pow(5));
	printf("%f\n",Pow(-5)); 	
	printf("%f\n",Pow(0)); 
    return 0;
}


