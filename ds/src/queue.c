/**************************************
Exercise: 	DS - Queue
Date:		3/7/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/


#include <stddef.h>    /* size_t */
#include "queue.h"
#include "sll.h"
#include <stdlib.h>

struct queue
{
    sll_t* queue;
};


queue_t *QueueCreate(void)
{
    queue_t* q = (queue_t *)malloc(sizeof(queue_t));
    if (NULL == q)
    {
        return NULL;
    }

    q->queue = SLLCreate();        
    if (NULL == q->queue)
    {
        free(q);
        return NULL;
    }

    return q;                        
}


void QueueDestroy(queue_t *queue)
{
    (void)queue; /* Prevent unused parameter warning */
    /* TODO: Free memory and cleanup */
}


int QueueEnqueue(queue_t *queue, void *data)
{
    (void)queue;
    (void)data;
    return 0;
}


void QueueDequeue(queue_t *queue)
{
    (void)queue;
    /* TODO: Remove front element */
}


int QueueIsEmpty(const queue_t *queue)
{
    (void)queue;
    return 1; /* Assume empty for now */
}


size_t QueueSize(const queue_t *queue)
{
    (void)queue;
    return 0; /* Placeholder value */
}


void *QueuePeek(const queue_t *queue)
{
    (void)queue;
    return NULL; /* Placeholder */
}


queue_t *QueueAppend(queue_t *src, queue_t *dst)
{
    (void)src;
    return dst; /* Return destination as per contract */
}
