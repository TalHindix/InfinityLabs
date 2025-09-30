/**************************************
Exercise:   SA - Multithreaded Quick Sort
Date:       29/09/2025
Developer:  Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <time.h>   /* time */
#include <string.h> /* memcpy */

#include "mt_q_sort.h" /* MTQuickSort */

#define NUM_OF_COPIES (100)

static int IsCString(const char* s);
static void TestMultithreading(char** words, size_t num_of_words);
static char** CopyArray(char** original, size_t length);

int main(void)
{
    char* buffer = NULL;
    char** words = NULL;
    size_t file_size = 0;
    size_t num_of_words = 0;
    size_t i = 0;
    char** large_array = NULL;
    size_t total_words = 0;
    
    buffer = ReadDictionary(&file_size);
    if (NULL == buffer)
    {
        return -1;
    }
    
    words = CreateArrayPointers(buffer, file_size, &num_of_words);
    if (NULL == words)
    {
        free(buffer);
        return -1;
    }
    
    if ((0 == num_of_words) || (NULL != words[num_of_words]))
    {
        free(words);
        free(buffer);
        return -1;
    }
    
    for (i = 0; i < num_of_words; ++i)
    {
        if ((NULL == words[i]) || (0 == IsCString(words[i])))
        {
            free(words);
            free(buffer);
            return -1;
        }
    }
    
    large_array = DuplicatePointers(words, num_of_words, NUM_OF_COPIES, &total_words);
    if (NULL == large_array)
    {
        free(words);
        free(buffer);
        return -1;
    }
    
 
    srand((unsigned int)time(NULL));

    Shuffle(large_array, total_words);
    
    printf("\n========== MULTITHREADED SORTING COMPARISON ==========\n\n");
    TestMultithreading(large_array, total_words);
    
    free(large_array);
    free(words);
    free(buffer);
    
    return 0;
}

static int IsCString(const char* s)
{
    size_t i = 0;
    
    assert(NULL != s);
    
    while ('\0' != s[i])
    {
        if ('\n' == s[i])
        {
            return 0;
        }
        ++i;
    }
    
    return 1;
}

static char** CopyArray(char** original, size_t length)
{
    char** copy = NULL;
    size_t i = 0;
    
    assert(NULL != original);
    
    copy = (char**)malloc(length * sizeof(char*));
    if (NULL == copy)
    {
        return NULL;
    }
    
    for (i = 0; i < length; ++i)
    {
        copy[i] = original[i];
    }
    
    return copy;
}

static void TestMultithreading(char** words, size_t num_of_words)
{
    size_t thread_counts[] = {1, 2, 4, 8};
    size_t num_tests = 4;
    size_t i = 0;
    char** test_array = NULL;
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};
    double elapsed = 0.0;
    
    assert(NULL != words);
    
    for (i = 0; i < num_tests; ++i)
    {
        test_array = CopyArray(words, num_of_words);
        if (NULL == test_array)
        {
            printf("Failed to create test array copy\n");
            continue;
        }
        
        printf("Testing with %lu thread(s)...\n", thread_counts[i]);
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        MTQuickSort(test_array, num_of_words, thread_counts[i]);
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Time: %.6f seconds\n\n", elapsed);
        
        free(test_array);
    }
}