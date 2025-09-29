/**************************************
Exercise: 	SA - Multithreaded Counting Sort
Date:		29/9/2025
Developer:	Tal Hindi
Reviewer: 	Guy Argaman
Status:		Wait For Review
**************************************/

#include <limits.h> /* CHAR_MIN */
#include <pthread.h> /* pthread_mutex_t */
#include <errno.h> /* EAGAIN */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* FILE */

#include "mt_counting_sort.h"   /* MTCountingSort */

#define ASCII (256)

typedef struct
{
    char* arr;
    size_t start_index;
    size_t end_index;
    size_t* local_count_arr; /* Local counting array for this thread */
} thread_data_t;


static void* CountSegment(void* arg);

int MTCountingSort(char* arr, size_t length, size_t num_of_threads)
{
    size_t global_count[ASCII] = {0};
    size_t i = 0;
    size_t j = 0;
    size_t pos = 0;
    size_t segment = length / num_of_threads;
    thread_data_t* thread_data = NULL;
    pthread_t* threads = NULL;
    int status = 0;

    thread_data = malloc(sizeof(thread_data_t) * num_of_threads);
    if (NULL == thread_data)
    {
        return -1;
    }

    threads = malloc(sizeof(pthread_t) * num_of_threads);
    if (NULL == threads)
    {
        free(thread_data);
        return -1;
    }

    for (i = 0; i < num_of_threads; ++i)
    {
        thread_data[i].local_count_arr = calloc(ASCII, sizeof(size_t));
        if (NULL == thread_data[i].local_count_arr)
        {
            while (i > 0)
            {
                free(thread_data[--i].local_count_arr);
            }
            free(threads);
            free(thread_data);
            return -1;
        }
        
        thread_data[i].arr = arr;
        thread_data[i].start_index = i * segment;
        thread_data[i].end_index = (i == num_of_threads - 1) ? length : (i + 1) * segment;
    }

    /* Create threads */
    for (i = 0; i < num_of_threads; ++i)
    {
        status = pthread_create(&threads[i], NULL, &CountSegment, &thread_data[i]);
        if (0 != status)
        {
            return -1;
        }
    }

    /* Wait for all threads to complete */
    for (i = 0; i < num_of_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    for (i = 0; i < num_of_threads; ++i)
    {
        for (j = 0; j < ASCII; ++j)
        {
            global_count[j] += thread_data[i].local_count_arr[j];
        }
        free(thread_data[i].local_count_arr);
    }

    for (i = 0; i < ASCII; ++i)
    {
        for (j = 0; j < global_count[i]; ++j)
        {
            arr[pos++] = (char)i;
        }
    }

    free(threads);
    free(thread_data);

    return 0;
}


static void* CountSegment(void* arg)
{
    thread_data_t* thread_data = (thread_data_t*)arg;
    size_t i = 0;
    
    for (i = thread_data->start_index; i < thread_data->end_index; ++i)
    {
        unsigned char c = (unsigned char)thread_data->arr[i];
        thread_data->local_count_arr[c]++;
    }
    
    return NULL;
}

char* ReadDictionary(size_t* length)
{
    char dir[] = "/usr/share/dict/words";
    FILE* fp = NULL;
    char* buffer = NULL;
    size_t bytes_read = 0;

    fp = fopen(dir, "r");
    if (NULL == fp)
    {
        perror("fopen failed");
        return NULL;
    }

    /* Determine file size */
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp);
    rewind(fp);

    /* Allocate buffer for entire file plus null terminator */
    buffer = malloc(*length + 1);
    if (NULL == buffer)
    {
        perror("malloc failed");
        fclose(fp);
        return NULL;
    }

    /* Read file contents */
    bytes_read = fread(buffer, sizeof(char), *length, fp);
    if (bytes_read != *length)
    {
        perror("fread failed");
        free(buffer);
        fclose(fp);
        return NULL;
    }

    buffer[*length] = '\0';
    fclose(fp);

    return buffer;
}




