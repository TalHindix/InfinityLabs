/**************************************
Exercise: 	SA - Comparing Sorting
Date:		5/8/2025
Developer:	Tal Hindi
Reviewer: 	
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

int main(void)
{
	srand((unsigned)time(NULL));
	/*
    TestBubbleSort();
    */
    
    TestSelectionSort();
    /*
    TestInsertionSort();
    */
    return 0;
}
/
void TestBubbleSort()
{
	int arr[ARR_SIZE] = { 0 };
    size_t i = 0;
    printf("------- TestBubbleSort -----------");
    for(i = 0; i < ARR_SIZE; ++i)
    {
    	arr[i] = (rand() % 5000) + 1;
    }
    
    BubbleSort(arr, ARR_SIZE);
    printf("Sorted array: \n");
    printArray(arr, ARR_SIZE);
}

void TestSelectionSort()
{
	int arr[ARR_SIZE] = { 0 };
    size_t i = 0;
    
    printf(" ------- TestSelectionSort -----------");
    for(i = 0; i < ARR_SIZE; ++i)
    {
    	arr[i] = (rand() % 5000) + 1;
    }
    
    SelectionSort(arr, ARR_SIZE);
    printf("Sorted array: \n");
    printArray(arr, ARR_SIZE);
}

void TestInsertionSort()
{
	int arr[ARR_SIZE] = { 0 };
    size_t i = 0;
    
     printf(" ------- TestInsertionSort -----------");
    for(i = 0; i < ARR_SIZE; ++i)
    {
    	arr[i] = (rand() % 5000) + 1;
    }
    
    InsertionSort(arr, ARR_SIZE);
    printf("Sorted array: \n");
    printArray(arr, ARR_SIZE);
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
