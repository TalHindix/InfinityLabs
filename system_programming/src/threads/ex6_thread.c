/**************************************
Exercise: 	System Programming - 100k Threads Ex 6
Date:		08/09/2025
Developer:	Tal Hindi
Reviewer: 	Lotem Kitaroo
Status:		In Progress
**************************************/

#include <stdio.h>		/* printf */
#include <pthread.h>	/* pthread_create */
#include <time.h>		/* clock_gettime */
#include <omp.h>        /* #pragma omp parallel */

#define NUM_THREADS (16)
#define TARGET_NUMBER (1000000000UL)

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
static size_t SimpleSumDivisors();

int main(void)
{
	/* Variable declarations for timing and results */
	struct timespec start_time, end_time;
	struct timespec simple_start_time, simple_end_time;
	double elapsed_time, simple_elapsed_time;
	size_t simple_result = 0;
    size_t i = 0;
	
	printf("Calculating sum of divisors for %lu using %d threads\n", 
	       TARGET_NUMBER, NUM_THREADS);

	/* Multithreaded version using pthreads */
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	CreateThreads();
	ShowResults();
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	
	elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
	               (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
	
	printf("Multithreaded execution time: %.6f seconds\n", elapsed_time);
	
	/* Single-threaded version for comparison */
	printf("\n--- Simple Version ---\n");
	clock_gettime(CLOCK_MONOTONIC, &simple_start_time);
	simple_result = SimpleSumDivisors();
	clock_gettime(CLOCK_MONOTONIC, &simple_end_time);
	
	simple_elapsed_time = (simple_end_time.tv_sec - simple_start_time.tv_sec) + 
	                      (simple_end_time.tv_nsec - simple_start_time.tv_nsec) / 1000000000.0;
	
	printf("Simple execution time: %.6f seconds\n", simple_elapsed_time);
	printf("Simple sum of divisors: %lu\n", simple_result);

	/* OpenMP parallelized version */
    simple_result = 0;
    clock_gettime(CLOCK_MONOTONIC, &simple_start_time);
    
    printf("\n--- Simple Version with OpenMP ---\n");
    printf("OpenMP using %d threads\n", omp_get_max_threads());

    #pragma omp parallel for reduction(+:simple_result)
    for (i = 1; i <= TARGET_NUMBER; ++i)
    {
        if (0 == TARGET_NUMBER % i)
        {
            simple_result += i;
        }
    }
    
	clock_gettime(CLOCK_MONOTONIC, &simple_end_time);

	simple_elapsed_time = (simple_end_time.tv_sec - simple_start_time.tv_sec) + 
	                      (simple_end_time.tv_nsec - simple_start_time.tv_nsec) / 1000000000.0;
	
	printf("Simple with OpenMP execution time: %.6f seconds\n", simple_elapsed_time);
	printf("Simple sum OpenMP of divisors: %lu\n", simple_result);
	
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
		
		pthread_create(&g_threads[i], NULL, CalculateSumOfDivisors, &g_thread_data[i]);
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
	
	printf("\nMultithreaded sum of divisors for %lu: %lu\n", TARGET_NUMBER, total_sum);
	printf("Threads that found divisors: %lu out of %d\n", divisor_count, NUM_THREADS);
}

static size_t SimpleSumDivisors()
{
	size_t i = 0;
	size_t sum = 0;
	
	for (i = 1; i <= TARGET_NUMBER; ++i)
	{
		if (0 == TARGET_NUMBER % i)
		{
			sum += i;
		}
	}
	
	return sum;
}