#include "globals.h"
#include <stdio.h>

int x = 5;

void PrintGlobalVariableAddress(void)
{
	printf("The address of global variable x is:%p\n", &x);
}

