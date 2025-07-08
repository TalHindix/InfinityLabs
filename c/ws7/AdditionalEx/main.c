#include "g.h"
#include <stdio.h>







int main()
{
	printf("Value g_s before: %d\n",g_s);
	Foo();
	printf("Value g_s After: %d\n",g_s);
	return 0;

}
