/**************************************
Exercise:	SA - Recursive Sorting
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/

#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc  */
#include <assert.h> /* assert  */

#include "sorts.h" /* QuickSort */

#define SWAP_SBO 256

enum { MS_OK = 0, MS_ERR = -1 };

static size_t RecBinarySearchHelper(const int arr[], size_t low, size_t high, int target);

static int   RecMergeSortHelper(int* arr, size_t left, size_t right, int* tmp);
static void  Merge(int* arr, size_t left, size_t mid, size_t right, int* tmp);

static void   SwapBytes(char* a, char* b, size_t n);
static size_t Partition(char* base, size_t left, size_t right, size_t n, cmp_func_t cmp);
static void   QuickSortRec(char* base, size_t left, size_t right, size_t n, cmp_func_t cmp);


size_t IterBinarySearch(const int arr[], size_t size, int target)
{
    size_t low  = 0;
    size_t high = size - 1;
    size_t mid  = 0;

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
        else
        {
            low = mid + 1;
        }
    }

    return (size_t)-1;
}

size_t RecBinarySearch(const int arr[], size_t size, int target)
{
    if (NULL == arr || 0 == size)
    {
        return (size_t)-1;
    }

    return RecBinarySearchHelper(arr, 0, size - 1, target);
}

void IMergeSort(int* arr, size_t n)
{
    size_t step  = 0;
    size_t left  = 0;
    size_t mid   = 0;
    size_t right = 0;
    size_t i     = 0;

    int* tmp = NULL;

    if (NULL == arr || n < 2)
    {
        return;
    }

    tmp = (int*)malloc(n * sizeof(int));
    if (NULL == tmp)
    {
        return;
    }

    for (step = 1; step < n; step *= 2)
    {
        for (i = 0; i + step < n; i += 2 * step)
        {
            left  = i;
            mid   = i + step - 1;
            right = (i + 2 * step - 1 < n) ? (i + 2 * step - 1) : (n - 1);

            Merge(arr, left, mid, right, tmp);
        }
    }

    free(tmp);
}

int RecMergeSort(int* arr, size_t n)
{
    int* tmp = NULL;
    int ret = MS_OK;

    if (NULL == arr || n < 2)
    {
        return MS_ERR;
    }

    tmp = (int*)malloc(n * sizeof(int));
    if (NULL == tmp)
    {
        return MS_ERR;
    }

    ret = RecMergeSortHelper(arr, 0, n - 1, tmp);

    free(tmp);

    return ret;
}

void QuickSort(void* base, size_t n_items, size_t elem_size, cmp_func_t cmp)
{
    if (NULL == base || n_items <= 2 || 0 == elem_size || NULL == cmp)
    {
        return;
    }

    QuickSortRec((char*)base, 0, n_items - 1, elem_size, cmp);
}

static int RecMergeSortHelper(int* arr, size_t left, size_t right, int* tmp)
{
    if (left < right)
    {
        size_t mid = left + (right - left) / 2;

        if (MS_OK != RecMergeSortHelper(arr, left, mid, tmp))
        {
            return MS_ERR;
        }

        if (MS_OK != RecMergeSortHelper(arr, mid + 1, right, tmp))
        {
            return MS_ERR;
        }

        Merge(arr, left, mid, right, tmp);
    }

    return MS_OK;
}

static void Merge(int* arr, size_t left, size_t mid, size_t right, int* tmp)
{
    size_t i = left;
    size_t j = mid + 1;
    size_t k = 0;

    while (i <= mid && j <= right)
    {
        if (arr[i] < arr[j])
        {
            tmp[k] = arr[i];
            ++k;
            ++i;
        }
        else
        {
            tmp[k] = arr[j];
            ++k;
            ++j;
        }
    }

    while (i <= mid)
    {
        tmp[k] = arr[i];
        ++k;
        ++i;
    }

    while (j <= right)
    {
        tmp[k] = arr[j];
        ++k;
        ++j;
    }

    memcpy(arr + left, tmp, k * sizeof(int));
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

    return RecBinarySearchHelper(arr, mid + 1, high, target);
}

static void SwapBytes(char* a, char* b, size_t n)
{
    size_t i = 0;

    if (NULL == a || NULL == b || a == b)
    {
        return;
    }

    for (i = 0; i < n; ++i)
    {
        char tmp_byte = a[i];
        a[i] = b[i];
        b[i] = tmp_byte;
    }
}

static size_t Partition(char* base, size_t left, size_t right, size_t n, cmp_func_t cmp)
{
    char* pivot = base + right * n;
    size_t store = left;
    size_t i = 0;

    for (i = left; i < right; ++i)
    {
        char* current = base + i * n;
        if (cmp(current, pivot) < 0)
        {
            SwapBytes(current, base + store * n, n);
            ++store;
        }
    }

    SwapBytes(base + store * n, pivot, n);
    return store;
}

static void QuickSortRec(char* base, size_t left, size_t right, size_t n, cmp_func_t cmp)
{
    size_t pivot = 0;

    if (left >= right)
    {
        return;
    }

    pivot = Partition(base, left, right, n, cmp);

    if (pivot > 0)
    {
        QuickSortRec(base, left, pivot - 1, n, cmp); /* protect underflow */
    }

    QuickSortRec(base, pivot + 1, right, n, cmp);
}
