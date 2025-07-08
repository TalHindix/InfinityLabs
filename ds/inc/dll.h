/**************************************
Exercise: 	DS - DLL
Date:		8/7/2025
Developer:	Tal Hindi
Reviewer: 	Menny Markovich
Status:		
**************************************/

#ifndef _ILRD_DLL_
#define _ILRD_DLL_

#include <stddef.h>


typedef struct dll dll_t;
typedef struct node node_t;
typedef node_t* dll_iter_t;


dll_t* DLLCreate(void);     /* O(1) */
void DLLDestroy(dll_t* list);   /* O(n) */
dll_iter_t DLLBegin(const dll_t* list); /* O(1) */
dll_iter_t DLLEnd(const dll_t* list);   /* O(1) */
dll_iter_t DLLNext(dll_iter_t curr);    /* O(1) */
dll_iter_t DLLPrev(dll_iter_t curr);    /* O(1) */
int DLLIsEqual(dll_iter_t iter1, dll_iter_t iter2); /* O(1) */
void* DLLGetData(dll_iter_t iter);  /* O(1) */
void DLLSetData(dll_iter_t iter, void* data);   /* O(1) */
dll_iter_t DLLInsert(dll_t* list, dll_iter_t where, void* data);     /* Insert before where. O(1) */
dll_iter_t DLLRemove(dll_iter_t to_remove);      /* Remove and return the iter after the removed node. O(1) */
dll_iter_t DLLPushFront(dll_t* list, void* data); /* Insert at the beginning. O(1) */
dll_iter_t DLLPushBack(dll_t* list, void* data); /* Insert at the end. O(1) */
void *DLLPopFront(dll_t* list); /* Remove the beginning and return the next iter. O(1) */
void *DLLPopBack(dll_t* list); /* Remove the end and return the previous iter. O(1) */
size_t DLLCount(const dll_t* list); /* O(n) */
int DLLIsEmpty(const dll_t* list);  /* O(1) */
dll_iter_t DLLFind(dll_iter_t from, dll_iter_t to,int (*is_match_func_t)(const void* data, const void* param), const void* param); 
int DLLMultiFind(dll_iter_t from, dll_iter_t to,int (*is_match_func_t)(const void* data, const void* param), const void* param, dll_t* output);
int DLLForEach(dll_iter_t from, dll_iter_t to,int (*action_func_t)(void* data, void* param), void* param); 
dll_t* DLLSplice(dll_iter_t where, dll_iter_t from, dll_iter_t to);

#endif /* _ILRD_DLL_ */


