/**************************************
Exercise: 	System Programming - 100k Threads Ex 3
Date:		08/09/2025
Developer:	Tal Hindi
Reviewer: 	Lotem Kitaroo
Status:		In Progress
**************************************/

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_create */
#include <time.h>       /* time */
#include <unistd.h>     /* sleep */

#define NUM_THREADS (1000000)

pthread_t g_thread[NUM_THREADS];
int g_array[NUM_THREADS] = {0};
pthread_mutex_t g_counter_mutex = PTHREAD_MUTEX_INITIALIZER;
size_t g_completed_threads = 0; 

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
	
	printf("Thread creation time: %ld seconds\n", end_time - start_time);
	
	printf("Waiting for threads to complete...\n");
	while (g_completed_threads < NUM_THREADS)
	{
		sleep(1);
	}
	
	printf("All threads completed\n");
	ValidateArray();
	
	return 0;
}

static void CreateThreads()
{
	size_t i = 0;
	
	for (i = 0; i < NUM_THREADS; ++i)
	{
		while (0 != pthread_create(&g_thread[i], NULL, InitValue, (void*)i))
		{
			sleep(1);
			printf("pthread_create failed for thread %lu, retrying...\n", i);
		}
		
		pthread_detach(g_thread[i]);
		
		if (0 == (i + 1) % 10000)
		{
			printf("Created %lu threads...\n", i + 1);
		}
	}
	
	printf("All %d threads created and detached\n", NUM_THREADS);
}

static void* InitValue(void* ptr)
{
	size_t thread_id = (size_t)ptr;
	g_array[thread_id] = (int)thread_id;

    pthread_mutex_lock(&g_counter_mutex);
	g_completed_threads++;
	pthread_mutex_unlock(&g_counter_mutex);

	return NULL;
}

static void ValidateArray()
{
	size_t i = 0;
	size_t errors = 0;
	
	for (i = 0; i < NUM_THREADS; ++i)
	{
		if ((int)i != g_array[i])
		{
			printf("ERROR: g_array[%lu] = %d, expected %lu\n", i, g_array[i], i);
			errors++;
		}
	}
	
	if (0 == errors)
	{
		printf("SUCCESS: All array elements have correct values\n");
	}
	else
	{
		printf("FAILED: %lu array elements have incorrect values\n", errors);
	}
}