/**************************************
Exercise: 	DS - DLL
Date:		8/7/2025
Developer:	Tal Hindi
Reviewer: 	Menny Markovich
Status:		
**************************************/

#ifndef _ILRD_DLL_
#define _ILRD_DLL_

#include <stddef.h> /* size_t */


typedef struct dll dll_t;
typedef struct node node_t;
typedef node_t *dll_iter_t;

typedef int (*dll_action_t)(void *data, void *param);


/**
 * @brief  Make an empty list (only the two dummy nodes exist).
 * @return Pointer to list, or NULL if malloc failed.
 * @complexity O(1)
 */
dll_t *DLLCreate(void);

/**
 * @brief  Free the whole list.  Does NOT free the user data inside nodes.
 * @param  list  List from DLLCreate (NULL = do nothing).
 * @complexity O(n)
 */
void DLLDestroy(dll_t *list);


/** @brief First real element (dummy head -> next).  O(1) */
dll_iter_t DLLBegin(const dll_t *list);

/** @brief One-past-last element (the dummy tail itself).  O(1) */
dll_iter_t DLLEnd(const dll_t *list);

/** @brief Move right  by one node. O(1) */
dll_iter_t DLLNext(dll_iter_t curr);

/** @brief Move left   by one node. O(1) */
dll_iter_t DLLPrev(dll_iter_t curr);

/** @brief 1 if two iterators point to the same node, else 0. O(1) */
int DLLIsEqual(dll_iter_t iter1, dll_iter_t iter2);


/** @brief Get what’s stored in this node. O(1) */
void *DLLGetData(dll_iter_t iter);

/** @brief Overwrite the stored data. O(1) */
void DLLSetData(dll_iter_t iter, void *data);

/**
 * @brief  Insert new node **before** spot “where”.
 * @return Iterator to the new node, or DLLEnd(list) if malloc failed.
 * @complexity O(1)
 */
dll_iter_t DLLInsert(dll_t *list, dll_iter_t where, void *data);

/**
 * @brief  Delete node at “to_remove”.
 * @return Iterator to the node that was right after it.
 * @complexity O(1)
 */
dll_iter_t DLLRemove(dll_iter_t to_remove);

/** @brief Push to front  (after dummy head). O(1) */
dll_iter_t DLLPushFront(dll_t *list, void *data);

/** @brief Push to back   (before dummy tail). O(1) */
dll_iter_t DLLPushBack(dll_t *list, void *data);

/** @brief Pop front; returns data or NULL if list empty. O(1) */
void *DLLPopFront(dll_t *list);

/** @brief Pop back;  returns data or NULL if list empty. O(1) */
void *DLLPopBack(dll_t *list);

/** @brief Count how many real nodes right now. O(n) */
size_t DLLCount(const dll_t *list);

/** @brief 1 if empty, 0 otherwise. O(1) */
int DLLIsEmpty(const dll_t *list);

/**
 * @brief   Scan [from, to) until match function says “yep”.
 * @return  Iterator to match, or “to” if none found.
 * @complexity O(k) where k = nodes checked
 */
dll_iter_t DLLFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void *data,const void *param), const void *param);

/**
 * @brief   Like find, but collects all matches into ‘output’ list.
 * @return  How many matches were copied (-1 if malloc blew up).
 */
int DLLMultiFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void *data, const void *param), const void *param, dll_t *output);

/**
 * @brief Run “action” on every node in [from, to) until action says stop.
 * @return 0 if full pass, or first non-zero value from action.
 */
int DLLForEach(dll_iter_t from, dll_iter_t to,
               dll_action_t action, void *param);

/**
 * @brief  Cut range and paste it right before “where”.
 *         Works even if range comes from another list.
 * @return Pointer to the list that now owns the moved nodes.
 * @complexity O(1) (only pointer rewiring)
 */
dll_t *DLLSplice(dll_iter_t where, dll_iter_t from, dll_iter_t to);

#endif /* _ILRD_DLL_ */

