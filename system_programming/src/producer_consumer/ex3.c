/**************************************
Exercise: 	System Programming - Producer Consumer Ex 3
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

#include "sll.h" /* SLLCreate */

#define BUFFER_EMPTY (0)
#define BUFFER_FULL (1)

#define NUM_PRODUCERS (3)
#define NUM_CONSUMERS (3)

#define MAX_VALUE (10)

/* Static global variables */
static sll_t* g_list = NULL;
static pthread_mutex_t mutex;
static sem_t items_sem;

static void* ProducerMsg(void* arg);
static void* ConsumerMsg(void* arg);
static void CreateThreads(pthread_t producers[],pthread_t consumers[]);
static void JoinThreads(pthread_t producers[],pthread_t consumers[]);

int main(void)
{
	pthread_t producers[NUM_PRODUCERS] = {0};
	pthread_t consumers[NUM_CONSUMERS] = {0};

    g_list = SLLCreate();
    if(!g_list)
    {
        return -1;
    }

	srand(time(NULL));

	printf("Starting Multiple Producers-Consumers with One mutex  \n\n");
	
    /* Init mutex and semaphore */
    pthread_mutex_init(&mutex,NULL);
    sem_init(&items_sem,0,0);
 
    CreateThreads(producers,consumers);
    JoinThreads(producers,consumers);
 
	printf("\nAll messages processed successfully!\n");
	
    /* Clean Memory */
    pthread_mutex_destroy(&mutex);
    sem_destroy(&items_sem);
    SLLDestroy(g_list);

	return EXIT_SUCCESS;
}


static void* ProducerMsg(void* arg)
{
    size_t i = 0;
	int* message = NULL;
	size_t index = (size_t)arg;

    message = (int*)malloc(sizeof(int));
    if(!message)
    {
        return NULL;
    }

    *message = rand() % MAX_VALUE;

    pthread_mutex_lock(&mutex);
    SLLInsert(SLLEnd(g_list),message);
    printf("Producer [%lu] - Message:%lu wrote %d\n",index + 1, i + 1, *message);
    pthread_mutex_unlock(&mutex);

    sem_post(&items_sem); /* Signal that an item was added */

	return NULL;
}

static void* ConsumerMsg(void* arg)
{
	size_t i = 0;
	int* message = NULL;
    size_t index = (size_t)arg;
    sll_iter_t iter = {0};

    sem_wait(&items_sem); /* Wait for an item to be available */

    pthread_mutex_lock(&mutex);

    iter = SLLBegin(g_list);
    message = (int*)SLLGetData(iter);
    SLLRemove(iter);
    printf("Consumer [%lu] - Message:%lu read %d\n",index + 1, i + 1, *message);
    
    pthread_mutex_unlock(&mutex);

    free(message);

	return NULL;
}

static void CreateThreads(pthread_t producers[],pthread_t consumers[])
{
  
    size_t i = 0;

    for(i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_create(&consumers[i], NULL, ConsumerMsg, (void*)i);
    }

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_create(&producers[i], NULL, ProducerMsg, (void*)i);
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