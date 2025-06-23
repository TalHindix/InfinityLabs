/***************************
author: Tal Hindi
reviewer: 
status:

*****************************/

#include <stdio.h>

#define MAX_CHARS 256


void PrintCharsInArr1_Arr2(const char *arr1 ,const char *arr2 ,const char *arr3)
{
	int in_first_array[MAX_CHARS] = {0};
	int in_second_array[MAX_CHARS] = {0};
	int in_third_array[MAX_CHARS] = {0};
	size_t i = 0;
	
	
	for (i = 0; arr1[i] != '\0'; ++i)
	{
		in_first_array[(unsigned char) arr1[i]] = 1;
	}
	
	for (i = 0; arr2[i] != '\0'; ++i)
	{
		in_second_array[(unsigned char) arr2[i]] = 1;
	}
	
	for (i = 0; arr3[i] != '\0'; ++i)
	{
		in_third_array[(unsigned char) arr3[i]] = 1;
	}
	
	for (i = 0; i < MAX_CHARS; ++i)
    {
        if (in_first_array[i] && in_second_array[i] && !in_third_array[i])
        {
            printf("%c\n", (char)i);
        }
    }
	
}


int main()
{
	char arr_1[] = "blabla";
	char arr_2[] = "blasaa";
	char arr_3[] = "sasa";
	
	PrintCharsInArr1_Arr2(arr_1,arr_2,arr_3);
	

	return 0;
}
