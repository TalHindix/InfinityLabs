/**************************************
Exercise: 	SA - Recursive Sorting
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h>

#include "rec_sorting.h" /* BinarySearch */

#define GRN  "\033[32m"
#define RED  "\033[31m"
#define RST  "\033[0m"

static void PrintArray(int arr[],size_t size);
static void RunOnce(const char* name, size_t (*func)(const int arr[], size_t size, int target), const int arr[], size_t n, int target);


int main(void)
{
    int arr1[] = {1,3,5,2,7, 10, 15,6,50};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);

    
    int arr[] = { 1, 3, 5, 7, 9, 11, 13 };
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int targets[] = { 7, 4 };
    size_t t = 0;
    size_t tcount = sizeof(targets) / sizeof(targets[0]);

    printf("===Iterative vs Recurisve \n\n"); 
    for (t = 0; t < tcount; ++t)
    {
        RunOnce("Iterative", IterBinarySearch, arr, n, targets[t]);
        RunOnce("Recursive", RecBinarySearch,  arr, n, targets[t]);
        printf("\n");
    }

    RecMergeSort(arr1,size1);
    PrintArray(arr1,size1);

    return 0;
}

static void RunOnce(const char* name, size_t (*func)(const int arr[], size_t size, int target), const int arr[], size_t n, int target)
{
    size_t idx = func(arr, n, target);

    if (idx != (size_t)-1)
    {
        printf(GRN "%s: %d Found In %lu\n" RST, name, target, (unsigned long)idx);
    }
    else
    {
        printf(RED "%s: %d Not found \n" RST, name, target);
    }
}

void PrintArray(int arr[],size_t size)
{
    size_t i = 0;
    for(i = 0; i < size; ++i)
    {
        printf("index: %lu value-> %d\n",i,arr[i]);
    }
    printf("\n");
}
