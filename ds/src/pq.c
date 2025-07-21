/**************************************
Exercise: 	DS - PQ
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		
**************************************/

#include <assert.h>   /* assert 		*/
#include <stdlib.h>   /* malloc, free 	*/

#include "pq.h"       /* PQCreate 		*/

struct pq
{
    sortedl_t* slist;
};

pq_t* PQCreate(int (*comp)(const void* data1, const void* data2))
{
    pq_t *pq = NULL;
    sortedl_t *inner = NULL;

    if (NULL == comp)
    {
        return NULL;
    }

    pq = (pq_t*)malloc(sizeof(pq_t));
    if (NULL == pq)
    {
        return NULL;
    }

    inner = SortedLCreate(comp);
    if (NULL == inner)
    {
        free(pq);
        pq = NULL;
        return NULL;
    }

    pq->slist = inner;
    return pq;
}

void PQDestroy(pq_t* pq)
{
    assert(pq);

    SortedLDestroy(pq->slist);
  
  	pq->slist = NULL;
  	
    free(pq);
}

int PQEnqueue(pq_t* pq, const void* data)
{
    sorted_iter_t inserted = {0};
    sorted_iter_t end_iter = {0};

    assert(pq);

    inserted = SortedLInsert(pq->slist, (void*)data);
    end_iter = SortedLEnd(pq->slist);

    if (SortedLIsEqual(inserted, end_iter))
    {
        return 1;
    }

    return 0;
}

void* PQDequeue(pq_t* pq)
{
    assert(pq);

    if (PQIsEmpty(pq))
    {
        return NULL;
    }

    return SortedLPopBack(pq->slist);
}

void* PQPeek(const pq_t* pq)
{
    sorted_iter_t end = {0};
    sorted_iter_t last = {0};

    assert(pq);
    
    if (PQIsEmpty(pq))
    {
        return NULL;
    }

    end = SortedLEnd(pq->slist);
    last = SortedLPrev(end);

    return SortedLGetData(last);
}

int PQIsEmpty(const pq_t* pq)
{
   	assert(pq);
   	
    return SortedLIsEmpty(pq->slist);
}

size_t PQSize(const pq_t* pq)
{
    assert(pq);
    
    return SortedLSize(pq->slist);
}

void PQClear(pq_t* pq)
{
	assert(pq);

    while (!PQIsEmpty(pq))
    {
    	PQDequeue(pq);
    }
}

void PQErase(pq_t* pq, int (*is_match_func)(const void* data, const void* param), const void* param)
{
    sorted_iter_t begin_iter = {0};
    sorted_iter_t end_iter = {0};
    sorted_iter_t found_iter = {0};

    begin_iter = SortedLBegin(pq->slist);
    end_iter = SortedLEnd(pq->slist);
    found_iter = SortedLFindIf(begin_iter, end_iter, is_match_func, param);

    if (!SortedLIsEqual(found_iter, end_iter))
    {
        SortedLRemove(found_iter);
    }
}



