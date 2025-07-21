/**************************************
Exercise: 	DS - PQ
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		
**************************************/

#ifndef ILRD_PQ_H
#define ILRD_PQ_H

#include <stddef.h> 	/* size_t 			*/
#include "sortedl.h"	/* SortedLCreate 	*/

typedef struct pq pq_t;

/*---------------------------------------------------------------------------
 * PQCreate
 * Create an empty priority queue ordered by user-supplied compare function.
 * Return: pointer to new PQ or NULL on failure / invalid comparator.
 * Complexity: O(1)
 *-------------------------------------------------------------------------*/
pq_t* PQCreate(int (*comp)(const void* data1, const void* data2)); /* O(1) */

/*---------------------------------------------------------------------------
 * PQDestroy
 * Destroys the PQ container. Does NOT free user data.
 * Complexity: O(n) due to internal list destruction.
 *-------------------------------------------------------------------------*/
void PQDestroy(pq_t* pq); /* O(n) */

/*---------------------------------------------------------------------------
 * PQEnqueue
 * Insert element keeping internal ordering by comparator.
 * Return: 0 on success, non-zero on allocation failure.
 * Complexity: O(n) for search + insert.
 *-------------------------------------------------------------------------*/
int PQEnqueue(pq_t* pq, const void* data); /* O(n) */

/*---------------------------------------------------------------------------
 * PQDequeue
 * PQDequeue highest priority element and return its data.
 * Return: data pointer or NULL if empty.
 * Complexity: O(1)
 *-------------------------------------------------------------------------*/
void* PQDequeue(pq_t* pq); /* O(1) */

/*---------------------------------------------------------------------------
 * PQPeek
 * Return (without removing) data pointer of highest priority element.
 * Return: data pointer or NULL if empty.
 * Complexity: O(1)
 *-------------------------------------------------------------------------*/
void* PQPeek(const pq_t* pq); /* O(1) */

/*---------------------------------------------------------------------------
 * PQIsEmpty
 * Return: non-zero if empty; 0 otherwise.
 * Complexity: O(1)
 *-------------------------------------------------------------------------*/
int PQIsEmpty(const pq_t* pq); /* O(1) */

/*---------------------------------------------------------------------------
 * PQSize
 * Return number of elements.
 * Complexity: O(n) (delegates to SortedLSize implementation)
 *-------------------------------------------------------------------------*/
size_t PQSize(const pq_t* pq); /* O(n) */

/*---------------------------------------------------------------------------
 * PQClear
 * Remove all elements (does not free payloads).
 * Complexity: O(n)
 *-------------------------------------------------------------------------*/
void PQClear(pq_t* pq); /* O(n) */

/*---------------------------------------------------------------------------
 * PQErase
 * Remove first element for which is_match_func(data, param) returns non-zero.
 * No return value per given API (silent if not found). Does not free payload.
 * Complexity: O(n)
 *-------------------------------------------------------------------------*/
void PQErase(pq_t* pq, int (*is_match_func)(const void* data, const void* param), const void* param); /* O(n) */

#endif /* ILRD_PQ_H */

