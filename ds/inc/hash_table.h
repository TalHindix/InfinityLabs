/**************************************
Exercise:   DS - Hash Table
Date:       20/8/2025
Developer:  Tal Hindi
Reviewer:   Baruchi Haimson
Status:     
**************************************/

#ifndef ILRD_HASH_TABLE_H
#define ILRD_HASH_TABLE_H

#include <stddef.h> /* size_t */

#include "sll.h"    /* SLLIsEmpty */


/* Maps a key (void *) to an index in [0 , buckets-1] */
typedef size_t (*hash_func_t)(const void* key);

typedef struct hash_table hash_table_t;

/**********************************************************************
* HashTableCreate
* --------------------------------------------------------------------
* Allocates and initialises an empty hash table.
*
* Parameters:
*   hash_func  – User-supplied hashing function (must not be NULL).
*   size       – Rough estimate of elements to store (also bucket count;
*                must be > 0).
*   is_match   – Predicate that decides key equality inside a bucket
*                (must not be NULL).
*
* Returns:
*   Pointer to a new, empty hash table on success; NULL on allocation
*   failure.
*
* Notes:
*   • Collisions are resolved with **separate chaining** (each bucket is
*     a singly linked list).
*
* Undefined Behaviour:
*   Passing NULL to hash_func or is_match, or passing size == 0.
*
* Complexity:
*   O(m) time, O(m) space — m is the number of buckets.
**********************************************************************/
hash_table_t* HashTableCreate(hash_func_t hash_func, size_t size, is_match_func_t is_match);

/**********************************************************************
* HashTableDestroy
* --------------------------------------------------------------------
* Releases all resources held by the hash table.
*
* Parameters:
*   table – Pointer returned by HashTableCreate (must not be NULL).
*
* Complexity:
*   O(n + m) — n stored elements, m buckets.
**********************************************************************/
void HashTableDestroy(hash_table_t* table);

/**********************************************************************
* HashTableInsert
* --------------------------------------------------------------------
* Inserts data into the table.
*
* Parameters:
*   table – Hash table pointer (must not be NULL).
*   data  – Pointer to user data to store (must not be NULL).
*
* Returns:
*   0 on success, non-zero on allocation failure.
*
* Undefined Behaviour:
*   Inserting a key already present in the table (duplicate handling is
*   undefined).
*
* Complexity:
*   Average O(1); Worst-case O(n) when many keys collide.
**********************************************************************/
int HashTableInsert(hash_table_t* table, void* data);

/**********************************************************************
* HashTableRemove
* --------------------------------------------------------------------
* Deletes a key (and its data) from the table. Has no effect if the key
* is not found.
*
* Parameters:
*   table – Hash table pointer (must not be NULL).
*   key   – Key to remove (must not be NULL).
*
* Complexity:
*   Average O(1); Worst-case O(n).
**********************************************************************/
void HashTableRemove(hash_table_t* table, const void* key);

/**********************************************************************
* HashTableFind
* --------------------------------------------------------------------
* Searches for a key and returns the stored data pointer.
*
* Parameters:
*   table – Hash table pointer (must not be NULL).
*   key   – Key to search for (must not be NULL).
*
* Returns:
*   Pointer to the data if found, NULL otherwise.
*
* Complexity:
*   Average O(1); Worst-case O(n).
**********************************************************************/
void* HashTableFind(const hash_table_t* table, const void* key);

/**********************************************************************
* HashTableSize
* --------------------------------------------------------------------
* Returns the number of elements currently stored in the table.
*
* Complexity:
*   O(n + m)
**********************************************************************/
size_t HashTableSize(const hash_table_t* table);

/**********************************************************************
* HashTableIsEmpty
* --------------------------------------------------------------------
* Checks if the table is empty.
*
* Returns:
*   1 if empty, 0 otherwise.
*
* Complexity:
*   O(n)
**********************************************************************/
int HashTableIsEmpty(const hash_table_t* table);

/**********************************************************************
* HashTableForEach
* --------------------------------------------------------------------
* Applies action_func to every element in the table until all elements
* are processed or action_func returns a non-zero value.
*
* Parameters:
*   table       – Hash table pointer (must not be NULL).
*   action_func – User function applied to each element (must not be
*                 NULL).  Return non-zero to stop iteration early.
*   param       – Extra parameter forwarded to action_func (may be NULL).
*
* Returns:
*   0 if all elements were processed; the non-zero value returned by
*   action_func if iteration was stopped early.
*
* Complexity:
*   O(n + m).
**********************************************************************/
int HashTableForEach(hash_table_t* table, action_func_t action_func, void* param);

#endif /* ILRD_HASH_TABLE_H */
