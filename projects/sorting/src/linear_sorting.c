/**************************************
Exercise: 	SA - Linear Sorting
Date:		6/8/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		
**************************************/

#include "linear_sorting.h"

#include <stdlib.h> /* malloc */
#include <stdio.h>	/* printf */

#define BASE 10

static int GetMax(int* arr, size_t size);
static void CountingSortDigit(int* arr, size_t size, int exp);

void CountingSort(int* arr, size_t size)
{
	long max = 0;
	long min = 0;
	size_t range = 0;
	size_t index = 0;
	size_t i = 0;
	int* count_arr = NULL;
	int* out_put = NULL;
	
		
	max = min = arr[0];
	
	for(i = 1; i < size; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
		if (arr[i] < min)
		{
			min = arr[i];
		}
	}
	
	range = (size_t)(max - min + 1);
	count_arr = (int*)calloc(range, sizeof(int));
	out_put = (int*)malloc(size * sizeof(int));
	
	if (!count_arr || !out_put)
	{
		printf("Memory allocated failed !");
		free(count_arr);
		free(out_put);
		return;
	}
	
	for (i = 0; i < size; ++i)
	{
		++count_arr[arr[i]-min];
	}
	
	for (i = 1; i < range; ++i)
	{
		count_arr[i] += count_arr[i - 1]; 
	}
	
	for (i = size; i > 0; --i)
	{
		index = (size_t)(arr[i - 1] - min);
		out_put[--count_arr[index]] = arr[i - 1];
	}
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = out_put[i];
	}
	
	free(count_arr);
	free(out_put);
	
}

void RadixSort(int* arr, size_t size)
{
	int max = 0;
	int exp = 0;
	
	max = GetMax(arr,size);
	
	for(exp = 1; max / exp > 0; exp *= BASE)
	{
		CountingSortDigit(arr,size,exp);
	}
	
}

static int GetMax(int* arr, size_t size)
{
	int max = arr[0];
	size_t i = 0;
	
	for (i = 1; i < size; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	
	return max;
}

static void CountingSortDigit(int* arr, size_t size, int exp)
{

	int digit = 0;
	size_t i = 0;
	int* out_put = NULL;
	
	int count_arr[BASE] = {0};
	out_put = (int*)malloc(size * sizeof(int));
	
	if (!out_put)
	{
		printf("Memory allocated failed !");
		free(out_put);
		return;
	}
	
	for (i = 0; i < size; ++i)
	{
		++count_arr[(arr[i] / exp) % BASE] ;
	}
	
	for (i = 1; i < BASE; ++i)
	{
		count_arr[i] += count_arr[i - 1]; 
	}
	
	for (i = size; i > 0; --i)
	{
		digit = (arr[i - 1] / exp) % BASE;
		out_put[--count_arr[digit]] = arr[i - 1];
	}
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = out_put[i];
	}
	
	free(out_put);
	
}
