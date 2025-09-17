/**************************************
Exercise: 	System Programming - Producer Consumer Ex 3
Date:		13/09/2025
Developer:	Tal Hindi
Reviewer: 	Meir Avital
Status:		Approved
**************************************/

#define _POSIX_C_SOURCE (200112L)

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_t */
#include <stdlib.h>     /* rand_r */
#include <time.h>       /* time */

#include "sll.h" /* SLLCreate */

#define NUM_PRODUCERS   (3)
#define NUM_CONSUMERS   (3)
#define MSGS_PER_PRODUCER (2)
#define MAX_VALUE       (10)

/* Static global variables */
static sll_t* g_list = NULL;
static pthread_mutex_t mutex;
static pthread_cond_t cond;

static size_t g_messages_left = NUM_PRODUCERS * MSGS_PER_PRODUCER;

static void* ProducerMsg(void* arg);
static void* ConsumerMsg(void* arg);
static void CreateThreads(pthread_t producers[],pthread_t consumers[]);
static void JoinThreads(pthread_t producers[],pthread_t consumers[]);

int main(void)
{
    pthread_t producers[NUM_PRODUCERS] = {0};
    pthread_t consumers[NUM_CONSUMERS] = {0};

    g_list = SLLCreate();
    if (!g_list)
    {
        return -1;
    }

    srand(time(NULL));

    printf("Starting Multiple Producers-Consumers with One mutex \nTotal messages: %lu\n\n",
           g_messages_left);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    CreateThreads(producers, consumers);
    JoinThreads(producers, consumers);

    for (size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_join(consumers[i], NULL);
    }

    printf("\nAll messages processed successfully!\n");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    SLLDestroy(g_list);

    return EXIT_SUCCESS;
}

static void* ProducerMsg(void* arg)
{
    size_t index = (size_t)arg;
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();

    for (size_t i = 0; i < MSGS_PER_PRODUCER; ++i)
    {
        int* message = (int*)malloc(sizeof(int));
        if (!message)
        {
            return NULL;
        }

        *message = rand_r(&seed) % MAX_VALUE;

        pthread_mutex_lock(&mutex);

        SLLInsert(SLLEnd(g_list), message);
        printf("Producer [%lu] wrote %d (msg %lu)\n", index + 1, *message, i + 1);

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

static void* ConsumerMsg(void* arg)
{
    size_t index = (size_t)arg;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (SLLIsEmpty(g_list) && g_messages_left > 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        if (0 == g_messages_left)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        sll_iter_t iter = SLLBegin(g_list);
        int* message = (int*)SLLGetData(iter);
        SLLRemove(iter);

        --g_messages_left;

        pthread_mutex_unlock(&mutex);

        printf("Consumer [%lu] read %d, messages left: %lu\n",
               index + 1, *message, g_messages_left);

        free(message);
    }

    return NULL;
}

static void CreateThreads(pthread_t producers[], pthread_t consumers[])
{
    for (size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_create(&consumers[i], NULL, ConsumerMsg, (void*)i);
    }

    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        pthread_create(&producers[i], NULL, ProducerMsg, (void*)i);
    }
}

static void JoinThreads(pthread_t producers[], pthread_t consumers[])
{
    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        pthread_join(producers[i], NULL);
    }
}
