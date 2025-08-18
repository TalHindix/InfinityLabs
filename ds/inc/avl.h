/**************************************
Exercise:   DS - AVL
Date:       18/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#ifndef __ILRD_AVL_H__
#define __ILRD_AVL_H__

#include <stddef.h> /* size_t */

typedef struct avl avl_t;

typedef enum avl_traversal_order
{
    AVL_PRE_ORDER,
    AVL_IN_ORDER,
    AVL_POST_ORDER
} avl_traversal_order_t;

typedef int (*cmp_func_t)(const void* param1, const void* param2);
typedef int (*action_func_t)(void* data, void* param);
/* O(1) */
avl_t* AVLCreate(cmp_func_t cmp_func);

/* O(n) */
void AVLDestroy(avl_t* tree);

/* O(log(n)) */
void AVLRemove(avl_t* tree, const void* data);

/* O(log(n)) */
int AVLInsert(avl_t* tree, void* data);

/* O(1) */
size_t AVLHeight(const avl_t* tree);

/* O(n) */
size_t AVLCount(const avl_t* tree);

/* O(1) */
int AVLIsEmpty(const avl_t* tree);

/* O(log(n)) */
void* AVLFind(const avl_t* tree, const void* data);

/* O(n) */
int AVLForEach(avl_t* tree, int traversal_type, action_func_t action_func, void* param); /* -- Pre / In / Post -- */

#endif /* __ILRD_AVL_H__ */