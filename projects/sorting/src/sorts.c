/**************************************
Exercise: 	SA - Recursive Sorting
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "sorts.h" /* QuickSort */

/* Enums */
enum { MS_OK = 0, MS_ERR = -1 };

/* Prototypes */
static size_t RecBinarySearchHelper(const int arr[], size_t low, size_t high, int target);
static int RecMergeSortHelper(int* arr, size_t low, size_t high);
static int Merge(int* arr, size_t l, size_t mid, size_t high);

static void SwapBytes(char* a, char* b, size_t ele_size);
static size_t Partition(char *base, size_t left, size_t right, size_t elem_size, cmp_func_t cmp);
static void QuickSortRec(char *base, size_t left, size_t right, size_t elem_size, cmp_func_t cmp);

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

    for(p = 1; p < num_elements; p *=2 )
    {
        for(i = 0; i + p < num_elements; i += 2 * p)
        {
            low  = i;
            mid  = i + p - 1;

            high = (i + 2 * p - 1 < num_elements) ? (i + 2 * p - 1) : (num_elements - 1);
            
            Merge(arr_to_sort, low, mid, high);
        }
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

void QuickSort(void *base, size_t n_items, size_t elem_size, cmp_func_t cmp)
{
    if (NULL == base || n_items <= 2 || elem_size == 0 || NULL == cmp)
    {
        return;
    }

    QuickSortRec(base, 0, n_items - 1, elem_size, cmp);
}

/* HELPER FUNCTIONS*/

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

static void SwapBytes(char* a, char* b, size_t ele_size)
{
    char* temp = NULL;

    if ((NULL == a) || (NULL == b) || (a == b))
    {
        return;
    }

    temp = (char*)malloc(ele_size);
    if(NULL == temp)
    {
        return;
    }

    memcpy(temp,a,ele_size);
    memcpy(a,b,ele_size);
    memcpy(b,temp,ele_size);

    free(temp);
}

static size_t Partition(char *base, size_t left, size_t right, size_t elem_size, cmp_func_t cmp)
{
    size_t pivot_index = left + (right - left) / 2;
    char* pivot_slot = base + pivot_index * elem_size;
    char* last_slot = base + right * elem_size;
    size_t store_idx = left;
    size_t i = 0;
    char* curr_slot = NULL;

    SwapBytes(pivot_slot, last_slot, elem_size);

    for(i = left; i < right; ++i)
    {
        curr_slot = (char *)base + i * elem_size;

        if(cmp(curr_slot,last_slot) < 0)
        {
            SwapBytes(curr_slot,base + (store_idx * elem_size),elem_size);
            ++store_idx;
        }
    }

    SwapBytes(base + store_idx * elem_size, last_slot, elem_size);

    return store_idx;
}

static void QuickSortRec(char *base, size_t left, size_t right, size_t elem_size, cmp_func_t cmp)
{
    size_t pivot = 0;

    if (left >= right)
    {
        return;
    }

    pivot = Partition(base,left,right,elem_size,cmp);

    if(pivot > 0)
    {
        QuickSortRec(base,left, pivot - 1, elem_size,cmp);
    }

    QuickSortRec(base,pivot+1,right,elem_size,cmp);

}