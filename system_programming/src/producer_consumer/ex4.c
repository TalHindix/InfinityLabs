/**************************************
Exercise: 	System Programming - Producer Consumer Ex 4
Date:		13/09/2025
Developer:	Tal Hindi
Reviewer: 	Meir Avital
Status:		Approved
**************************************/

#define _POSIX_C_SOURCE (200112L)

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_t */
#include <stdlib.h>     /* rand */
#include <time.h>       /* time */
#include <semaphore.h>  /* semaphore */

#include "cbuff.h" /* CBuffCreate */

#define NUM_PRODUCERS (3)
#define NUM_CONSUMERS (3)
#define NUM_ITEMS (1)
#define MAX_VALUE (10)

/* Static global variables */
static cbuff_t* g_cbuffer = NULL;
static pthread_mutex_t mutex;
static sem_t write_sem;
static sem_t read_sem;

static void* ProducerMsg(void* arg);
static void* ConsumerMsg(void* arg);
static void CreateThreads(pthread_t producers[],pthread_t consumers[]);
static void JoinThreads(pthread_t producers[],pthread_t consumers[]);

int main(void)
{
	pthread_t producers[NUM_PRODUCERS] = {0};
	pthread_t consumers[NUM_CONSUMERS] = {0};

    g_cbuffer = CBuffCreate(NUM_ITEMS);
    if(!g_cbuffer)
    {
        return -1;
    }

	srand(time(NULL));

	printf("Starting Multiple Producers-Consumers with One mutex  \n\n");
	
    /* Init mutex and semaphores */
    pthread_mutex_init(&mutex,NULL);
    sem_init(&write_sem, 0, NUM_ITEMS);
    sem_init(&read_sem, 0, 0);

 
    CreateThreads(producers,consumers);
    JoinThreads(producers,consumers);
 
	printf("\nAll messages processed successfully!\n");
	
    /* Clean Memory */
    pthread_mutex_destroy(&mutex);
    sem_destroy(&write_sem);
    sem_destroy(&read_sem);
    CBuffDestroy(g_cbuffer);

	return EXIT_SUCCESS;
}


static void* ProducerMsg(void* arg)
{
    size_t i = 0;
	int message = 0;
	size_t index = (size_t)arg;
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();

    for(i = 0; i < NUM_ITEMS; ++i)
    {
        message = rand_r(&seed) % MAX_VALUE;
        
        sem_wait(&write_sem); /* Wait for space to write */
        
        pthread_mutex_lock(&mutex);

        CBuffWrite(g_cbuffer, &message, sizeof(int));
        
        pthread_mutex_unlock(&mutex);

        printf("Producer [%lu] - Message:%lu wrote %d\n", index + 1, i + 1, message);
        
        sem_post(&read_sem); /* Signal that an item is available to read */
    }

	return NULL;
}

static void* ConsumerMsg(void* arg)
{
	size_t i = 0;
	int message = 0;
    size_t index = (size_t)arg;

    for(i = 0; i < NUM_ITEMS; ++i)
    {
        sem_wait(&read_sem); /* Wait for an item to be available */
        
        pthread_mutex_lock(&mutex);

        CBuffRead(g_cbuffer, &message, sizeof(int));
        
        pthread_mutex_unlock(&mutex);
        printf("Consumer [%lu] - Message:%lu read %d\n", index + 1, i + 1, message);
        
        sem_post(&write_sem); /* Signal that space is available for writing */
    }

	return NULL;
}

static void CreateThreads(pthread_t producers[],pthread_t consumers[])
{
  
    size_t i = 0;

    for(i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_create(&consumers[i], NULL, ConsumerMsg, (void*)i);
    }

    for (i = 0; i < NUM_PRODUCERS; ++i)
    {
        pthread_create(&producers[i], NULL, ProducerMsg, (void*)i);
    }
}

static void JoinThreads(pthread_t producers[],pthread_t consumers[])
{
    size_t i = 0;

    for (i = 0; i < NUM_PRODUCERS; ++i)
    {
        pthread_join(producers[i], NULL);
    }
    
    for(i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_join(consumers[i], NULL);
    }

}