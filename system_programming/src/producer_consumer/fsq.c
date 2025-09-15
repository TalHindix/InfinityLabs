/**************************************
Exercise: 	DS - FSQ Thread SAFE
Date:		14/9/2025
Developer:	Tal Hindi
Reviewer: 	Guy Argaman
Status:		
**************************************/

#include <stddef.h>     /* size_t */
#include <stdlib.h>     /* malloc */
#include <assert.h>     /* assert */
#include <pthread.h>    /* pthread_t */
#include <semaphore.h>  /* semaphore */

#include "fsq.h" /* FSQCreate */

struct fsq
{
	sll_t* list;
	size_t capacity;
	pthread_mutex_t write_lock;
	pthread_mutex_t read_lock;
	sem_t enqueue_sem;
	sem_t dequeue_sem;
};

fsq_t* FSQCreate(size_t capacity)
{
	fsq_t* fsq = NULL;

	fsq = (fsq_t*)malloc(sizeof(fsq_t));
	if (NULL == fsq)
	{
		return NULL;
	}

	fsq->list = SLLCreate();
	if (NULL == fsq->list)
	{
		free(fsq);
		return NULL;
	}

	fsq->capacity = capacity;

	sem_init(&fsq->enqueue_sem, 0, capacity);
	sem_init(&fsq->dequeue_sem, 0, 0);

	pthread_mutex_init(&fsq->write_lock, NULL);
	pthread_mutex_init(&fsq->read_lock, NULL);

	return fsq;
}

void FSQDestroy(fsq_t* fsq)
{
	assert(fsq);

	SLLDestroy(fsq->list);
	sem_destroy(&fsq->enqueue_sem);
	sem_destroy(&fsq->dequeue_sem);
	pthread_mutex_destroy(&fsq->write_lock);
	pthread_mutex_destroy(&fsq->read_lock);
	free(fsq);
	
}

int FSQEnqueue(fsq_t* fsq, void* data)
{
    sll_iter_t new_node_iter = NULL;

    assert(fsq);

    sem_wait(&fsq->enqueue_sem);

    pthread_mutex_lock(&fsq->write_lock);

    new_node_iter = SLLInsert(SLLEnd(fsq->list), data);

    pthread_mutex_unlock(&fsq->write_lock);

    if (SLLIsEqual(SLLEnd(fsq->list), new_node_iter))
    {
        sem_post(&fsq->enqueue_sem);
        return -1;
    }

    sem_post(&fsq->dequeue_sem);
    return 0;
}


void* FSQDequeue(fsq_t* fsq)
{
    void* data = NULL;
    sll_iter_t head_iter = NULL;
    int is_singleton = 0;

    assert(fsq);

    sem_wait(&fsq->dequeue_sem);

    pthread_mutex_lock(&fsq->read_lock);

    is_singleton = SLLIsEqual(SLLNext(SLLBegin(fsq->list)), SLLEnd(fsq->list));

    if (is_singleton)
    {
        pthread_mutex_lock(&fsq->write_lock);
    }

    head_iter = SLLBegin(fsq->list);
    data = SLLGetData(head_iter);
    SLLRemove(head_iter);

    if (is_singleton)
    {
        pthread_mutex_unlock(&fsq->write_lock);
    }

    pthread_mutex_unlock(&fsq->read_lock);

    sem_post(&fsq->enqueue_sem);

    return data;
}

int FSQIsEmpty(const fsq_t* fsq)
{
    int result = 0;

    assert(fsq);

    pthread_mutex_lock(&((fsq_t*)fsq)->read_lock);
    
    result = SLLIsEqual(SLLBegin(fsq->list), SLLEnd(fsq->list));
    
    pthread_mutex_unlock(&((fsq_t*)fsq)->read_lock);

    return result;
}

size_t FSQSize(const fsq_t* fsq)
{
	size_t result = 0;

	assert(fsq);

	pthread_mutex_lock(&((fsq_t*)fsq)->read_lock);
	
	result = SLLCount(fsq->list);
	
	pthread_mutex_unlock(&((fsq_t*)fsq)->read_lock);

	return result;
}

void* FSQPeek(const fsq_t* fsq)
{
	void* result = NULL;
	sll_iter_t first_real_node = NULL;

	assert(fsq);

	pthread_mutex_lock(&((fsq_t*)fsq)->read_lock);
	
	first_real_node = SLLNext(SLLBegin(fsq->list));
	result = SLLIsEqual(first_real_node, SLLEnd(fsq->list)) ? NULL : SLLGetData(first_real_node);
	
	pthread_mutex_unlock(&((fsq_t*)fsq)->read_lock);

	return result;
}
