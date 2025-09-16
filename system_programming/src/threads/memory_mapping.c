/**************************************
Exercise: 	System Programming - Threads Memory Map
Date:		15/09/2025
Developer:	Tal Hindi
Reviewer:	
Status:		In Progress
**************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc, free */
#include <pthread.h>    /* pthread_create, pthread_join */
#include <assert.h>     /* assert */

#define NUM_THREADS (2)

/* global variable */
static int g_global_var = 42;

/* TLS variable */
__thread int g_tls_var = 7;

/* typedefs */
typedef struct thread_args
{
	size_t thread_id;
} thread_args_t;

/* static prototypes */
static void* ThreadFunc(void* arg);

/* main */
int main(void)
{
	pthread_t threads[NUM_THREADS] = {0};
	thread_args_t args[NUM_THREADS] = {0};
	size_t i = 0;
	int* heap_var = NULL;

	heap_var = (int*)malloc(sizeof(int));
	if (NULL == heap_var)
	{
		perror("malloc failed");
		return 1;
	}

	*heap_var = 99;

	printf("Main Thread:\n");
	printf("  &g_global_var = %p (Global)\n", (void*)&g_global_var);
	printf("  heap_var      = %p (Heap)\n", (void*)heap_var);
	printf("  &g_tls_var    = %p (TLS)\n", (void*)&g_tls_var);

	for (i = 0; i < NUM_THREADS; i++)
	{
		args[i].thread_id = i;
		if (0 != pthread_create(&threads[i], NULL, ThreadFunc, (void*)&args[i]))
		{
			perror("pthread_create failed");
			free(heap_var);
			return 1;
		}
	}

	for (i = 0; i < NUM_THREADS; i++)
	{
		if (0 != pthread_join(threads[i], NULL))
		{
			perror("pthread_join failed");
			free(heap_var);
			return 1;
		}
	}

	free(heap_var);

	return 0;
}

/* static implementations */
static void* ThreadFunc(void* arg)
{
	thread_args_t* args = NULL;
	int local_stack_var = 0;

	assert(NULL != arg);
	args = (thread_args_t*)arg;

	local_stack_var = (int)args->thread_id * 10;
	g_tls_var = (int)args->thread_id + 100;

	printf("Thread %lu:\n", args->thread_id);
	printf("  &local_stack_var = %p (Stack)\n", (void*)&local_stack_var);
	printf("  &g_tls_var       = %p (TLS)\n", (void*)&g_tls_var);

	return NULL;
}
