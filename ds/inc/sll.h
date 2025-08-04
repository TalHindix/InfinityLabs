/**************************************
Exercise: 	DS - SLL
Date:		2/7/2025
Developer:	Tal Hindi
Reviewer: 	Lotem Kitaroo
Status:		Approved
**************************************/

#ifndef _ILRD_SLL_
#define _ILRD_SLL_

#include <stddef.h> /* size_t */

typedef struct sll sll_t;
typedef struct node* sll_iter_t;

typedef int (*is_match_func_t)(const void* data, void* param);
typedef int (*action_func_t)(void* data, void* param);

/******************************************************************************
 * Function:   SLLCreate
 *
 * Purpose:    Creates a new SLL .
 *
 * Params:     void
 *
 * Returns:    sll_t*  – pointer to the new SLL, or NULL if failed
 *
 * Complexity: O(1)
 ******************************************************************************/
sll_t* SLLCreate(void);    /* O(1) */
/******************************************************************************
 * Function:   SLLDestroy
 *
 * Purpose:    Free memory SLL .
 *
 * Params:     sll_t* list - SLL
 
 * Returns:    void
 *
 * Complexity: O(n)
 ******************************************************************************/
void SLLDestroy(sll_t* list);   /* O(n) */
/******************************************************************************
 * Function:   SLLBegin
 *
 * Purpose:    Address to the start of the SLL
 *
 * Params:     const sll_t* list
 
 * Returns:    sll_iter_t
 *
 * Complexity: O(1)
 ******************************************************************************/
sll_iter_t SLLBegin(const sll_t* list); 
/******************************************************************************
 * Function:   SLLEnd
 *
 * Purpose:    Address to the tail/end of the SLL
 *
 * Params:     const sll_t* list
 
 * Returns:    sll_iter_t
 *
 * Complexity: O(1)
 ******************************************************************************/
sll_iter_t SLLEnd(const sll_t* list);   
/******************************************************************************
 * Function:   SLLNext
 *
 * Purpose:    Address to the next node
 *
 * Params:     sll_iter_t iter
 
 * Returns:    sll_iter_t
 *
 * Complexity: O(1)
 ******************************************************************************/
sll_iter_t SLLNext(sll_iter_t iter);    
/******************************************************************************
 * Function:   SLLIsEqual
 *
 * Purpose:    Check if nodes are equal
 *
 * Params:     sll_iter_t iter1, sll_iter_t iter2
 
 * Returns:    true or false 
 *
 * Complexity: O(1)
 ******************************************************************************/
int SLLIsEqual(sll_iter_t iter1, sll_iter_t iter2);
/******************************************************************************
 * Function:   SLLGetData
 *
 * Purpose:    Get data node
 *
 * Params:     sll_iter_t iter
 
 * Returns:    void* -> data of the node. 
 *
 * Complexity: O(1)
 ******************************************************************************/
void* SLLGetData(sll_iter_t iter);
/******************************************************************************
 * Function:   SLLSetData
 *
 * Purpose:    Set data to node
 *
 * Params:     sll_iter_t iter , and data (void *)
 
 * Returns:    none 
 *
 * Complexity: O(1)
 ******************************************************************************/
void SLLSetData(sll_iter_t iter, void* data);   
/******************************************************************************
 * Function:   SLLInsert
 *
 * Purpose:    Insert a node to SLL
 *
 * Params:     sll_iter_t where, void* data
 
 * Returns:    sll_iter_t 
 *
 * Complexity: O(1)
 ******************************************************************************/
sll_iter_t SLLInsert(sll_iter_t where, void* data);     /* Insert before where. O(1) */
/******************************************************************************
 * Function:   SLLRemove
 *
 * Purpose:    Remove a node from SLL
 *
 * Params:     sll_iter_t to_remove
 
 * Returns:    sll_iter_t 
 *
 * Complexity: O(1)
 ******************************************************************************/
sll_iter_t SLLRemove(sll_iter_t to_remove);      /* Remove and return the iter after the removed node. O(1) */
/******************************************************************************
 * Function:   SLLCount
 *
 * Purpose:    Count numbers of nodes
 *
 * Params:     const sll_t* list
 
 * Returns:    size_t 
 *
 * Complexity: O(n)
 ******************************************************************************/
size_t SLLCount(const sll_t* list); /* O(n) */
/******************************************************************************
 * Function:   SLLIsEmpty
 *
 * Purpose:    Check If SLL Exist
 *
 * Params:     const sll_t* list
 
 * Returns:    true or false (int) 
 *
 * Complexity: O(1)
 ******************************************************************************/
int SLLIsEmpty(const sll_t* list);  /* O(1) */
/******************************************************************************
 * Function:   SLLFind
 *
 * Purpose:    Check If node exist in sll
 *
 * Params:     sll_iter_t from, sll_iter_t to, is_match_func_t is_match, void* param
 
 * Returns:    sll_iter_t
 *
 * Complexity: O(n)
 ******************************************************************************/
sll_iter_t SLLFind(sll_iter_t from, sll_iter_t to, is_match_func_t is_match, void* param);
/******************************************************************************
 * Function:   SLLForEach
 *
 * Purpose:    Iterate over sll
 *
 * Params:     sll_iter_t from, sll_iter_t to, action_func_t action_func, void* param
 
 * Returns:    int (status)
 *
 * Complexity: O(n)
 ******************************************************************************/
int SLLForEach(sll_iter_t from, sll_iter_t to, action_func_t action_func, void* param);
/******************************************************************************
 * Function:   SLLAppend
 *
 * Purpose:    To combine to SLL src to dst
 *
 * Params:     sll_t* src, sll_t* dst
 
 * Returns:    sll_t* (dst)
 *
 * Complexity: O(n)
 ******************************************************************************/
sll_t* SLLAppend(sll_t* src, sll_t* dst);

#endif /* _ILRD_SLL_ */

