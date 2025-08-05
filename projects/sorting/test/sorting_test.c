/**************************************
Exercise: 	SA - Comparing Sorting
Date:		5/8/2025
Developer:	Tal Hindi
Reviewer: 	Dana Chesner
Status:		
**************************************/


#include <stdio.h>		/* printf 	*/
#include <stdlib.h>  	/* rand() 	*/
#include <time.h>    	/* time() 	*/


#include "sorting.h"

#define ARR_SIZE 5000

void TestBubbleSort();
void TestSelectionSort();
void TestInsertionSort();
void printArray(int* arr, size_t size);
int CompareInts(const void *a, const void *b);


int main(void)
{

	srand((unsigned)time(NULL));	
	/*
    TestBubbleSort();
       
    TestSelectionSort();
    */
    TestInsertionSort();
    
    
    return 0;
}

void TestBubbleSort()
{
    int arr[ARR_SIZE] = {0};
    int arr_qsort[ARR_SIZE] = {0};
    clock_t start = 0;
    clock_t end = 0;
    double duration_bubble = 0.0;
    double duration_qsort = 0.0;
    size_t i = 0;

    printf("------- TestBubbleSort -----------\n");

    for (i = 0; i < ARR_SIZE; ++i)
    {
        int val = (rand() % 5000) + 1;
        arr[i] = val;
        arr_qsort[i] = val;
    }

    start = clock();
    BubbleSort(arr, ARR_SIZE);
    end = clock();
    duration_bubble = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    qsort(arr_qsort, ARR_SIZE, sizeof(int), CompareInts);
    end = clock();
    duration_qsort = (double)(end - start) / CLOCKS_PER_SEC;


    printf("Sorted array (BubbleSort):\n");
    printArray(arr, ARR_SIZE);

    printf("\nBubbleSort time: %.6f seconds\n", duration_bubble);
    printf("qsort time     : %.6f seconds\n", duration_qsort);
}

void TestSelectionSort()
{
	int arr[ARR_SIZE] = {0};
    int arr_qsort[ARR_SIZE] = {0};
    clock_t start = 0;
    clock_t end = 0;
    double duration_selection = 0.0;
    double duration_qsort = 0.0;
    size_t i = 0;

    printf("------- TestSelectionSort -----------\n");

    for (i = 0; i < ARR_SIZE; ++i)
    {
        int val = (rand() % 5000) + 1;
        arr[i] = val;
        arr_qsort[i] = val;
    }

    start = clock();
    SelectionSort(arr, ARR_SIZE);
    end = clock();
    duration_selection = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    qsort(arr_qsort, ARR_SIZE, sizeof(int), CompareInts);
    end = clock();
    duration_qsort = (double)(end - start) / CLOCKS_PER_SEC;


    printf("Sorted array (SelectionSort):\n");
    printArray(arr, ARR_SIZE);

    printf("\nSelectionSort time: %.6f seconds\n", duration_selection);
    printf("qsort time     : %.6f seconds\n", duration_qsort);
}

void TestInsertionSort()
{
	int arr[ARR_SIZE] = {0};
    int arr_qsort[ARR_SIZE] = {0};
    clock_t start = 0;
    clock_t end = 0;
    double duration_insertion = 0.0;
    double duration_qsort = 0.0;
    size_t i = 0;

    printf("------- TestInsertionSort -----------\n");

    for (i = 0; i < ARR_SIZE; ++i)
    {
        int val = (rand() % 5000) + 1;
        arr[i] = val;
        arr_qsort[i] = val;
    }

    start = clock();
    InsertionSort(arr, ARR_SIZE);
    end = clock();
    duration_insertion = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    qsort(arr_qsort, ARR_SIZE, sizeof(int), CompareInts);
    end = clock();
    duration_qsort = (double)(end - start) / CLOCKS_PER_SEC;


    printf("Sorted array (InsertionSort):\n");
    printArray(arr, ARR_SIZE);

    printf("\nInsertionSort time: %.6f seconds\n", duration_insertion);
    printf("qsort time     : %.6f seconds\n", duration_qsort);
}

int CompareInts(const void* a, const void* b)
{
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return (int_a > int_b) - (int_a < int_b);
}

void printArray(int* arr, size_t size)
{
    size_t i = 0;
    
    for (i = 0; i < size; i++)
    {
        printf("%5d", arr[i]);
    }
    printf("\n");
    
}
