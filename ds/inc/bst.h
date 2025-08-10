/**************************************
Exercise:  DS - BST
Date:      7/8/2025
Developer: Tal Hindi
Reviewer:  
Status:    
**************************************/

#ifndef _ILRD_BST_H_
#define _ILRD_BST_H_

#include <stddef.h> /* size_t */

typedef struct bst bst_t;
typedef struct bst_node bst_node_t;
typedef int(*cmp_func_t)(const void* data1, const void* data2);

typedef struct bst_iter
{
    bst_node_t* node;
}bst_iter_t;


/* O(1) */
bst_t* BSTCreate(cmp_func_t cmp);

/* O(n) */
void BSTDestroy(bst_t* tree);

/* O(logn) */ 
bst_iter_t BSTInsert(bst_t* tree, void* data); 

/* O(logn) */
void BSTRemove(bst_iter_t to_remove);

/* O(n) */
size_t BSTSize(const bst_t* tree);

/* O(1) */
int BSTIsEmpty(const bst_t* tree);

/* O(logn) */
bst_iter_t BSTBegin(const bst_t* tree);

/* O(1) */
bst_iter_t BSTEnd(const bst_t* tree);

/* O(1) */
bst_iter_t BSTPrev(bst_iter_t iter);

/* O(1) */
bst_iter_t BSTNext(bst_iter_t iter);

/* O(1) */
int BSTIterIsSame(bst_iter_t iter1, bst_iter_t iter2);

/* O(1) */
void* BSTGetData(bst_iter_t iter);

/* O(logn) */
bst_iter_t BSTFind(const bst_t* bst, void* to_find); 

/* O(n) */
int BSTForEach(bst_iter_t from, bst_iter_t to, int(*action_func)(void* data, void* param), void* param);

#endif /* _ILRD_BST_H_ */
