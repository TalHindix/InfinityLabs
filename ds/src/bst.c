/**************************************
Exercise:   DS - BST
Date:       7/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     Approved
**************************************/

#include <stddef.h> /* size_t       */
#include <stdlib.h> /* malloc       */
#include <assert.h> /* assert       */

#include "bst.h"    /* BSTCreate    */

#define LEFT_CHILD(node) (node->children[LEFT])
#define RIGHT_CHILD(node) (node->children[RIGHT])

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

static bst_iter_t StepInOrder(bst_iter_t iter, child_node_pos_t first_dir, child_node_pos_t second_dir);
static bst_node_t* IterToNode(bst_iter_t iter);
static bst_iter_t NodeToIter(bst_node_t* node);
static bst_node_t* CreateNode(void* data);
static bst_node_t* FindNode(const bst_t* tree, void* key);
static bst_node_t* Root(const bst_t* tree);
static int IsLeftChild(bst_node_t* node);
static void AttachChild(bst_node_t* parent, child_node_pos_t side, bst_node_t* child);
static void ReplaceSubtree(bst_node_t* u, bst_node_t* v);
static bst_node_t* Leftmost(bst_node_t* node);
static bst_node_t* Rightmost(bst_node_t* node);
static int IsLeaf(bst_node_t* node);
static int IsBegin(bst_iter_t iter);
static int AddOne(void* data, void* param);

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
    bst_node_t* parent = NULL;
    bst_node_t* node = NULL;

    assert(tree);

    node = Root(tree);

    /* walk down to leaves, free them, and climb up*/
    while (!BSTIsEmpty(tree))
    {
        if (LEFT_CHILD(node))
        {
            node = LEFT_CHILD(node);
        }
        else if (RIGHT_CHILD(node))
        {
            node = RIGHT_CHILD(node);
        }
        else
        {
            parent = node->parent;

            if(parent)
            {
                if(LEFT_CHILD(parent) == node)
                {
                    LEFT_CHILD(parent) = NULL;
                }
                else
                {
                    RIGHT_CHILD(parent) = NULL;
                }
            }

            free(node);
            node = parent;
        }
        
    }

    free(tree);
}

bst_iter_t BSTInsert(bst_t* tree, void* data)
{
    bst_node_t* new_node = NULL;
    bst_node_t* parent = NULL;
    bst_node_t* current = NULL;
    int cmp_result = 0;

    assert(tree);
    assert(data);

    current = Root(tree);
    parent = &tree->root_stub;

    new_node = CreateNode(data);
    if (!new_node)
    {
        return BSTEnd(tree);
    }

    if (BSTIsEmpty(tree))
    {
        LEFT_CHILD(parent) = new_node;
        new_node->parent = parent; /* parent is &tree->root_stub */
        return NodeToIter(new_node);
    }

    while (current)
    {
        parent = current;
        cmp_result = tree->cmp(data, current->data);

        assert(cmp_result);

        if (cmp_result > 0)
        {
            current = RIGHT_CHILD(current);
        }
        else
        {
            current = LEFT_CHILD(current);
        }
    }

    new_node->parent = parent;

    if (cmp_result < 0)
    {
        LEFT_CHILD(parent) = new_node;
    }
    else
    {
        RIGHT_CHILD(parent) = new_node;
    }

    return NodeToIter(new_node);
}


void BSTRemove(bst_iter_t to_remove)
{
    bst_node_t* node_to_remove = NULL;
    bst_node_t* next = NULL;

    assert(to_remove.node);
    assert(to_remove.node->parent != NULL);

    node_to_remove = IterToNode(to_remove);

    if (!LEFT_CHILD(node_to_remove))
    {
        ReplaceSubtree(node_to_remove, RIGHT_CHILD(node_to_remove));    
        free(node_to_remove);
        return;
    }

    if (!RIGHT_CHILD(node_to_remove))
    {
        ReplaceSubtree(node_to_remove, LEFT_CHILD(node_to_remove));
        free(node_to_remove);
        return;
    }

    next = IterToNode(BSTNext(NodeToIter(node_to_remove)));

    if (next->parent != node_to_remove)
    {
        ReplaceSubtree(next, RIGHT_CHILD(next));
        AttachChild(next, RIGHT, RIGHT_CHILD(node_to_remove));
    }

    ReplaceSubtree(node_to_remove, next);

    AttachChild(next, LEFT, LEFT_CHILD(node_to_remove));
    
    free(node_to_remove);
}

size_t BSTSize(const bst_t* tree)
{
    size_t count = 0;

    assert(tree);

    BSTForEach(BSTBegin(tree), BSTEnd(tree), AddOne, &count);

    return count;
}

int BSTIsEmpty(const bst_t* tree)
{
    assert(tree);
    return (NULL == Root(tree));
}

bst_iter_t BSTBegin(const bst_t* tree)
{
    bst_node_t* node = NULL;

    assert(tree);

    node = Root(tree);

    return node ? NodeToIter(Leftmost(node)) : BSTEnd(tree);
}

bst_iter_t BSTEnd(const bst_t* tree)
{
    assert(tree);

    return NodeToIter(tree->root_stub.children[RIGHT]);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    assert(!IsBegin(iter));

    return StepInOrder(iter, LEFT, RIGHT);
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    assert(IterToNode(iter)->parent != NULL);

    return StepInOrder(iter, RIGHT, LEFT);
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

    node = FindNode(bst, to_find);
    if (node)
    {
        return NodeToIter(node);
    }

    return BSTEnd((bst_t*)bst);
}

int BSTForEach(bst_iter_t from, bst_iter_t to,
               int (*action_func)(void* data, void* param), void* param)
{
    int status = 0;
    bst_iter_t it;

    assert(action_func);
    assert(from.node);
    assert(to.node);

    it = from;
    while (!BSTIterIsSame(it, to))
    {
        status = action_func(BSTGetData(it), param);
        if (status)
        {
            return status;
        }
        it = BSTNext(it);
    }

    return 0;
}

/**************************************HELPER FUNCTIONS  ********************************************************/

static bst_iter_t StepInOrder(bst_iter_t iter,
                              child_node_pos_t first_dir,
                              child_node_pos_t second_dir)
{
    bst_node_t* node = NULL;

    assert(IterToNode(iter));

    node = IterToNode(iter);

    if (node->children[first_dir])
    {
        node = node->children[first_dir];
        node = (second_dir == LEFT) ? Leftmost(node) : Rightmost(node);
        return NodeToIter(node);
    }

    while (node->parent && node == node->parent->children[first_dir])
    {
        node = node->parent;
    }

    return node->parent ? NodeToIter(node->parent) : NodeToIter(node);
}

static bst_node_t* IterToNode(bst_iter_t iter)
{
	return (iter.node);
}

static bst_iter_t NodeToIter(bst_node_t* node)
{
    bst_iter_t iter = {0};

    assert(node);

    iter.node = node;

	return iter;
}

static bst_node_t* CreateNode(void* data)
{
    bst_node_t* new_node = NULL;

    new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
    if (!new_node)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->parent = NULL;

    return new_node;
}

static bst_node_t* FindNode(const bst_t* tree, void* key)
{
    bst_node_t* current_node = NULL;
    int cmp_result = 0;

    assert(tree);
    assert(key);
    
    current_node = Root(tree);

    while (current_node)
    {
        cmp_result = tree->cmp(key, current_node->data);

        if (cmp_result == 0)
        {
            return current_node;
        }

        current_node = current_node->children[(cmp_result > 0) ? RIGHT : LEFT];
    }

    return NULL;
}

static bst_node_t* Root(const bst_t* tree)
{
    return tree->root_stub.children[LEFT];
}

static int IsLeftChild(bst_node_t* node)
{
    assert(node);
    return node->parent && (node->parent->children[LEFT] == node);
}

static void AttachChild(bst_node_t* parent, child_node_pos_t side, bst_node_t* child)
{
    parent->children[side] = child;
    if (child)
    {
        child->parent = parent;
    }
}

static void ReplaceSubtree(bst_node_t* u, bst_node_t* v)
{
    bst_node_t* p = u->parent;

    if (p->parent == NULL)
    {
        AttachChild(p, LEFT, v);
        return;
    }

    AttachChild(p, IsLeftChild(u) ? LEFT : RIGHT, v);
}

static bst_node_t* Leftmost(bst_node_t* node)
{
    while (node && node->children[LEFT] && node->children[LEFT] != node)
    {
        node = node->children[LEFT];
    }
    return node;
}

static bst_node_t* Rightmost(bst_node_t* node)
{
    while (node && node->children[RIGHT] && node->children[RIGHT] != node)
    {
        node = node->children[RIGHT];
    }
    return node;
}

static int IsLeaf(bst_node_t* node)
{
    return !node->children[LEFT] && !node->children[RIGHT];
}

static int IsBegin(bst_iter_t iter)
{
    bst_node_t* node = IterToNode(iter);

    if (LEFT_CHILD(node))
    {
        return 0;
    }

    while (node->parent)
    {
        if (node == node->parent->children[RIGHT])
        {
            return 0;
        }

        node = node->parent;
    }

    return 1;
}

static int AddOne(void* data, void* param)
{
    (void)data;
    *(size_t*)param += 1;
    
    return 0;
}

