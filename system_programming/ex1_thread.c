/**************************************
Exercise: 	System Programming - 100k Threads ex1
Date:		08/09/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		In Progress
**************************************/

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_create */
#include <stdlib.h>     /* malloc, free */

#define NUM_THREADS 100000

pthread_t g_thread[NUM_THREADS];
int g_array[NUM_THREADS];

static void CreateThreads();
static void* InitValue(void* ptr);
static void PrintArray();

int main(void)
{
    CreateThreads();
    PrintArray();
    
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

static void PrintArray()
{
    size_t i = 0;
    for(i = 0; i < NUM_THREADS; ++i)
    {
        printf("g_array[%lu] = %d\n", i, g_array[i]);
    }
}


