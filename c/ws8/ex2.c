/*************************
author: Tal Hindi
reviewer:
status: 
**************************/




#include <stdio.h>

#define MAX2(X,Y) ((X) > (Y) ? (X) : (Y))
#define MAX3(X, Y, Z) ( MAX2( MAX2((X), (Y)), (Z) ) )




int main()
{
	int a = 5;
	int b = 3;
	int c = 2;
	
	
	printf("MAX2: %d\n", MAX2(a, b));      
    printf("MAX3: %d\n", MAX3(a, b, c));
	
	return 0;
}
