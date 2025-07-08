#ifndef _ILRD_QUEUE_
#define _ILRD_QUEUE_

#include "sll.h"




queue_t* QueueCreate(void); /* O(1) */
void QueueDestroy(queue_t* queue); /* O(n) */
int QueueEnqueue(queue_t* queue, void* data); /* O(1) */
void QueueDequeue(queue_t* queue); /* O(1) */
int QueueIsEmpty(const queue_t* queue); /* O(1) */
size_t QueueSize(const queue_t* queue); /* O(n) */
void* QueuePeek(const queue_t* queue); /* O(1) */
queue_t* QueueAppend(queue_t* src, queue_t* dst); /* O(1) */

#endif /* _ILRD_QUEUE_ */

