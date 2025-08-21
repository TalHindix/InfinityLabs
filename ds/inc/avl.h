/**************************************
Exercise:   DS - AVL
Date:       18/8/2025
Developer:  Tal Hindi
Reviewer:   Dana Chesner
Status:     Approved
**************************************/

#ifndef __ILRD_AVL_H__
#define __ILRD_AVL_H__

#include <stddef.h> /* size_t */

/*
    Comparator contract:
    cmp_func(a, b) must return <0 if *a < *b, 0 if equal, >0 if *a > *b.
    Ordering must be strict and consistent with transitivity.

    Notes:
    - The tree stores user pointers only; it NEVER frees your data.
    - Equal keys are NOT allowed. Passing a duplicate key is undefined
      behavior; do not insert duplicates.
*/
typedef int (*cmp_func_t)(const void* a, const void* b);

/*
    Action function contract (used by AVLForEach):
    Should return 0 on success; any non-zero value aborts the traversal
    early and is propagated back to the caller.
*/
typedef int (*action_func_t)(void* data, void* param);

typedef struct avl avl_t;

/*
    Traversal order for AVLForEach:
    AVL_PRE_ORDER : visit node, then left, then right
    AVL_IN_ORDER  : visit left, node, right  (sorted order for BST/AVL)
    AVL_POST_ORDER: visit left, right, node
*/
typedef enum avl_traversal_order
{
    AVL_PRE_ORDER,
    AVL_IN_ORDER,
    AVL_POST_ORDER,
    TRAVERSAL_NUM
} avl_traversal_order_t;

/*
    Create an empty AVL tree that uses 'cmp_func' to order elements.
    Returns NULL on allocation failure.
    Complexity: O(1)
*/
avl_t* AVLCreate(cmp_func_t cmp_func);

/*
    Destroy the entire tree and free all internal node structures.
    IMPORTANT: user data pointers stored in nodes are NOT freed.
    Safe to call with an empty tree.
    Complexity: O(n)
*/
void AVLDestroy(avl_t* tree);

/*
    Insert a new element (pointer) into the tree.
    Returns 0 on success; returns -1 on allocation failure.
    Duplicate keys are not allowed (see comparator contract above).
    Complexity: O(log n)
*/
int AVLInsert(avl_t* tree, void* data);

/*
    Remove the element equal to 'data' according to the comparator.
    If the element does not exist, the tree is unchanged.
    Complexity: O(log n)
*/
void AVLRemove(avl_t* tree, const void* data);

/*
    Return the height of the tree.
    By convention: empty tree has height 0; a leaf has height 1.
    Complexity: O(1)
*/
size_t AVLHeight(const avl_t* tree);

/*
    Count how many nodes are currently stored.
    Complexity: O(n)
*/
size_t AVLCount(const avl_t* tree);

/*
    Return 1 if the tree is empty, 0 otherwise.
    Complexity: O(1)
*/
int AVLIsEmpty(const avl_t* tree);

/*
    Find an element equal to 'data' using the comparator.
    Returns a pointer to the stored element 
    if found, or NULL if not found.
    Complexity: O(log n)
*/
void* AVLFind(const avl_t* tree, const void* data);

/*
    Apply 'action_func' to every element in the specified traversal order.
    'traversal_type' must be one of: AVL_PRE_ORDER / AVL_IN_ORDER / AVL_POST_ORDER.
    Traversal stops early if 'action_func' returns non-zero. that value is
    returned to the caller. Returns 0 if all applications succeed.
    Complexity: O(n)
*/
int AVLForEach(avl_t* tree, int traversal_type, action_func_t action_func, void* param);

#endif /* __ILRD_AVL_H__ */
