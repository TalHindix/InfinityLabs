/**************************************
Exercise:   DS - AVL
Date:       18/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h>

#include "avl.h"    /* avl_t */

#define LEFT_CHILD(node) (node->children[LEFT])
#define RIGHT_CHILD(node) (node->children[RIGHT])

typedef enum
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
} children;

struct avl_node
{
    size_t height;
    void* data;
    struct avl_node* children[NUM_OF_CHILDREN];
};

typedef struct avl_node avl_node_t;

struct avl
{
   avl_node_t* root;
   cmp_func_t cmp;
};

/* ============== PROTOTYPES ===========================*/
static void* FindRec(const avl_node_t* node, const void* data, cmp_func_t cmp);
static void DestroyRec(avl_node_t *node);
static avl_node_t* InsertRec(avl_node_t* sub_root,void* data, cmp_func_t  cmp, int* status);
static avl_node_t* CreateNode(void* data);
static size_t CountRec(const avl_node_t* node);

avl_t* AVLCreate(cmp_func_t cmp)
{
    avl_t* tree = NULL;

    assert(cmp);
    
    tree = (avl_t*)malloc(sizeof(avl_t));
    if (!tree)
    {
        return NULL;
    }
    
    tree->cmp = cmp;
    tree->root = NULL;

    return tree;
}

void AVLDestroy(avl_t* tree)
{
    assert(tree);

    DestroyRec(tree->root);

    free(tree);
}

void AVLRemove(avl_t* tree, const void* data)
{
    assert(tree);
    assert(data);
}

int AVLInsert(avl_t* tree, void* data)
{
    int status = 0;

    assert(tree);
    assert(data);

    tree->root = InsertRec(tree->root, data, tree->cmp, &status);

    return status;

}

size_t AVLHeight(const avl_t* tree)
{
    (void)tree;
    return 0;
}

size_t AVLCount(const avl_t* tree)
{
    assert(tree);

    return CountRec(tree->root);
}

int AVLIsEmpty(const avl_t* tree)
{
    assert(tree);

    return (tree->root == NULL) ;
}

void* AVLFind(const avl_t* tree, const void* data)
{
    assert(tree);
    assert(data);

    return FindRec(tree->root, data, tree->cmp);
}

int AVLForEach(avl_t* tree, int traversal_type, action_func_t action_func, void* param)
{
    (void)tree;
    (void)action_func;
    (void)param;
    (void)traversal_type;
    return 0;
}

/* ============== HELPER FUNCTIONS ===========================*/
static void* FindRec(const avl_node_t* node, const void* data, cmp_func_t cmp)
{
    int cmp_result = 0;

    if (NULL == node)
    {
        return NULL;
    }

    cmp_result = cmp(data,node->data);

    if (0 == cmp_result)
    {
        return node->data;
    } 
    else if (0 > cmp_result)
    {
        return FindRec(LEFT_CHILD(node),data,cmp);
    }
    else
    {
        return FindRec(RIGHT_CHILD(node),data,cmp);
    }

}

static void DestroyRec(avl_node_t* node)
{
    if(NULL == node)
    {
        return;
    }

    DestroyRec(LEFT_CHILD(node));
    DestroyRec(RIGHT_CHILD(node)); 

    free(node);                    
}

static avl_node_t* InsertRec(avl_node_t* sub_root,void* data, cmp_func_t cmp, int* status)
{
    int cmp_result = 0;

    if (NULL == sub_root)
    {
        sub_root = CreateNode(data);
        *status = (sub_root) ? 0 : -1;
        return sub_root;
    }

    cmp_result = cmp(data,sub_root->data);

    assert(cmp_result);

    if(cmp_result < 0)
    {
        LEFT_CHILD(sub_root) = InsertRec(LEFT_CHILD(sub_root),data,cmp,status);
    }
    else
    {
        RIGHT_CHILD(sub_root) = InsertRec(RIGHT_CHILD(sub_root),data,cmp,status);
    }

    return sub_root;
}

static avl_node_t* CreateNode(void* data)
{
    avl_node_t* node = (avl_node_t *)malloc(sizeof(avl_node_t));
    if (!node)
    {
        return NULL;
    }

    node->data = data;
    LEFT_CHILD(node) = NULL;
    RIGHT_CHILD(node) = NULL;
    node->height = 1;

    return node;
}

static size_t CountRec(const avl_node_t* node)
{
    if (NULL == node)
    {
        return 0;
    }

    return 1 + CountRec(LEFT_CHILD(node)) + CountRec(RIGHT_CHILD(node));
}