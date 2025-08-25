/**************************************
Exercise:   DS – Priority Queue  (heap-based)
Date:       20/7/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     Approved
**************************************/

#ifndef __ILRD_PQ_H__
#define __ILRD_PQ_H__

#include <stddef.h>   /* size_t */

#include "heap.h"     /* HeapCreate */

typedef struct pq pq_t;

/*---------------------------------------------------------------------------
 * PQCreate
 * Make an empty priority queue.
 * The compare function decides the order (higher return value ⇒ higher priority).
 * Return: pointer to a new queue, or NULL on error.
 * Time:   O(1)
 *-------------------------------------------------------------------------*/
pq_t* PQCreate(int (*cmp)(const void* data1, const void* data2));

/*---------------------------------------------------------------------------
 * PQDestroy
 * Free the queue object itself.  Does NOT free user data.
 * Time:   O(n)   (destroys the entire heap)
 *-------------------------------------------------------------------------*/
void PQDestroy(pq_t* pq);

/*---------------------------------------------------------------------------
 * PQEnqueue
 * Add a new item.
 * Return: 0 on success, non-zero on allocation failure.
 * Time:   O(log n)   (heap insert)
 *-------------------------------------------------------------------------*/
int PQEnqueue(pq_t* pq, void* data);

/*---------------------------------------------------------------------------
 * PQDequeue
 * Remove and return the highest-priority item.
 * Return: data pointer, or NULL if the queue is empty.
 * Time:   O(log n)   (heap pop)
 *-------------------------------------------------------------------------*/
void* PQDequeue(pq_t* pq);

/*---------------------------------------------------------------------------
 * PQPeek
 * Return the highest-priority item without removing it.
 * Return: data pointer, or NULL if the queue is empty.
 * Time:   O(1)
 *-------------------------------------------------------------------------*/
void* PQPeek(const pq_t* pq);

/*---------------------------------------------------------------------------
 * PQIsEmpty
 * Return: non-zero if empty, 0 otherwise.
 * Time:   O(1)
 *-------------------------------------------------------------------------*/
int PQIsEmpty(const pq_t* pq);

/*---------------------------------------------------------------------------
 * PQSize
 * Return the number of items currently in the queue.
 * Time:   O(1)
 *-------------------------------------------------------------------------*/
size_t PQSize(const pq_t* pq);

/*---------------------------------------------------------------------------
 * PQClear
 * Remove every item (does not free the user data).
 * Time:   O(n log n)   (repeated heap pops)
 *-------------------------------------------------------------------------*/
void PQClear(pq_t* pq);

/*---------------------------------------------------------------------------
 * PQErase
 * Remove the first item for which
 *     is_match_func(data, param)  !=  0
 * Return: pointer to the removed data if found, NULL otherwise.
 * Time:   O(n)   (linear scan inside the heap)
 *-------------------------------------------------------------------------*/
void* PQErase(pq_t* pq,
              int (*is_match_func)(const void* data, const void* param),
              const void* param);

#endif /* __ILRD_PQ_H__ */
