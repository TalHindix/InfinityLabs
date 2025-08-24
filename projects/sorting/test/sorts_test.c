/**************************************
Exercise: 	SA - Recursive Sorting
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <stdio.h>    /* printf */
#include <stdlib.h>   /* malloc */
#include <string.h>   /* memcpy */
#include <time.h>     /* clock */

#include "sorts.h" /* QuickSort */

#define CLR_RST  "\033[0m"
#define CLR_OK   "\033[1;32m"
#define CLR_BAD  "\033[1;31m"

#define BENCH_N (500000) 
#define BENCH_SEED (20250824)

static void TestBinaryIter(void);
static void TestBinaryRec(void);
static void TestMergeSort(void);
static void TestQuickSort(void);
static void TestBenChmark(void);

static int CmpInt(const void* a, const void* b);
static int* GenRandArray(size_t n);
static int IsSorted(const int* arr, size_t n);
static void Report(int pass, const char* msg);

int main(void)
{
    TestBinaryIter();
    TestBinaryRec();
    TestMergeSort();
    TestQuickSort();

    TestBenChmark();

    return 0;
}

static void TestBinaryIter(void)
{
    const int data[] = { -30 , -10 , -1 , 0 , 2 , 7 , 42 };
    const size_t n = sizeof(data)/sizeof(data[0]);

    printf("\n== Binary Search (Iterative) ==");

    Report(IterBinarySearch(data, n ,  0) == 3 , "find 0");
    Report(IterBinarySearch(data, n , 42) == 6 , "find 42");
    Report(IterBinarySearch(data, n , 99) == (size_t)-1, "miss 99");
}

static void TestBinaryRec(void)
{
    const int data[] = { -5 , 0 , 1 , 1 , 2 , 3 };
    const size_t n = sizeof(data)/sizeof(data[0]);

    printf("\n== Binary Search (Recursive) ==");

    Report(RecBinarySearch(data, n , -5) == 0 , "find -5");
    Report(RecBinarySearch(data, n , 3) == 5 , "find 3");
    Report(RecBinarySearch(data, n , 9) == (size_t)-1, "miss 9");
}

static void TestMergeSort(void)
{
    int data[] = { 10 , -2 , 7 , 4 , 0 , 99 , -30 };
    const size_t n = sizeof(data)/sizeof(data[0]);

    printf("\n== Merge Sort ==");
    IMergeSort(data , n);
    Report(IsSorted(data , n) , "array sorted");
}

static void TestQuickSort(void)
{
    int data[] = { 5 , 4 , 3 , 2 , 1 };
    const size_t n = sizeof(data)/sizeof(data[0]);

    printf("\n== Quick Sort ==");
    QuickSort(data , n , sizeof(data[0]) , CmpInt);
    Report(IsSorted(data , n) , "array sorted");
}

static double TimeCall(void (*sort_fn)(void *, size_t, size_t, int (*)(const void *, const void *)), void *arr , size_t n)
{
    clock_t t0 = clock();
    sort_fn(arr , n , sizeof(int) , CmpInt);
    return (double)(clock() - t0) / CLOCKS_PER_SEC;
}

static double Time_qsort(int *arr , size_t n)
{
    clock_t t0 = clock();
    qsort(arr , n , sizeof(arr[0]) , CmpInt);
    return (double)(clock() - t0) / CLOCKS_PER_SEC;
}

static void TestBenChmark(void)
{
    int* orig = NULL;
    int* tmp  = NULL;
    double t_my = 0.0;
    double t_std = 0.0;

    printf("\n== Benchmark – QuickSort vs qsort ==");

    orig = GenRandArray(BENCH_N);
    if (!orig) 
    { 
        printf("alloc failed");
        return;
    }

    tmp  = (int *)malloc(BENCH_N * sizeof(int));
    if (!tmp)
    {
        free(orig);
        printf("alloc failed"); 
        return;
    }

    memcpy(tmp , orig , BENCH_N * sizeof(int));
    t_my  = TimeCall(QuickSort , tmp , BENCH_N);
    printf("QuickSort  : %.3f sec\n", t_my);

    memcpy(tmp , orig , BENCH_N * sizeof(int));
    t_std = Time_qsort(tmp , BENCH_N);
    printf("glibc qsort: %.3f sec\n", t_std);

    free(tmp);
    free(orig);
}

static int CmpInt(const void *a , const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

static int *GenRandArray(size_t n)
{
    int *arr = (int *)malloc(n * sizeof(int));
    size_t i = 0;

    if (!arr) 
    { 
        return NULL;
    }

    srand((unsigned)BENCH_SEED);
    for (i = 0 ; i < n ; ++i)
    {
        arr[i] = rand();
    }
    return arr;
}

static int IsSorted(const int *arr , size_t n)
{
    size_t i = 1;
    for (; i < n ; ++i)
    {
        if (arr[i-1] > arr[i])
        {
            return 0;
        }
    }
    return 1;
}

static void Report(int pass , const char *msg)
{
    printf("%s%s%s – %s\n", pass ? CLR_OK : CLR_BAD, pass ? "PASS" : "FAIL", CLR_RST, msg);
}
