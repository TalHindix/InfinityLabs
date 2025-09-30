/**************************************
Exercise:   SA - Multithreaded Quick Sort
Date:       29/09/2025
Developer:  Tal Hindi
Reviewer:   Yuval Hochman
Status:     In Progress
**************************************/

#include <stdlib.h> /* malloc */
#include <stdio.h>  /* fopen */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <pthread.h> /* pthread_create */
#include <time.h>   /* clock_gettime */

#include "mt_q_sort.h" /* MTQuickSort */

typedef struct
{
    char** arr;
    size_t start_index;
    size_t end_index;
} thread_data_t;

static int RandomCompare(const void* a, const void* b);
static int StringCompare(const void* a, const void* b);
static void* ThreadSort(void* arg);
static void Merge(char** arr, size_t start, size_t mid, size_t end);
static void MergeSortedParts(char** arr, size_t length, size_t num_of_threads);

int MTQuickSort(char** arr, size_t length, size_t num_of_threads)
{
    pthread_t* threads = NULL;
    thread_data_t* thread_data = NULL;
    size_t i = 0;
    size_t chunk_size = 0;
    
    assert(NULL != arr);
    assert(0 < num_of_threads);
    assert(0 < length);
    
    threads = (pthread_t*)malloc(num_of_threads * sizeof(pthread_t));
    if (NULL == threads)
    {
        return -1;
    }
    
    thread_data = (thread_data_t*)malloc(num_of_threads * sizeof(thread_data_t));
    if (NULL == thread_data)
    {
        free(threads);
        return -1;
    }
    
    chunk_size = length / num_of_threads;
    
    /* create threads to sort chunks */
    for (i = 0; i < num_of_threads; ++i)
    {
        thread_data[i].arr = arr;
        thread_data[i].start_index = i * chunk_size;
        thread_data[i].end_index = (i == num_of_threads - 1) ? length : (i + 1) * chunk_size;
        
        if (0 != pthread_create(&threads[i], NULL, ThreadSort, &thread_data[i]))
        {
            free(thread_data);
            free(threads);
            return -1;
        }
    }
    
    /* wait for all threads to complete */
    for (i = 0; i < num_of_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    
    /* merge sorted chunks */
    MergeSortedParts(arr, length, num_of_threads);
    
    free(thread_data);
    free(threads);
    
    return 0;
}

void Shuffle(char** arr, size_t length)
{
    assert(NULL != arr);
    
    qsort(arr, length, sizeof(char*), RandomCompare);
}

char* ReadDictionary(size_t* num_of_words)
{
    char dir[] = "/usr/share/dict/words";
    FILE* fp = NULL;
    char* buffer = NULL;
    size_t bytes_read = 0;
    long file_size = 0;
    
    assert(NULL != num_of_words);
    
    fp = fopen(dir, "r");
    if (NULL == fp)
    {
        perror("fopen");
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    if (file_size < 0)
    {
        perror("ftell");
        fclose(fp);
        fp = NULL;
        return NULL;
    }
    *num_of_words = (size_t)file_size;
    rewind(fp);
    
    buffer = (char*)malloc(*num_of_words + 1);
    if (NULL == buffer)
    {
        perror("malloc");
        fclose(fp);
        fp = NULL;
        return NULL;
    }
    
    bytes_read = fread(buffer, 1, *num_of_words, fp);
    if (bytes_read != *num_of_words)
    {
        perror("fread");
        free(buffer);
        buffer = NULL;
        fclose(fp);
        fp = NULL;
        return NULL;
    }
    
    buffer[*num_of_words] = '\0';
    fclose(fp);
    fp = NULL;
    
    return buffer;
}

char** CreateArrayPointers(char* buffer, size_t length, size_t* num_of_words)
{
    size_t i = 0;
    size_t j = 0;
    size_t count = 0;
    int in_word = 0;
    char** ptrs = NULL;
    
    if ((NULL == buffer) || (NULL == num_of_words))
    {
        return NULL;
    }
    
    *num_of_words = 0;
    
    if (0 == length)
    {
        ptrs = (char**)malloc(sizeof(char*));
        if (NULL == ptrs)
        {
            return NULL;
        }
        
        ptrs[0] = NULL;
        return ptrs;
    }
    
    for (i = 0; i < length; ++i)
    {
        if ('\n' != buffer[i])
        {
            if (0 == in_word)
            {
                in_word = 1;
                ++count;
            }
        }
        else
        {
            in_word = 0;
        }
    }
    
    ptrs = (char**)malloc((count + 1) * sizeof(char*));
    if (NULL == ptrs)
    {
        return NULL;
    }
    
    in_word = 0;
    for (i = 0; i < length; ++i)
    {
        if ('\n' == buffer[i])
        {
            buffer[i] = '\0';
            in_word = 0;
        }
        else
        {
            if (0 == in_word)
            {
                in_word = 1;
                ptrs[j] = &buffer[i];
                ++j;
            }
        }
    }
    
    ptrs[j] = NULL;
    *num_of_words = j;
    
    return ptrs;
}

char** DuplicatePointers(char** original, size_t num_of_words, size_t num_of_copies, size_t* total_words)
{
    size_t i = 0;
    size_t copy = 0;
    size_t offset = 0;
    char** duplicated = NULL;
    
    assert(NULL != original);
    assert(NULL != total_words);
    assert(0 < num_of_copies);
    
    *total_words = num_of_words * num_of_copies;
    
    duplicated = (char**)malloc((*total_words + 1) * sizeof(char*));
    if (NULL == duplicated)
    {
        return NULL;
    }
    
    for (copy = 0; copy < num_of_copies; ++copy)
    {
        offset = copy * num_of_words;
        for (i = 0; i < num_of_words; ++i)
        {
            duplicated[offset + i] = original[i];
        }
    }
    
    duplicated[*total_words] = NULL;
    
    return duplicated;
}

static int RandomCompare(const void* a, const void* b)
{
    (void)a;
    (void)b;
    
    return rand() - RAND_MAX / 2;
}

static int StringCompare(const void* a, const void* b)
{
    const char* str_a = NULL;
    const char* str_b = NULL;
    
    assert(NULL != a);
    assert(NULL != b);
    
    str_a = *(const char**)a;
    str_b = *(const char**)b;
    
    return strcmp(str_a, str_b);
}

static void* ThreadSort(void* arg)
{
    thread_data_t* data = NULL;
    size_t chunk_length = 0;
    
    assert(NULL != arg);
    
    data = (thread_data_t*)arg;
    chunk_length = data->end_index - data->start_index;
    
    qsort(&data->arr[data->start_index], chunk_length, sizeof(char*), StringCompare);
    
    return NULL;
}

static void Merge(char** arr, size_t start, size_t mid, size_t end)
{
    char** temp = NULL;
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    
    assert(NULL != arr);
    
    temp = (char**)malloc((end - start) * sizeof(char*));
    if (NULL == temp)
    {
        return;
    }
    
    i = start;
    j = mid;
    k = 0;
    
    /* merge two sorted sections */
    while ((i < mid) && (j < end))
    {
        if (strcmp(arr[i], arr[j]) <= 0)
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }
    
    /* copy remaining elements */
    while (i < mid)
    {
        temp[k++] = arr[i++];
    }
    
    while (j < end)
    {
        temp[k++] = arr[j++];
    }
    
    /* copy back to original array */
    for (i = 0; i < (end - start); ++i)
    {
        arr[start + i] = temp[i];
    }
    
    free(temp);
}

static void MergeSortedParts(char** arr, size_t length, size_t num_of_threads)
{
    size_t chunk_size = 0;
    size_t current_size = 0;
    size_t i = 0;
    
    assert(NULL != arr);
    assert(0 < num_of_threads);
    
    if (1 == num_of_threads)
    {
        return;
    }
    
    chunk_size = length / num_of_threads;
    current_size = chunk_size;
    
    /* progressively merge chunks */
    for (i = 1; i < num_of_threads; ++i)
    {
        if (i == num_of_threads - 1)
        {
            Merge(arr, 0, current_size, length);
        }
        else
        {
            Merge(arr, 0, current_size, current_size + chunk_size);
            current_size += chunk_size;
        }
    }
}

