/**************************************
Exercise:  DS - SortedL
Date:      8/7/2025
Developer: Tal Hindi
Reviewer:  Yael Keidar
Status:    Approved
**************************************/

#ifndef _ILRD_SORTEDL_H_
#define _ILRD_SORTEDL_H_

#include "dll.h"  /* dll_t */

typedef struct sortedl_iter_t sorted_iter_t;
typedef struct sortedl        sortedl_t;

struct sortedl_iter_t
{
    dll_iter_t iter;
#ifndef NDEBUG
    sortedl_t* list;
#endif /* NDEBUG */
};

/***********************************************************************
 * SortedLCreate
 * --------------------------------------------------------------------
 * Create an empty list ordered by user-supplied compare function.
 *
 * @param cmp Comparison functor: like `strcmp` – returns
 *            <0 / 0 / >0  for  a<b / a==b / a>b
 * @return    Pointer to new list, or NULL on allocation failure
 * @complexity O(1)
 **********************************************************************/
sortedl_t *SortedLCreate(int (*cmp)(const void *a, const void *b));

/***********************************************************************
 * SortedLDestroy
 * --------------------------------------------------------------------
 * Free all nodes **and** the list object itself.
 * @warning  Stored data are **NOT** freed – user owns them.
 * @complexity O(n)
 **********************************************************************/
void SortedLDestroy(sortedl_t *list);

/***********************************************************************
 * SortedLInsert
 * --------------------------------------------------------------------
 * Insert `data` in its correct sorted position.
 * @return    Iterator to inserted element, or SortedLEnd(list) on fail
 * @complexity O(n)  (search)  |  O(1) if inserted at ends
 **********************************************************************/
sorted_iter_t SortedLInsert(sortedl_t *list, void *data);

/***********************************************************************
 * SortedLRemove
 * --------------------------------------------------------------------
 * Erase the element pointed to by `to_remove`.
 * @return    Iterator to the element that followed the removed one
 * @complexity O(1)
 **********************************************************************/
sorted_iter_t SortedLRemove(sorted_iter_t to_remove);

/***********************************************************************
 * SortedLSize
 * --------------------------------------------------------------------
 * Count elements (excluding dummies).
 * @complexity O(n)
 **********************************************************************/
size_t SortedLSize(const sortedl_t *list);

/***********************************************************************
 * SortedLIsEmpty
 * --------------------------------------------------------------------
 * @return 1 if empty, 0 otherwise
 * @complexity O(1)
 **********************************************************************/
int SortedLIsEmpty(const sortedl_t *list);

/*---------------------------- Searching -------------------------------*/

/***********************************************************************
 * SortedLFind
 * --------------------------------------------------------------------
 * Linear search for `to_find` in the **half-open** range [from, to).
 * @return Iterator to match, or `to` if not found
 * @complexity O(n)
 **********************************************************************/
sorted_iter_t SortedLFind(sortedl_t *list,
                          sorted_iter_t from,
                          sorted_iter_t to,
                          const void    *to_find);

/***********************************************************************
 * SortedLFindIf
 * --------------------------------------------------------------------
 * General predicate search in [from, to).
 * @param is_match  Predicate: return non-zero on match
 * @return Iterator to match, or `to` if none match
 * @complexity O(n)
 **********************************************************************/
sorted_iter_t SortedLFindIf(sorted_iter_t from,
                            sorted_iter_t to,
                            int (*is_match)(const void *, const void *),
                            const void *param);

/*--------------------------- Iterator API -----------------------------*/

/***********************************************************************
 * SortedLIsEqual       – compare two iterators                              O(1)
 * SortedLNext / Prev  	– move one step forward/backward                     O(1)
 * SortedLGetData      	– access element data                             	 O(1)
 **********************************************************************/
int          SortedLIsEqual(sorted_iter_t it1, sorted_iter_t it2);
sorted_iter_t SortedLNext(sorted_iter_t curr);
sorted_iter_t SortedLPrev(sorted_iter_t curr);
void        *SortedLGetData(sorted_iter_t iter);

/***********************************************************************
 * SortedLBegin / SortedLEnd
 * --------------------------------------------------------------------
 * Begin -> first real element; End -> tail dummy .
 * @complexity O(1)
 **********************************************************************/
sorted_iter_t SortedLBegin(const sortedl_t *list);
sorted_iter_t SortedLEnd  (const sortedl_t *list);


/***********************************************************************
 * SortedLPopFront / SortedLPopBack
 * --------------------------------------------------------------------
 * Remove element from front/back and return its data pointer.
 * @warning Undefined if list is empty.
 * @complexity O(1)
 **********************************************************************/
void *SortedLPopFront(sortedl_t *list);
void *SortedLPopBack (sortedl_t *list);

/***********************************************************************
 * SortedLMerge
 * --------------------------------------------------------------------
 * Splice **all** elements from `src` into `dest` (keeps order).
 * Post-condition: `src` becomes empty.
 * @require  dest->cmp == src->cmp
 * @complexity O(n)  (traverses src once)
 **********************************************************************/
void SortedLMerge(sortedl_t *dest, sortedl_t *src);

/***********************************************************************
 * SortedLForEach
 * --------------------------------------------------------------------
 * Apply `action_func` to each element in [from, to) until it returns
 * 
 *
 * @return 0 if entire range processed
 * @complexity O(n)
 **********************************************************************/
int SortedLForEach(sorted_iter_t from,
                   sorted_iter_t to,
                   int (*action_func)(void *data, void *param),
                   void *param);
                                             
#endif /* _ILRD_SORTEDL_H_ */

