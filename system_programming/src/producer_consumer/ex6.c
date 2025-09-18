/**************************************
Exercise: 	System Programming - Producer Consumer Ex 6
Date:		18/09/2025
Developer:	Tal Hindi
Reviewer: 	Meir Avital
Status:		In Progress
**************************************/

#define _POSIX_C_SOURCE (200112L)

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_t */
#include <stdlib.h>     /* rand */
#include <time.h>       /* time */
#include <semaphore.h>  /* sem_t */
#include <assert.h>     /* assert */

#define NUM_PRODUCERS       (1)
#define NUM_CONSUMERS       (10)
#define MAX_VALUE           (100)

/* Static global variables */
static pthread_mutex_t mutex;
static pthread_cond_t cond;
static sem_t done_sem;

static int g_message = 0;
static int g_ready = 0;

static void* ProducerMsg(void* arg);
static void* ConsumerMsg(void* arg);
static void CreateThreads(pthread_t producers[], pthread_t consumers[]);
static void JoinThreads(pthread_t producers[], pthread_t consumers[]);

int main(void)
{
	size_t i = 0;
	pthread_t producers[NUM_PRODUCERS] = {0};
	pthread_t consumers[NUM_CONSUMERS] = {0};

	srand((unsigned int)time(NULL));

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	if (0 != sem_init(&done_sem, 0, 0))
	{
		perror("sem_init");
		return EXIT_FAILURE;
	}

	printf("Starting Single Producer - Multiple Consumers EX6 \n\n");

	CreateThreads(producers, consumers);
	JoinThreads(producers, consumers);

	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		pthread_join(consumers[i], NULL);
	}

	printf("\nAll consumers finished successfully!\n");

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	sem_destroy(&done_sem);

	return EXIT_SUCCESS;
}

static void* ProducerMsg(void* arg)
{
	size_t i = 0;
	(void)arg;

	pthread_mutex_lock(&mutex);

	g_message = rand() % MAX_VALUE;
	g_ready = 1;

	printf("Producer wrote value %d\n", g_message);

	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);

	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		sem_wait(&done_sem);
	}

	return NULL;
}

static void* ConsumerMsg(void* arg)
{
	size_t index = (size_t)arg;

	pthread_mutex_lock(&mutex);

	while (0 == g_ready)
	{
		pthread_cond_wait(&cond, &mutex);
	}

	printf("Consumer [%lu] read %d\n", index + 1, g_message);

	sem_post(&done_sem);

	pthread_mutex_unlock(&mutex);

	return NULL;
}

static void CreateThreads(pthread_t producers[], pthread_t consumers[])
{
	size_t i = 0;

	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		pthread_create(&consumers[i], NULL, ConsumerMsg, (void*)i);
	}

	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		pthread_create(&producers[i], NULL, ProducerMsg, (void*)i);
	}
}

static void JoinThreads(pthread_t producers[], pthread_t consumers[])
{
	size_t i = 0;

	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		pthread_join(producers[i], NULL);
	}
}
