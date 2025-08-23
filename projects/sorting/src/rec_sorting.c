/**************************************
Exercise: 	SA - Recursive Sorting
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc */
#include "rec_sorting.h"

static size_t RecBinarySearchHelper(const int arr[], size_t low, size_t high, int target);
static int RecMergeSortHelper(int* arr, size_t low, size_t high);
static int Merge(int* arr, size_t l, size_t mid, size_t high);

enum { MS_OK = 0, MS_ERR = -1 };

size_t IterBinarySearch(const int arr[],size_t size, int target)
{
    size_t low = 0;
    size_t high = size - 1;
    size_t mid = 0;

    while (low <= high)
    {
        mid = low + (high - low) / 2;

        if (target == arr[mid])
        {
            return mid;
        }
        else if (target < arr[mid])
        {
            high = mid - 1;
        }
        else /* target > arr[mid]) */
        {
            low = mid + 1 ;
        }

    }

    return (size_t)-1;
}

size_t RecBinarySearch(const int arr[],size_t size, int target)
{
    if (NULL == arr || size == 0)
    {
        return (size_t)-1;
    }

    return RecBinarySearchHelper(arr, 0, size - 1, target);
}

void IMergeSort(int* arr_to_sort, size_t num_elements)
{
    size_t p = 0;
    size_t low = 0;
    size_t mid = 0;
    size_t high = 0;
    size_t i = 0;

    for(p = 2; p <= num_elements; p*=2 )
    {
        for(i = 0; i + p - 1 < num_elements; i+=p)
        {
            low = i;
            high = i + p - 1;
            mid = (low + high) / 2;
            
            Merge(arr_to_sort,low,mid,high);
        }
    }

    if(p/2 < num_elements)
    {
        Merge(arr_to_sort,0,(p/2)-1,num_elements-1);
    }
}

int RecMergeSort(int* arr_to_sort, size_t num_elements)
{
    if (NULL == arr_to_sort || num_elements < 2)
    {
        return MS_ERR;
    }

    return RecMergeSortHelper(arr_to_sort,0,num_elements - 1);
}

static int RecMergeSortHelper(int* arr, size_t low, size_t high)
{
    int status = MS_OK;

    if (low < high)
    {
        size_t mid = low + (high - low) / 2;

        status = RecMergeSortHelper(arr, low, mid);
        if (status != MS_OK)
        {
            return status;
        }

        status = RecMergeSortHelper(arr, mid + 1, high);
        if (status != MS_OK)
        {
            return status;
        }

        status = Merge(arr, low, mid, high);
    }

    return status;
}

static int Merge(int* arr, size_t left, size_t mid, size_t right)
{
    size_t i = left;
    size_t j = mid + 1;
    size_t k = 0;

    int* tmp = malloc(sizeof(int) * (right-left + 1));
    if (NULL == tmp)
    {
        return MS_ERR;
    }

    while(i <= mid && j <= right)
    {
        if(arr[i] < arr[j])
        {
            tmp[k++] = arr[i++];
        }
        else
        {
            tmp[k++] = arr[j++];
        }
    }
    
    while(i <= mid)
    {
        tmp[k++] = arr[i++];
    }

    while(j <= right)
    {
        tmp[k++] = arr[j++];
    }

    memcpy(arr + left, tmp, (right - left + 1) * sizeof(int));

    free(tmp);

    return MS_OK;
}

static size_t RecBinarySearchHelper(const int arr[], size_t low, size_t high, int target)
{
    size_t mid = 0;

    if (low > high)
    {
        return (size_t)-1;
    }

    mid = low + (high - low) / 2;

    if (target == arr[mid])
    {
        return mid;
    }

    if (target < arr[mid])
    {
        return RecBinarySearchHelper(arr, low, mid - 1, target);
    }

    return RecBinarySearchHelper(arr,mid + 1 , high, target);
    
}
