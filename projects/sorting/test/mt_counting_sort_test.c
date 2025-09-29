/**************************************
Exercise: 	SA - Multithreaded Counting Sort
Date:		29/9/2025
Developer:	Tal Hindi
Reviewer: 	Guy Argaman
Status:		Wait For Review
**************************************/

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_create */
#include <time.h>       /* clock */
#include <stdlib.h>     /* malloc, free */
#include <string.h>     /* memcpy */

#include "mt_counting_sort.h" /* MTCountingSort */

#define GREEN "\033[92m"
#define RED "\033[1;31m"
#define BLUE "\033[34m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

static int IsSorted(char* arr, size_t len);
static void TestCorrectness(void);
static int TestPerformance(size_t num_of_threads, size_t iterations);

int main(void)
{
    printf("\n ******** Multi Threaded Counting Sort Test ******** \n\n");

    printf(BOLD GREEN "=== Correctness Tests ===\n" RESET);
    TestCorrectness();

    printf(BOLD GREEN "\n=== Performance Tests ===\n" RESET);
    
    printf(BOLD GREEN "For 1 iteration:\n" RESET);
    TestPerformance(1, 1);
    TestPerformance(2, 1);
    TestPerformance(4, 1);
    TestPerformance(8, 1);
    TestPerformance(16, 1);

    printf(BOLD GREEN "\nFor 10 iterations:\n" RESET);
    TestPerformance(1, 10);
    TestPerformance(2, 10);
    TestPerformance(4, 10);
    TestPerformance(8, 10);
    TestPerformance(16, 10);

    printf(BOLD GREEN "\nFor 1000 iterations:\n" RESET);
    TestPerformance(1, 1000);
    TestPerformance(2, 1000);
    TestPerformance(4, 1000);
    TestPerformance(8, 1000);
    TestPerformance(16, 1000);

    printf("\n" BOLD GREEN "All tests complete!\n" RESET);

    return 0;
}

static void TestCorrectness(void)
{
    char test1[] = "zyxwvutsrqponmlkjihgfedcba";
    char test2[] = "Hello World! 123";
    char test3[] = "aaazzzbbbccc";
    size_t len1 = sizeof(test1) - 1;
    size_t len2 = sizeof(test2) - 1;
    size_t len3 = sizeof(test3) - 1;
    
    printf("Test 1 - Reverse alphabet: ");
    MTCountingSort(test1, len1, 4);
    printf("%s\n", IsSorted(test1, len1) ? BOLD GREEN "PASS" RESET : BOLD RED "FAIL" RESET);
    
    printf("Test 2 - Mixed chars:      ");
    MTCountingSort(test2, len2, 2);
    printf("%s\n", IsSorted(test2, len2) ? BOLD GREEN "PASS" RESET : BOLD RED "FAIL" RESET);
    
    printf("Test 3 - Duplicates:       ");
    MTCountingSort(test3, len3, 3);
    printf("%s\n", IsSorted(test3, len3) ? BOLD GREEN "PASS" RESET : BOLD RED "FAIL" RESET);
}

static int TestPerformance(size_t num_of_threads, size_t iterations)
{
    double time_taken = 0;
    clock_t start_time = 0;
    size_t length = 0;
    size_t i = 0;
    char* arr = NULL;
    
    start_time = clock();
    
    for (i = 0; i < iterations; ++i)
    {
        arr = ReadDictionary(&length);

        if (0 != MTCountingSort(arr, length, num_of_threads))
        {
            free(arr);
            return 1;
        }
        
        free(arr);
    }
    
    time_taken = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf(BOLD BLUE "%2lu threads: %.6f sec (avg: %.6f sec/sort)\n" RESET, 
           num_of_threads, time_taken, time_taken / iterations);

    return 0;
}

static int IsSorted(char* arr, size_t len)
{
    size_t i = 0;
    
    if (0 == len)
    {
        return 1;
    }
    
    for (i = 0; i < len - 1; ++i)
    {
        if ((unsigned char)arr[i] > (unsigned char)arr[i + 1])
        {
            return 0;
        }
    }
    
    return 1;
}
