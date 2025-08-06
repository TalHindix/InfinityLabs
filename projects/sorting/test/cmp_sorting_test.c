/**************************************
Exercise: 	SA - Cmp Sorting
Date:		5/8/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		Approved
**************************************/


#include <stdio.h>		/* printf 	*/
#include <stdlib.h>  	/* rand() 	*/
#include <time.h>    	/* time() 	*/

#include "cmp_sorting.h"

#define ARR_SIZE   5000
#define RAND_RANGE 5000

static void FillRandom(int *arr, size_t size);
static int  CmpInts(const void *a, const void *b);
static void PrintArray(const int *arr, size_t size);
static void TestBubbleSort(void);
static void TestSelectionSort(void);
static void TestInsertionSort(void);

int main(void)
{
    srand((unsigned)time(NULL));

    TestBubbleSort();
    TestSelectionSort();
    TestInsertionSort();

    return 0;
}

static void TestBubbleSort(void)
{
    int     arr[ARR_SIZE]       = {0};
    int     arr_qsort[ARR_SIZE] = {0};
    clock_t start               = 0;
    clock_t end                 = 0;
    clock_t ticks_bubble        = 0;
    clock_t ticks_qsort         = 0;
    size_t  i                   = 0;

    printf("\n========== BubbleSort ==========\n");

    FillRandom(arr, ARR_SIZE);
    for (i = 0; i < ARR_SIZE; ++i) { arr_qsort[i] = arr[i]; }

    start = clock();
    BubbleSort(arr, ARR_SIZE);
    end   = clock();
    ticks_bubble = end - start;

    start = clock();
    qsort(arr_qsort, ARR_SIZE, sizeof(int), CmpInts);
    end   = clock();
    ticks_qsort = end - start;

#if ARR_SIZE <= 50
    PrintArray(arr, ARR_SIZE);
#endif

    printf("BubbleSort ticks : %ld\n", (long)ticks_bubble);
    printf("qsort ticks      : %ld\n", (long)ticks_qsort);
}

static void TestSelectionSort(void)
{
    int     arr[ARR_SIZE]       = {0};
    int     arr_qsort[ARR_SIZE] = {0};
    clock_t start               = 0;
    clock_t end                 = 0;
    clock_t ticks_sel           = 0;
    clock_t ticks_qsort         = 0;
    size_t  i                   = 0;

    printf("\n========== SelectionSort ==========\n");

    FillRandom(arr, ARR_SIZE);
    for (i = 0; i < ARR_SIZE; ++i) { arr_qsort[i] = arr[i]; }

    start = clock();
    SelectionSort(arr, ARR_SIZE);
    end   = clock();
    ticks_sel = end - start;

    start = clock();
    qsort(arr_qsort, ARR_SIZE, sizeof(int), CmpInts);
    end   = clock();
    ticks_qsort = end - start;

    printf("SelectionSort ticks : %ld\n", (long)ticks_sel);
    printf("qsort ticks         : %ld\n", (long)ticks_qsort);
}

static void TestInsertionSort(void)
{
    int     arr[ARR_SIZE]       = {0};
    int     arr_qsort[ARR_SIZE] = {0};
    clock_t start               = 0;
    clock_t end                 = 0;
    clock_t ticks_ins           = 0;
    clock_t ticks_qsort         = 0;
    size_t  i                   = 0;

    printf("\n========== InsertionSort ==========\n");

    FillRandom(arr, ARR_SIZE);
    for (i = 0; i < ARR_SIZE; ++i) { arr_qsort[i] = arr[i]; }

    start = clock();
    InsertionSort(arr, ARR_SIZE);
    end   = clock();
    ticks_ins = end - start;

    start = clock();
    qsort(arr_qsort, ARR_SIZE, sizeof(int), CmpInts);
    end   = clock();
    ticks_qsort = end - start;

    printf("InsertionSort ticks : %ld\n", (long)ticks_ins);
    printf("qsort ticks         : %ld\n", (long)ticks_qsort);
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
#if ARR_SIZE <= 50
    size_t i = 0;
    for (i = 0; i < size; ++i)
    {
        printf("%5d", arr[i]);
    }
    putchar('\n');
#else
    (void)arr;
    (void)size;
#endif
}

