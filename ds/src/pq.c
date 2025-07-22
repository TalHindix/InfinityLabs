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
    pq_t* pq = NULL;

	assert(comp);
	
    pq = (pq_t*)malloc(sizeof(pq_t));
    if (NULL == pq)
    {
        return NULL;
    }

    pq->slist = SortedLCreate(comp);
    if (NULL == pq->slist)
    {
        free(pq);
        pq = NULL;
        return NULL;
    }

    return pq;
}

void PQDestroy(pq_t* pq)
{
    assert(pq);

    SortedLDestroy(pq->slist);
  
  	pq->slist = NULL;
  	
    free(pq);
}

int PQEnqueue(pq_t* pq, void* data)
{
    sorted_iter_t inserted = {0};
    sorted_iter_t end_iter = {0};

    assert(pq);

    inserted = SortedLInsert(pq->slist, (void*)data);
    end_iter = SortedLEnd(pq->slist);

    return SortedLIsEqual(inserted, end_iter);
    
}

void* PQDequeue(pq_t* pq)
{
    assert(pq);

    return SortedLPopBack(pq->slist);
}

void* PQPeek(const pq_t* pq)
{
    assert(pq);
   
    return SortedLGetData(SortedLPrev(SortedLEnd(pq->slist)));
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

int PQErase(pq_t* pq, int (*is_match_func)(const void* data, const void* param), const void* param)
{
    sorted_iter_t begin_iter = {0};
    sorted_iter_t end_iter = {0};
    sorted_iter_t to_remove = {0};
	
	assert(pq);
	assert(is_match_func);
	
    begin_iter = SortedLBegin(pq->slist);
    end_iter = SortedLEnd(pq->slist);
    to_remove = SortedLFindIf(begin_iter, end_iter, is_match_func, param);
    
    if (!SortedLIsEqual(to_remove, end_iter))
    {
        SortedLRemove(to_remove);
        return 0;
    }
    
    return 1;
}



