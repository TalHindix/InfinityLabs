/**************************************
Exercise: 	System Programming - Producer Consumer Ex 5
Date:		13/09/2025
Developer:	Tal Hindi
Reviewer: 	Guy Argaman
Status:		In Progress
**************************************/

#define _POSIX_C_SOURCE (200112L)

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_t */
#include <stdlib.h>     /* rand */
#include <time.h>       /* time */
#include <semaphore.h>  /* semaphore */

#include "fsq.h" /* FSQCreate */

#define NUM_PRODUCERS (3)
#define NUM_CONSUMERS (3)

#define NUM_ITEMS (2)
#define QUEUE_CAPACITY (5)

#define MAX_VALUE (10)


/* global variables */
fsq_t* g_fsq = NULL;

static void* ProducerMsg(void* arg);
static void* ConsumerMsg(void* arg);
static void CreateThreads(pthread_t producers[],pthread_t consumers[]);
static void JoinThreads(pthread_t producers[],pthread_t consumers[]);

int main(void)
{
	pthread_t producers[NUM_PRODUCERS] = {0};
	pthread_t consumers[NUM_CONSUMERS] = {0};

    g_fsq = FSQCreate(QUEUE_CAPACITY);
  
    if(!g_fsq)
    {
        return -1;
    }

	srand(time(NULL));

	printf("Starting Multiple Producers-Consumers with 2 mutex and 2 Semphores in the struct  \n\n");
	
    CreateThreads(producers,consumers);
    JoinThreads(producers,consumers);
 
	printf("\nAll messages processed successfully!\n");
	
    /* Clean Memory */
    FSQDestroy(g_fsq);

	return EXIT_SUCCESS;
}

static void* ProducerMsg(void* arg)
{
	size_t i = 0;
	size_t thread_id = 0;
	int* message = NULL;
	
	thread_id = (size_t)arg;
	
	for (i = 0; i < NUM_ITEMS; ++i)
	{
		message = (int*)malloc(sizeof(int));
		if (NULL == message)
		{
			perror("malloc failed");
			return NULL;
		}
        
		*message = rand() % MAX_VALUE;
		
		FSQEnqueue(g_fsq, message);
		
		printf("Producer [%lu] - Item %lu: Produced %d\n", thread_id + 1, i + 1, *message);
	}
    return NULL;
}

static void* ConsumerMsg(void* arg)
{
	size_t i = 0;
	size_t thread_id = 0;
	int* message = NULL;
	
	thread_id = (size_t)arg;
	
	for (i = 0; i < NUM_ITEMS; ++i)
	{
		message = (int*)FSQDequeue(g_fsq);
		
		if (NULL != message)
		{
			printf("Consumer [%lu] - Item %lu: Consumed %d\n", thread_id + 1, i + 1, *message);
			free(message);
		}
	}
	
	return NULL;
}


static void CreateThreads(pthread_t producers[],pthread_t consumers[])
{
  
    size_t i = 0;

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_create(&producers[i], NULL, ProducerMsg, (void*)i);
    }

    for(i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_create(&consumers[i], NULL, ConsumerMsg, (void*)i);
    }

}

static void JoinThreads(pthread_t producers[],pthread_t consumers[])
{
    size_t i = 0;

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }
    
    for(i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_join(consumers[i], NULL);
    }

}

