#include <stdio.h>
#include <string.h>
#include <stddef.h>



void TF(int num)
{
    int i;
    for (i = 1; i < num; i++)
    {
        if (0 == i % 5)
        {
            printf("num:%d = %c\n", i, 'T');
        }
        else if (0 == i % 3)
        {
            printf("num:%d = %c\n", i, 'F');
        }
        else if (0 == i % 5 && 0 == i % 3)
        {
            printf("num:%d = %s\n", i, "TF");
        }
        else
        {
            printf("num:%d\n", i);
        }
    }
}

void ReverseString(char *s)
{
    char *l;
    char *r;
    char temp;

    l = s;
    r = s+ strlen(s) - 1;

    while (l <= r)
    {
        if (*l >= 'A' && *l <= 'Z')
        {
            *l += 32;
        }
        if (*r >= 'A' && *r <= 'Z')
        {
            *r += 32;
        }

        temp = *l;
        *l = *r;
        *r = temp;

        l++;
        r--;
    }
}



int main()
{
	char string[] = "Hello worlD";
	size_t i;
	/*Q1*/
	TF(15);
	
	/*Q2*/
	ReverseString(c);
	for(i = 0; i < strlen(string); i++)
  	{
    	printf("%c",*(string+i));
 	}
  	printf("\n");
  	
	
	

	return 1;
}
