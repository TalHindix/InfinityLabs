/**************************************
Exercise: 	SA - Linear Sorting Test
Date:		6/8/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		
**************************************/

#include <stdio.h> 	/* printf 	*/
#include <stdlib.h>	/* size_t 	*/ 
#include <time.h>	/* clock_t 	*/

#include "linear_sorting.h"

#define ARR_SIZE   5000
#define RAND_RANGE 5000

static void FillRandom(int *arr, size_t size);
static int CmpInts(const void *a, const void *b);
static void PrintArray(const int *arr, size_t size);
static void TestCountingSort(void);
static void TestRadixSort(void);

int main(void)
{
	srand((unsigned)time(NULL));

	TestCountingSort();
	TestRadixSort();

	return 0;
}

static void TestCountingSort(void)
{
	int arr[ARR_SIZE] = {0};
	int arr_qsort[ARR_SIZE] = {0};
	clock_t start = 0;
	clock_t end = 0;
	clock_t ticks_count = 0;
	clock_t ticks_qsort = 0;
	size_t i = 0;

	printf("\n========== CountingSort ==========\n");

	FillRandom(arr, ARR_SIZE);
	for (i = 0; i < ARR_SIZE; ++i) { arr_qsort[i] = arr[i]; }

	start = clock();
	CountingSort(arr, ARR_SIZE);
	end = clock();
	ticks_count = end - start;

	start = clock();
	qsort(arr_qsort, ARR_SIZE, sizeof(int), CmpInts);
	end = clock();
	ticks_qsort = end - start;

	/* Uncomment for debug:
	PrintArray(arr, ARR_SIZE);
	*/

	printf("CountingSort ticks : %ld\n", (long)ticks_count);
	printf("qsort ticks        : %ld\n", (long)ticks_qsort);
}

static void TestRadixSort(void)
{
	int arr[ARR_SIZE] = {0};
	int arr_qsort[ARR_SIZE] = {0};
	clock_t start = 0;
	clock_t end = 0;
	clock_t ticks_radix = 0;
	clock_t ticks_qsort = 0;
	size_t i = 0;

	printf("\n========== RadixSort ==========\n");

	FillRandom(arr, ARR_SIZE);
	for (i = 0; i < ARR_SIZE; ++i) { arr_qsort[i] = arr[i]; }

	start = clock();
	RadixSort(arr, ARR_SIZE);
	end = clock();
	ticks_radix = end - start;

	start = clock();
	qsort(arr_qsort, ARR_SIZE, sizeof(int), CmpInts);
	end = clock();
	ticks_qsort = end - start;

	/* Uncomment for debug:
	PrintArray(arr, ARR_SIZE);
	*/

	printf("RadixSort ticks    : %ld\n", (long)ticks_radix);
	printf("qsort ticks        : %ld\n", (long)ticks_qsort);
}

static void FillRandom(int *arr, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		arr[i] = (rand() % RAND_RANGE) + 1;
	}
}

static int CmpInts(const void *a, const void *b)
{
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

static void PrintArray(const int *arr, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		printf("%5d", arr[i]);
	}
	putchar('\n');
}

