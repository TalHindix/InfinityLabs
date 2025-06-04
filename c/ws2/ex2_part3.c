#include <stdio.h>

int ContainSeven(int num)
{
	if (num < 0)
	{
		num = -num;
	}
	
	while (num > 0)
	{
		if (num % 10 == 7)
		{
			return 1;
		}
		num /= 10;
	}
	return 0;
}

void SevenBoom(int from,int to)
{
	int i;
	for(i = from; i < to; i++)
	{
		if (i % 7 == 0 || ContainSeven(i))
		{
			printf("BOOM!\n");
		}
		else
		{
			printf("%d\n",i);
		}
	}
}




int main()
{
	SevenBoom(-15,18);

	return 0;
}
