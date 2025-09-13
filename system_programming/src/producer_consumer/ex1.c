/**************************************
Exercise: 	System Programming - Producer Consumer 
Date:		13/09/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		In Progress
**************************************/

#define _POSIX_C_SOURCE (200112L)

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_t */
#include <stdlib.h>     /* rand */
#include <time.h>       /* time */
#include <assert.h>     /* assert */

#define MESSAGE_COUNT (10)
#define MAX_VALUE (10)

#define BUFFER_EMPTY (0)
#define BUFFER_FULL (1)


volatile int g_buffer_state = BUFFER_EMPTY;
volatile int g_message = 0;

static void* ProducerMsg(void* arg);
static void* ConsumerMsg(void* arg);


int main(void)
{
	pthread_t producer = 0;
	pthread_t consumer = 0;
	
	srand(time(NULL));
	printf("Starting Producer-Consumer with Spin Lock\nMessage count: %d\n\n", MESSAGE_COUNT);
	
	pthread_create(&consumer, NULL, ConsumerMsg, NULL);
	pthread_create(&producer, NULL, ProducerMsg, NULL);
	
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
	
	printf("\nAll messages processed successfully!\n");
	
	return EXIT_SUCCESS;
}


static void* ProducerMsg(void* arg)
{
	size_t i = 0;
	int message = 0;
	
	(void)arg;
	
	for (i = 0; i < MESSAGE_COUNT; ++i)
	{
		/* Spinlock */
		while (BUFFER_FULL == __sync_fetch_and_add(&g_buffer_state, 0))
		{
			/* Busy wait */
		}
		
		message = rand() % MAX_VALUE;
		g_message = message;
		printf("Producer Message %lu: wrote %d\n", i + 1, message);
		
		__sync_lock_test_and_set(&g_buffer_state, BUFFER_FULL);
	}
	
	printf("[Producer] Finished producing..\n");
	
	return NULL;
}

static void* ConsumerMsg(void* arg)
{
	size_t i = 0;
	int message = 0;
	
	(void)arg;
	
	for (i = 0; i < MESSAGE_COUNT; ++i)
	{
		/* Spinlock */
		while (BUFFER_EMPTY == __sync_fetch_and_add(&g_buffer_state, 0))
		{
			/* Busy wait  */
		}
		
		message = g_message;
		printf("Consumer Message %lu: read %d\n", i + 1, message);
		
		__sync_lock_test_and_set(&g_buffer_state, BUFFER_EMPTY);
	}
	
	printf("[Consumer] Finished consuming.\n");
	
	return NULL;
}