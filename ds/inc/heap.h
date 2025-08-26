/**************************************
Exercise:   DS – Heap
Date:       25/8/2025
Developer:  Tal Hindi
Reviewer:   Baruchi Haimson   
Status:     
**************************************/

#ifndef __ILRD__HEAP__H
#define __ILRD__HEAP__H

#include <stddef.h> /* size_t */

#include "vector.h" /* VectorCreate */

/*--------------------------------------------------
  User-supplied functions
--------------------------------------------------*/
typedef int (*compare_func)(const void* data1, const void* data2);

typedef int (*is_match_func)(const void* data1, const void* data2);

typedef struct heap heap_t;

/*--------------------------------------------------
HeapCreate
Creates an empty binary heap (priority queue).

compare – must point to a valid compare_func.
Return  – new heap on success, NULL on allocation error.

Time  O(1)   •   Space  O(1)
--------------------------------------------------*/
heap_t* HeapCreate(compare_func compare);

/*--------------------------------------------------
HeapDestroy
Frees all memory that belongs to the heap itself.
Does NOT free the user data stored inside.

heap – pointer from HeapCreate (must not be NULL).

Time  O(n)
--------------------------------------------------*/
void HeapDestroy(heap_t* heap);

/*--------------------------------------------------
HeapPeek
Returns a pointer to the top (highest-priority) item.
Returns NULL if the heap is empty.

Time  O(1)
--------------------------------------------------*/
void* HeapPeek(const heap_t* heap);

/*--------------------------------------------------
HeapPush
Adds a new item.

heap – heap pointer (not NULL)
data – pointer to user data (not NULL)
Return 0 on success, non-zero on allocation error.

Time  O(log n)
--------------------------------------------------*/
int HeapPush(heap_t* heap, void* data);

/*--------------------------------------------------
HeapPop
Removes the highest-priority item.
If the heap is empty, does nothing.

Time  O(log n)
--------------------------------------------------*/
void HeapPop(heap_t* heap);

/*--------------------------------------------------
HeapRemove
Removes the first item that matches *key*.
Nothing happens if no match is found.

heap      – heap pointer (not NULL)
key       – key to look for   (not NULL)
is_match  – returns 0 when (item, key) are equal (not NULL)

Time  O(n) to find + O(log n) to restore heap
--------------------------------------------------*/
void* HeapRemove(heap_t* heap, const void* key, is_match_func is_match);

/*--------------------------------------------------
HeapIsEmpty
Returns 1 if the heap has no items, 0 otherwise.

Time  O(1)
--------------------------------------------------*/
int HeapIsEmpty(const heap_t* heap);

/*--------------------------------------------------
HeapSize
Returns how many items are stored now.

Time  O(1)
--------------------------------------------------*/
size_t HeapSize(const heap_t* heap);

#endif /* __ILRD__HEAP__H */
