/**************************************
Exercise: 	SA - Comparing Sorting
Date:		5/8/2025
Developer:	Tal Hindi
Reviewer: 	Dana Chesner
Status:		
**************************************/
 
#include "sorting.h"

void Swap(int* a, int* b)
{
	int temp = *a;
	
	*a = *b;
	*b = temp;
}

void BubbleSort(int* arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	int is_swap = 1;
	
	if (size < 2 || !arr)
	{
		return;
	}
	
	for (i = 0; i < size - 1 && is_swap ; ++i)
	{
		is_swap = 0;
		for(j = 0; j < size - i - 1; ++j)
		{
			if(arr[j] > arr[j + 1])
			{
				Swap(&arr[j], &arr[j + 1]);
				is_swap = 1;
			}
		}		
	}
}

void SelectionSort(int* arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	size_t min_index = 0;
	
	if (!arr || size < 2)
	{
		return;
	}
	
	for(i = 0; i < size - 1; ++i)
	{
		min_index = i;
		
		for(j = i + 1; j < size; ++j)
		{
			if(arr[j] < arr[min_index])
			{
				min_index = j;
			}
		}
		
		Swap(&arr[i], &arr[min_index]);
	}
	
}

void InsertionSort(int* arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	
	if (!arr || size < 2)
	{
		return;
	}
	
	for (i = 1; i < size; ++i)
	{
		j = i;
		
		while (j > 0 && arr[j - 1] > arr[j])
		{
			Swap(&arr[j], &arr[j-1]);
			j = j - 1;
		}
	}
}



