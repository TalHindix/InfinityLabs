/**************************************
Exercise: 	System Programming - 100k Threads Ex 1
Date:		08/09/2025
Developer:	Tal Hindi
Reviewer: 	Lotem Kitaroo
Status:		In Progress
**************************************/

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_create */
#include <stdlib.h>     /* malloc */
#include <time.h>       /* time */
#include <unistd.h>     /* sleep */

#define NUM_THREADS 1000000

pthread_t g_thread[NUM_THREADS];
int g_array[NUM_THREADS] = {0};

static void CreateThreads();
static void* InitValue(void* ptr);
static void ValidateArray();

int main(void)
{
    time_t start_time = 0;
    time_t end_time = 0;
    
    start_time = time(NULL);
    CreateThreads();
    end_time = time(NULL);
    
    printf("Thread creation and execution time: %ld seconds\n", end_time - start_time);
    
    sleep(10);
    
    ValidateArray();

    return 0;
}

static void CreateThreads()
{
    size_t i = 0;
    
    for(i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&g_thread[i],NULL,InitValue,(void*)i);
    }
    
    for(i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(g_thread[i], NULL);
    }
}

static void* InitValue(void* ptr)
{
    size_t thread_id = (size_t)ptr;
    g_array[thread_id] = (int)thread_id;
    return NULL;
}

static void ValidateArray()
{
    size_t i = 0;
    size_t errors = 0;
    
    for(i = 0; i < NUM_THREADS; ++i)
    {
        if ((int)i != g_array[i])
        {
            printf("ERROR: g_array[%lu] = %d, expected %lu\n", i, g_array[i], i);
            errors++;
        }
    }
    
    if(0 == errors)
    {
        printf("SUCCESS: All array elements have correct values\n");
    }
    else
    {
        printf("FAILED: %lu array elements have incorrect values\n", errors);
    }
}


