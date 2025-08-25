/**************************************
Exercise: 	DS - PQ
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		Approved
**************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "pq.h" /* PQCreate */

struct pq
{
    heap_t* heap;
};

pq_t* PQCreate(int (*comp)(const void* data1, const void* data2))
{
    pq_t* pq = NULL;

	assert(comp);
	
    pq = (pq_t*)malloc(sizeof(pq_t));
    if (NULL == pq)
    {
        return NULL;
    }

    pq->heap = HeapCreate(comp);
    if (NULL == pq->heap)
    {
        free(pq);
        return NULL;
    }

    return pq;
}

void PQDestroy(pq_t* pq)
{
    assert(pq);

    HeapDestroy(pq->heap);
  
  	pq->heap = NULL;
  	
    free(pq);
}

int PQEnqueue(pq_t* pq, void* data)
{
    assert(pq);

    return HeapPush(pq->heap,data);
}

void* PQDequeue(pq_t* pq)
{
    void* data = NULL;

    assert(pq);
    assert(!PQIsEmpty(pq));

    data = HeapPeek(pq->heap);
    HeapPop(pq->heap);

    return data;
}

void* PQPeek(const pq_t* pq)
{
    assert(pq);
   	
    return HeapPeek(pq->heap);
}

int PQIsEmpty(const pq_t* pq)
{
   	assert(pq);
   	
    return HeapIsEmpty(pq->heap);
}

size_t PQSize(const pq_t* pq)
{
    assert(pq);
    
    return HeapSize(pq->heap);
}

void PQClear(pq_t* pq)
{
	assert(pq);

    while (!PQIsEmpty(pq))
    {
    	PQDequeue(pq);
    }
}

void* PQErase(pq_t* pq, int (*is_match_func)(const void* data, const void* param), const void* param)
{
    assert(is_match_func);
    assert(pq);

	return HeapRemove(pq->heap,param,is_match_func);	
}




