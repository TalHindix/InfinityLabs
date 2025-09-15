/**************************************
Exercise: 	DS - FSQ Thread SAFE
Date:		14/9/2025
Developer:  Tal Hindi
Reviewer:   Guy Argaman
Status:		
**************************************/

#ifndef __ILRD__FSQ__
#define __ILRD__FSQ__

#include "sll.h"

#include <stddef.h>

typedef struct fsq fsq_t;

/* Create a new fixed-size thread-safe queue */
fsq_t* FSQCreate(size_t capacity); /* O(1) */

/* Destroy the queue and free all memory */
void FSQDestroy(fsq_t* fsq); /* O(n) */

/* Add an item to the end of the queue (waits if full) */
int FSQEnqueue(fsq_t* fsq, void* data); /* O(1) */

/* Remove and return the item at the front of the queue (waits if empty) */
void* FSQDequeue(fsq_t* fsq); /* O(1) */

/* Check if the queue is empty (returns 1 if yes, 0 if no) */
int FSQIsEmpty(const fsq_t* fsq); /* O(1) */

/* Return the number of items currently in the queue */
size_t FSQSize(const fsq_t* fsq); /* O(1) */

/* Look at the front item without removing it */
void* FSQPeek(const fsq_t* fsq); /* O(1) */

#endif /* __ILRD__FSQ__ */
