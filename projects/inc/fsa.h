/**************************************
Exercise: 	Prog - FSA
Date:		24/07/2025
Developer:	Tal Hindi
Reviewer: 	Daniel Gamliel
Status:		
**************************************/

#ifndef __ILRD_FSA_H__
#define __ILRD_FSA_H__

#include <stdlib.h> /* size_t */

typedef struct FSA fsa_t;

/**
 * Description: Returns the number of bytes to allocate. 
 *
 * Function Arguments :
 *		 block_amount - amount of blocks wanted.
 *		 block_size - size of each block. 
 *
 * Return Value: 
 *        - Suggested allocation size.
 *
 * General notes: None.
 *
 * Complexity:
 *        - Time: O(1)
 **/
size_t FSASuggestSize(size_t block_amount ,size_t block_size );

/**
 * Description: Initialize Fixed size allocator according to FSASuggestSize.
 *
 * Function Arguments : 
 *		 pool - memory block allocated by user.
 *		 pool_size - size of pool allocated.
 *		 block_size - size of each block. 
 *
 * Return Value:
 *        - a pointer to the FSA.     
 *
 * General notes:
 *        - Behavior is undefined if pool pointer is NULL.
 *        - Behavior is undefined if allocated_size or block_size is 0.
 *        - if block_size is less then default -> block_size = default.
 * 
 * Complexity:
 *        - Time: O(n).
 **/
fsa_t* FSAInit(void* pool ,size_t pool_size ,size_t block_size );
/**
 * Description: Allocate fixed size block of memory.
 *
 * Function Arguments : 
 *        fsa_t* - a pointer to the FSA.
 *
 * Return Value: 
 *        - a pointer to the new block.
 *        - NULL if no free blocks are available. 
 *
 * General notes: 
 *        - Behavior is undefined if fsa pointer is NULL.
 *
 * Complexity:
 *        - Time: O(1).
 **/
void* FSAAlloc(fsa_t* fsa);

/**
 * Description: free block of memory.
 *
 * Function Arguments : 
 *		 block - block wanted to be freed.
 *
 * Return Value: None.
 *
 * General notes: 
 *        - block must be pointer recived by FSAAlloc.
 *        - block that is null will be ignored.
 *
 * Complexity:
 *        - Time: O(1).
 **/
void FSAFree(fsa_t* fsa ,void* block);

/**
 * Description: Return the number of free blocks available.
 *
 * Function Arguments :
 *        fsa - fsa to count free blocks in.        
 * Return Value: 
 *        - number of free block available.
 *
 * General notes: None.
 *
 * Complexity:
 *        - Time: O(n).
 **/
size_t FSACountFree(const fsa_t* fsa);

#endif /* __ILRD_FSA_H__ */
