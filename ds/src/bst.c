/**************************************
Exercise:  DS - BST
Date:      7/8/2025
Developer: Tal Hindi
Reviewer:
Status:
**************************************/

#include <stddef.h> /* size_t, NULL */
#include <stdlib.h> /* malloc       */
#include <assert.h> /* assert       */
#include <string.h>

#include "bst.h"

typedef enum child_node_pos
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
} child_node_pos_t;

struct bst_node
{
    bst_node_t* parent;
    bst_node_t* children[NUM_OF_CHILDREN];
    void* data;
};

struct bst
{
    bst_node_t root_stub;    
    cmp_func_t cmp;
};

static bst_iter_t* StepInOrder(bst_iter_t iter,child_node_pos_t first_dir,child_node_pos_t second_dir);
static bst_iter_t* NodeToIter(bst_node_t* ptr);
static bst_node_t* CreateNode(void* data);
static bst_node_t* FindNode(const bst_t* tree, void* key);

bst_t* BSTCreate(cmp_func_t cmp)
{
    bst_t* tree = NULL;

    assert(cmp);
    
    tree = (bst_t*)malloc(sizeof(bst_t));
    if (!tree)
    {
        return NULL;
    }
    
    tree->cmp = cmp;

    tree->root_stub.children[RIGHT] = &tree->root_stub;
    tree->root_stub.children[LEFT] = NULL;
    tree->root_stub.data = NULL;
    tree->root_stub.parent = NULL;

    return tree;
}

void BSTDestroy(bst_t* tree)
{
    (void)tree;
}

bst_iter_t BSTInsert(bst_t* tree, void* data)
{
    bst_node_t* new_node = NULL;
    bst_node_t* parent = NULL;
    bst_node_t* current = NULL;
    int cmp_result = 0;

    assert(tree);
    assert(data);

    current = tree->root_stub.children[LEFT];
    parent = &tree->root_stub;

    /* Find The Insertion Point*/
    while (!current)
    {
        parent = current;
        cmp_result = tree->cmp(data,current->data);

        if(cmp_result > 0)
        {
            current = current->children[RIGHT];
        }
        else
        {
            current = current->children[LEFT];
        }
    }

    new_node = CreateNode(data);

    if(!new_node)
    {
        return NULL;
    }

    new_node->parent = parent;

    if(cmp_result > 0)
    {
        current->children[RIGHT] = new_node;
    }
    else
    {
        current->children[LEFT] = new_node;
    }


    return NodeToIter(new_node);
}

void BSTRemove(bst_iter_t to_remove)
{
    (void)to_remove;
}

size_t BSTSize(const bst_t *tree)
{
    size_t count = 0;
    bst_iter_t it = {0};

    assert(tree);

    for (it = BSTBegin(tree); !BSTIterIsSame(it, BSTEnd(tree)); it = BSTNext(it))
    {
        ++count;
    }

    return count;
}


int BSTIsEmpty(const bst_t* tree)
{
    assert(tree);

    return (tree->root_stub.children[LEFT] == NULL);
}

bst_iter_t BSTBegin(const bst_t* tree)
{
    bst_node_t* node = NULL;

    assert(tree);

    node = tree->root_stub.children[RIGHT];

    if(!node)
    {
        return BSTEnd(tree);
    }

    while (!node->children[LEFT])
    {
        node = node->children[LEFT];
    }

    return NodeToIter(node);
}

bst_iter_t BSTEnd(const bst_t* tree)
{
    assert(tree);

    return NodeToIter((bst_node_t*)&tree->root_stub);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    return StepInOrder(iter,LEFT , RIGHT);
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    return StepInOrder(iter,RIGHT , LEFT);
}

int BSTIterIsSame(bst_iter_t iter1, bst_iter_t iter2)
{
    return iter1.node == iter2.node;
}

void* BSTGetData(bst_iter_t iter)
{
    assert(iter.node);

    return iter.node->data;
}

bst_iter_t BSTFind(const bst_t* bst, void* to_find)
{
    bst_node_t* node = NULL;

    assert(bst);
    assert(to_find);

    node = FindNode(bst,to_find);
    if(!node)
    {
        return NodeToIter(node);
    }
    
    return BSTEnd((bst_t*)bst);
}

int BSTForEach(bst_iter_t from, bst_iter_t to,
               int (*action_func)(void* data, void* param), void* param)
{
    (void)from;
    (void)to;
    (void)action_func;
    (void)param;
    return 0;
}


static bst_iter_t StepInOrder(bst_iter_t iter,
                              child_node_pos_t first_dir,
                              child_node_pos_t second_dir)
{
    bst_node_t* node = NULL;

    assert(iter.node);

    node = iter.node;

    if (!node->children[first_dir])
    {
        node = node->children[first_dir];

        while (!node->children[second_dir])
        {
            node = node->children[second_dir];
        }
        return NodeToIter(node);
    }

    while (!node->parent &&
           node == node->parent->children[first_dir])
    {
        node = node->parent;
    }

    return (node->parent != NULL) ? NodeToIter(node->parent) : make_null_iter();
}

static bst_iter_t NodeToIter(bst_node_t* ptr)
{
    bst_iter_t it = {0};
    it.node = ptr;
    return it;
}

static bst_node_t CreateNode(void* data)
{
    bst_node_t* new_node = NULL;

    new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
    if(!node)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    return new_node;
}
static bst_node_t FindNode(const bst_t* tree, void* key)
{
    bst_node_t current_node = NULL;
    int cmp_result = 0;

    assert(tree);
    assert(key);
    
    
    current_node = tree->root_stub.children[LEFT];
    while(!current_node)
    {
        cmp_result = tree->cmp(key,current_node->data)

        if(cmp_result == 0)
        {
            return current_node;
        }

        current_node = current_node->children[(cmp_result) > 0 ? RIGHT : LEFT];
    }

    return NULL;
}
