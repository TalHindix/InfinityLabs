/**************************************
Exercise:  DS - BST
Date:      7/8/2025
Developer: Tal Hindi
Reviewer:
Status:
**************************************/

#include <stddef.h> /* size_t       */
#include <stdlib.h> /* malloc       */
#include <assert.h> /* assert       */

#include "bst.h"    /* BSTCreate    */

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
static bst_iter_t NodeToIter(bst_node_t* ptr);
static bst_node_t* CreateNode(void* data);
static bst_node_t* FindNode(const bst_t* tree, void* key);
static bst_node_t* Root(const bst_t* tree);
static int IsLeftChild(bst_node_t* node);
static void Link(bst_node_t* parent, child_node_pos_t side, bst_node_t* child);
static void Transplant(bst_node_t* u, bst_node_t* v);
static bst_node_t* MinNode(bst_node_t* node);
static bst_node_t* Leftmost(bst_node_t* n);
static bst_node_t* Rightmost(bst_node_t* n);
static int IsLeaf(bst_node_t* node);

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

    node = tree->root_stub.children[LEFT];

    /* walk down to leaves, free them, and climb up*/
    while ((NULL != node) && (NULL != node->parent))
    {
        if (IsLeaf(node))
        {
            child_node_pos_t side = LEFT;

            parent = node->parent;

            /* decide if this node is the LEFT or RIGHT child of its parent */
            if ((NULL != parent->children[RIGHT]) && (parent->children[RIGHT] == node))
            {
                side = RIGHT;
            }

            /* detach from parent, free, and climb */
            parent->children[side] = NULL;
            free(node);
            node = parent;
        }
        else
        {
            if (NULL != node->children[RIGHT])
            {
                node = node->children[RIGHT];
            }
            else
            {
                node = node->children[LEFT];
            }
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

    /* find the insertion point */
    while (current)
    {
        parent = current;
        cmp_result = tree->cmp(data, current->data);

        if (cmp_result > 0)
        {
            current = current->children[RIGHT];
        }
        else
        {
            current = current->children[LEFT];
        }
    }

    new_node = CreateNode(data);
    if (!new_node)
    {
        return BSTEnd(tree);
    }

    new_node->parent = parent;

    if (parent == &tree->root_stub || cmp_result < 0)
    {
        parent->children[LEFT] = new_node;
    }
    else
    {
        parent->children[RIGHT] = new_node;
    }

    return NodeToIter(new_node);
}

void BSTRemove(bst_iter_t to_remove)
{
    bst_node_t* node_to_remove = NULL;
    bst_node_t* succ = NULL;

    assert(to_remove.node);
    assert(to_remove.node->parent != NULL);

    node_to_remove = to_remove.node;

    if (!node_to_remove->children[LEFT])
    {
        Transplant(node_to_remove, node_to_remove->children[RIGHT]);    
        free(node_to_remove);
        return;
    }

    if (!node_to_remove->children[RIGHT])
    {
        Transplant(node_to_remove, node_to_remove->children[LEFT]);
        free(node_to_remove);
        return;
    }

    succ = MinNode(node_to_remove->children[RIGHT]);

    if (succ->parent != node_to_remove)
    {
        Transplant(succ, succ->children[RIGHT]);
        Link(succ, RIGHT, node_to_remove->children[RIGHT]);
    }

    Transplant(node_to_remove, succ);
    Link(succ, LEFT, node_to_remove->children[LEFT]);
    free(node_to_remove);
}

size_t BSTSize(const bst_t *tree)
{
    size_t count = 0;
    bst_iter_t it;

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
    return (Root(tree) == NULL);
}

bst_iter_t BSTBegin(const bst_t* tree)
{
    bst_node_t* n = NULL;

    assert(tree);
    n = Root(tree);
    return n ? NodeToIter(Leftmost(n)) : BSTEnd(tree);
}

bst_iter_t BSTEnd(const bst_t* tree)
{
    assert(tree);
    return NodeToIter((bst_node_t*)&tree->root_stub);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    return StepInOrder(iter, LEFT, RIGHT);
}

bst_iter_t BSTNext(bst_iter_t iter)
{
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

static bst_iter_t StepInOrder(bst_iter_t iter,
                              child_node_pos_t first_dir,
                              child_node_pos_t second_dir)
{
    bst_node_t* node = NULL;

    assert(iter.node);
    node = iter.node;

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

static bst_iter_t NodeToIter(bst_node_t* ptr)
{
    bst_iter_t it;
    it.node = ptr;
    return it;
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

static void Link(bst_node_t* parent, child_node_pos_t side, bst_node_t* child)
{
    parent->children[side] = child;
    if (child)
    {
        child->parent = parent;
    }
}

static void Transplant(bst_node_t* u, bst_node_t* v)
{
    bst_node_t* p = u->parent;

    if (p->parent == NULL)
    {
        Link(p, LEFT, v);
        return;
    }

    Link(p, IsLeftChild(u) ? LEFT : RIGHT, v);
}

static bst_node_t* MinNode(bst_node_t* node)
{
    return Leftmost(node);
}

static bst_node_t* Leftmost(bst_node_t* n)
{
    while (n && n->children[LEFT] && n->children[LEFT] != n)
    {
        n = n->children[LEFT];
    }
    return n;
}

static bst_node_t* Rightmost(bst_node_t* n)
{
    while (n && n->children[RIGHT] && n->children[RIGHT] != n)
    {
        n = n->children[RIGHT];
    }
    return n;
}

static int IsLeaf(bst_node_t* node)
{
    return !node->children[LEFT] && !node->children[RIGHT];
}