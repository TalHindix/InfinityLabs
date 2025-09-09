/**************************************
Exercise: 	System Programming - Exercise 4.3
Date:		08/09/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		In Progress
**************************************/

#include <stdio.h>		/* printf */
#include <pthread.h>	/* pthread_create */
#include <time.h>		/* time */
#include <unistd.h>		/* sleep */

#define NUM_THREADS (15)
#define TARGET_NUMBER (10000000007)

typedef struct thread_data_s
{
	size_t start_range;
	size_t end_range;
	size_t partial_sum;
} thread_data_t;

static pthread_t g_threads[NUM_THREADS];
static thread_data_t g_thread_data[NUM_THREADS];

static void CreateThreads();
static void* CalculateSumOfDivisors(void* thread_data);
static void ShowResults();

int main(void)
{
	time_t start_time = 0;
	time_t end_time = 0;
	
	printf("Calculating sum of divisors for %lu using %d threads\n", 
	       TARGET_NUMBER, NUM_THREADS);
	
	start_time = time(NULL);
	CreateThreads();
	end_time = time(NULL);
	
	printf("Total execution time: %ld seconds\n", end_time - start_time);
	
	ShowResults();
	
	return 0;
}

static void CreateThreads()
{
	size_t i = 0;
	size_t range_size = 0;
	
	range_size = TARGET_NUMBER / NUM_THREADS;
	
	for (i = 0; i < NUM_THREADS; ++i)
	{
		g_thread_data[i].start_range = (i * range_size) + 1;
		g_thread_data[i].end_range = (i + 1) * range_size;
		g_thread_data[i].partial_sum = 0;
		
		if (i == NUM_THREADS - 1)
		{
			g_thread_data[i].end_range = TARGET_NUMBER;
		}
		
		while (0 != pthread_create(&g_threads[i], NULL, CalculateSumOfDivisors, 
		                          &g_thread_data[i]))
		{
			sleep(1);
		}
	}
	
	printf("All %d threads created\n", NUM_THREADS);
	
	for (i = 0; i < NUM_THREADS; ++i)
	{
		pthread_join(g_threads[i], NULL);
	}
	
	printf("All threads completed\n");
}

static void* CalculateSumOfDivisors(void* thread_data)
{
	thread_data_t* data = NULL;
	size_t i = 0;
	size_t sum = 0;
	
	data = (thread_data_t*)thread_data;
	
	for (i = data->start_range; i <= data->end_range; ++i)
	{
		if (0 == TARGET_NUMBER % i)
		{
			sum += i;
		}
	}
	
	data->partial_sum = sum;
	
	return NULL;
}

static void ShowResults()
{
	size_t i = 0;
	size_t total_sum = 0;
	size_t divisor_count = 0;
	
	printf("\nResults:\n");
	
	for (i = 0; i < NUM_THREADS; ++i)
	{
		if (g_thread_data[i].partial_sum > 0)
		{
			printf("Thread %lu found divisors: %lu\n", i, g_thread_data[i].partial_sum);
			divisor_count++;
		}
		total_sum += g_thread_data[i].partial_sum;
	}
	
	printf("\nSum of divisors for %lu: %lu\n", TARGET_NUMBER, total_sum);
	printf("Threads that found divisors: %lu out of %d\n", divisor_count, NUM_THREADS);
}