/**************************************
Exercise:   DS - AVL
Date:       18/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <stdio.h>  /* printf */

#include "avl.h"    /* AVLCreate */

/************** MACROS **************/
#define LEFT_CHILD(node)  ((node)->children[LEFT])
#define RIGHT_CHILD(node) ((node)->children[RIGHT])

/************** ENUMS **************/
typedef enum
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
} children;

/************ STRUCTURES ***********/
struct avl_node
{
    size_t height;
    void* data;
    struct avl_node* children[NUM_OF_CHILDREN];
};

typedef struct avl_node avl_node_t;

typedef int (*TraversalFunc)(avl_node_t* node, action_func_t action_func, void* param);

struct avl
{
    avl_node_t* root;
    cmp_func_t  cmp;
};

/*********** PROTOTYPES ************/
static void*       FindRec(const avl_node_t* node, const void* data, cmp_func_t cmp);
static void        DestroyRec(avl_node_t* node);
static avl_node_t* CreateNode(void* data);
static avl_node_t* InsertRec(avl_node_t* sub_root, void* data, cmp_func_t cmp, int* status);
static avl_node_t* RemoveRec(avl_node_t* sub_root, void* data, cmp_func_t cmp);
static avl_node_t* PopMin(avl_node_t* sub_root, avl_node_t** min_out);
static int         PreTraversalOrder (avl_node_t* node, action_func_t action_func, void* param);
static int         InTraversalOrder  (avl_node_t* node, action_func_t action_func, void* param);
static int         PostTraversalOrder(avl_node_t* node, action_func_t action_func, void* param);
static int         AddOne(void* data, void* param);
static size_t      Max2(size_t a, size_t b);
static size_t      NodeHeight(const avl_node_t* n);
static void        FixHeight(avl_node_t* n);
static int         BalanceFactor(avl_node_t* node);
static avl_node_t* RotateLeft(avl_node_t* node);
static avl_node_t* RotateRight(avl_node_t* node);
static avl_node_t* Rebalance(avl_node_t* node);

/******** LUT INIT ********/
static TraversalFunc traversal_lut[TRAVERSAL_NUM] =
{
    PreTraversalOrder,
    InTraversalOrder,
    PostTraversalOrder
};

avl_t* AVLCreate(cmp_func_t cmp)
{
    avl_t* tree = NULL;

    assert(cmp);

    tree = (avl_t*)malloc(sizeof(avl_t));
    if (!tree)
    {
        return NULL;
    }

    tree->root = NULL;
    tree->cmp  = cmp;

    return tree;
}

void AVLDestroy(avl_t* tree)
{
    assert(tree);

    DestroyRec(tree->root);

    free(tree);
}

int AVLInsert(avl_t* tree, void* data)
{
    int status = 0;

    assert(tree);
    assert(data);

    tree->root = InsertRec(tree->root, data, tree->cmp, &status);

    return status;
}

void AVLRemove(avl_t* tree, const void* data)
{
    assert(tree);
    assert(data);

    tree->root = RemoveRec(tree->root, (void*)data, tree->cmp);
}

size_t AVLHeight(const avl_t* tree)
{
    assert(tree);

    return (tree->root) ? tree->root->height : 0;
}

size_t AVLCount(const avl_t* tree)
{
    size_t counter = 0;

    assert(tree);

    AVLForEach((avl_t*)tree, AVL_PRE_ORDER, AddOne, &counter);

    return counter;
}

int AVLIsEmpty(const avl_t* tree)
{
    assert(tree);
    
    return (tree->root == NULL);
}

void* AVLFind(const avl_t* tree, const void* data)
{
    assert(tree);
    assert(data);

    return FindRec(tree->root, data, tree->cmp);
}

int AVLForEach(avl_t* tree, avl_traversal_order_t traversal_type, action_func_t action_func, void* param)
{
    int status = 0;

    assert(tree);
    assert(action_func);
    assert(traversal_type >= 0 && traversal_type < TRAVERSAL_NUM);

    status = traversal_lut[traversal_type](tree->root, action_func, param);

    return status;
}

/*********** HELPER FUNCTIONS ***********/

static void* FindRec(const avl_node_t* node, const void* data, cmp_func_t cmp)
{
    int cmp_res = 0;

    if (NULL == node)
    {
        return NULL;
    }

    cmp_res = cmp(data, node->data);

    if (0 == cmp_res)
    {
        return node->data;
    }
    else if (cmp_res < 0)
    {
        return FindRec(LEFT_CHILD(node), data, cmp);
    }
    else
    {
        return FindRec(RIGHT_CHILD(node), data, cmp);
    }
}

static void DestroyRec(avl_node_t* node)
{
    if (NULL == node)
    {
        return;
    }

    DestroyRec(LEFT_CHILD(node));
    DestroyRec(RIGHT_CHILD(node));
    free(node);
}

static avl_node_t* CreateNode(void* data)
{
    avl_node_t* node = (avl_node_t*)malloc(sizeof(avl_node_t));
    if (!node)
    {
        return NULL;
    }

    node->data            = data;
    node->children[LEFT]  = NULL;
    node->children[RIGHT] = NULL;
    node->height          = 1;

    return node;
}

static avl_node_t* InsertRec(avl_node_t* sub_root, void* data, cmp_func_t cmp, int* status)
{
    int cmp_res = 0;

    if (NULL == sub_root)
    {
        sub_root = CreateNode(data);
        *status  = (sub_root) ? 0 : -1;
        return sub_root;
    }

    cmp_res = cmp(data, sub_root->data);

    assert(cmp_res); /* duplicates not allowed */

    if (cmp_res < 0)
    {
        LEFT_CHILD(sub_root) = InsertRec(LEFT_CHILD(sub_root), data, cmp, status);
    }
    else
    {
        RIGHT_CHILD(sub_root) = InsertRec(RIGHT_CHILD(sub_root), data, cmp, status);
    }

    FixHeight(sub_root);
    return Rebalance(sub_root);
}

static avl_node_t* RemoveRec(avl_node_t* sub_root, void* data, cmp_func_t cmp)
{
    int cmp_res = 0;
    avl_node_t* successor = NULL;

    if (NULL == sub_root)
    {
        return NULL;
    }

    cmp_res = cmp(data, sub_root->data);

    if (cmp_res < 0)
    {
        LEFT_CHILD(sub_root) = RemoveRec(LEFT_CHILD(sub_root), data, cmp);
    }
    else if (cmp_res > 0)
    {
        RIGHT_CHILD(sub_root) = RemoveRec(RIGHT_CHILD(sub_root), data, cmp);
    }
    else /* found */
    {
        if (NULL == LEFT_CHILD(sub_root))
        {
            avl_node_t* temp = RIGHT_CHILD(sub_root);
            free(sub_root);
            return temp;
        }
        if (NULL == RIGHT_CHILD(sub_root))
        {
            avl_node_t* temp = LEFT_CHILD(sub_root);
            free(sub_root);
            return temp;
        }

        RIGHT_CHILD(sub_root) = PopMin(RIGHT_CHILD(sub_root), &successor);
        sub_root->data       = successor->data;
        free(successor);
    }

    FixHeight(sub_root);
    return Rebalance(sub_root);
}

static avl_node_t* PopMin(avl_node_t* sub_root, avl_node_t** min_out)
{
    if (NULL == sub_root)
    {
        *min_out = NULL;
        return NULL;
    }

    if (NULL == LEFT_CHILD(sub_root))
    {
        avl_node_t* right = RIGHT_CHILD(sub_root);
        *min_out = sub_root;
        return right;
    }

    LEFT_CHILD(sub_root) = PopMin(LEFT_CHILD(sub_root), min_out);

    FixHeight(sub_root);

    return sub_root;
}

/************ TRAVERSALS ************/

static int PreTraversalOrder (avl_node_t* node, action_func_t action_func, void* param)
{
    int status = 0;

    if (NULL == node)
    {
        return 0;
    }

    status = action_func(node->data, param);
    if (0 != status)
    {
        return status;
    }

    status = PreTraversalOrder(LEFT_CHILD(node), action_func, param);
    if (0 != status)
    {
        return status;
    }

    return PreTraversalOrder(RIGHT_CHILD(node), action_func, param);
}

static int InTraversalOrder  (avl_node_t* node, action_func_t action_func, void* param)
{
    int status = 0;

    if (NULL == node)
    {
        return 0;
    }

    status = InTraversalOrder(LEFT_CHILD(node), action_func, param);
    if (0 != status)
    {
        return status;
    }

    status = action_func(node->data, param);
    if (0 != status)
    {
        return status;
    }

    return InTraversalOrder(RIGHT_CHILD(node), action_func, param);
}

static int PostTraversalOrder(avl_node_t* node, action_func_t action_func, void* param)
{
    int status = 0;

    if (NULL == node)
    {
        return 0;
    }

    status = PostTraversalOrder(LEFT_CHILD(node), action_func, param);
    if (0 != status)
    {
        return status;
    }

    status = PostTraversalOrder(RIGHT_CHILD(node), action_func, param);
    if (0 != status)
    {
        return status;
    }

    return action_func(node->data, param);
}

static int AddOne(void* data, void* param)
{
    (void)data;
    *(size_t*)param += 1;
    return 0;
}

static size_t Max2(size_t a, size_t b)
{
    return (a > b) ? a : b;
}

static size_t NodeHeight(const avl_node_t* n)
{
    return (n) ? n->height : 0;
}

static void FixHeight(avl_node_t* n)
{
    n->height = 1 + Max2(NodeHeight(LEFT_CHILD(n)), NodeHeight(RIGHT_CHILD(n)));
}

static int BalanceFactor(avl_node_t* node)
{
    return (int)NodeHeight(LEFT_CHILD(node)) - (int)NodeHeight(RIGHT_CHILD(node));
}

static avl_node_t* RotateLeft(avl_node_t* node)
{
    avl_node_t* y  = RIGHT_CHILD(node);
    avl_node_t* T2 = LEFT_CHILD(y);

    LEFT_CHILD(y)   = node;
    RIGHT_CHILD(node) = T2;

    FixHeight(node);
    FixHeight(y);

    return y;
}

static avl_node_t* RotateRight(avl_node_t* node)
{
    avl_node_t* y  = LEFT_CHILD(node);
    avl_node_t* T2 = RIGHT_CHILD(y);

    RIGHT_CHILD(y)  = node;
    LEFT_CHILD(node) = T2;

    FixHeight(node);
    FixHeight(y);

    return y;
}

static avl_node_t* Rebalance(avl_node_t* node)
{
    int balance = BalanceFactor(node);

    if (balance > 1)
    {
        if (BalanceFactor(LEFT_CHILD(node)) < 0)
        {
            LEFT_CHILD(node) = RotateLeft(LEFT_CHILD(node));
        }
        return RotateRight(node);
    }

    if (balance < -1)
    {
        if (BalanceFactor(RIGHT_CHILD(node)) > 0)
        {
            RIGHT_CHILD(node) = RotateRight(RIGHT_CHILD(node));
        }
        return RotateLeft(node);
    }

    return node;
}
