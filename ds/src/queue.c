/**************************************
Exercise: 	DS - Queue
Date:		3/7/2025
Developer:	Tal Hindi
Reviewer: 	Lotem Kitaroo
Status:		Approved
**************************************/


#include <stddef.h>    /* size_t */		
#include <stdlib.h>	
#include <stddef.h>
#include <assert.h>

#include "queue.h"

typedef struct queue queue_t;

struct queue
{
    sll_t* list;
};


/*helper func*/
static int IsQueueInvalid(const queue_t *queue)
{
    return (NULL == queue) || (NULL == queue->list);
}

queue_t *QueueCreate(void)
{
	queue_t* q = NULL;
	
    q = (queue_t *)malloc(sizeof(queue_t));
    if (NULL == q)
    {
        return NULL;
    }

    q->list = SLLCreate();        
    if (NULL == q->list)
    {
        free(q);
        return NULL;
    }

    return q;                        
}


void QueueDestroy(queue_t* queue)
{
	if (!IsQueueInvalid(queue)) { SLLDestroy(queue->list); free(queue); }
}



int QueueEnqueue(queue_t *queue, void *data)
{
    sll_iter_t tail_it     = NULL;
    sll_iter_t new_node_it = NULL;

    assert(!IsQueueInvalid(queue));

    tail_it     = SLLEnd(queue->list);
    new_node_it = SLLInsert(tail_it, data);

    return (new_node_it == tail_it) ? 1 /* FAIL */ : 0 /* OK */;
}

void QueueDequeue(queue_t *queue)
{
   	assert(!IsQueueInvalid(queue));
   	
   	SLLRemove(SLLBegin(queue->list));
	
}

int QueueIsEmpty(const queue_t* queue)
{
    assert(!IsQueueInvalid(queue));

    return SLLIsEmpty(queue->list);
}


size_t QueueSize(const queue_t* queue)
{
	assert(!IsQueueInvalid(queue));
  
    return SLLCount(queue->list);
}


void* QueuePeek(const queue_t* queue)
{
    assert(!IsQueueInvalid(queue));

    return QueueIsEmpty(queue) ? NULL : SLLGetData(SLLBegin(queue->list));
}


queue_t *QueueAppend(queue_t *src, queue_t *dst)
{
    dst->list = SLLAppend(src->list, dst->list);
    return dst; 
}
