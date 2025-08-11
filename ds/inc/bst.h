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

/* 
    Comparator:
    return <0 if data1 < data2, 0 if equal, >0 if data1 > data2.
    The tree only stores pointers. It never frees your data.
*/
typedef int (*cmp_func_t)(const void *data1, const void *data2);

typedef struct bst bst_t;
typedef struct bst_node bst_node_t;

typedef struct bst_iter
{
    bst_node_t *node;
} bst_iter_t;

/*
    Create an empty tree that uses 'cmp' to order items.
    Returns NULL on allocation failure.
    O(1) 
*/
bst_t* BSTCreate(cmp_func_t cmp);

/*
    Delete the whole tree and free all its nodes.
    Does NOT free the user data pointers inside the nodes.
    O(n) 
*/
void BSTDestroy(bst_t* tree);

/* 
    Insert a new item (pointer) into the tree.
    On success returns an iterator to the new node.
    On allocation failure returns BSTEnd(tree).
    Note: equal keys are allowed; ties go to the LEFT side.
    O(logn) 
*/
bst_iter_t BSTInsert(bst_t* tree, void* data);

/*
    Remove the node pointed to by 'to_remove'.
    Frees the node structure; does NOT free the user data.
    O(logn)
*/
void BSTRemove(bst_iter_t to_remove);

/* 
    Count how many nodes are in the tree.
    O(n)
*/
size_t BSTSize(const bst_t* tree);

/*
    Return 1 if the tree is empty, 0 otherwise.
    O(1) 
*/
int BSTIsEmpty(const bst_t* tree);

/* 
    Get an iterator to the smallest element.
    If the tree is empty, returns BSTEnd(tree).
    O(logn)
*/
bst_iter_t BSTBegin(const bst_t* tree);

/* 
    Get the “past-the-end” iterator (dummy).
    O(1)
*/
bst_iter_t BSTEnd(const bst_t* tree);

/*
    Move one step backward in in-order order.
    If there is no previous element, you’ll reach the boundary.
    O(1)
*/
bst_iter_t BSTNext(bst_iter_t iter);

/* 
    Return 1 if both iterators point to the same node, else 0.
    O(1)
*/
int BSTIterIsSame(bst_iter_t iter1, bst_iter_t iter2);

/* 
    Get the user data pointer stored at this iterator.
    The iterator must not be BSTEnd().
    O(1)
*/
void* BSTGetData(bst_iter_t iter);

/* 
    Find an element equal to 'to_find' using the comparator.
    Returns an iterator to it, or BSTEnd(tree) if not found.
    O(logn)
*/
bst_iter_t BSTFind(const bst_t* bst, void* to_find);

/*
    Apply 'action_func' to each item in [from, to) in in-order.
    Stop early if the action returns non-zero and return that value.
    Return 0 if all actions succeed.
    O(n)
*/
int BSTForEach(bst_iter_t from, bst_iter_t to,
               int (*action_func)(void* data, void* param), void* param);

#endif /* _ILRD_BST_H_ */