/**************************************
Exercise: 	DS - DLL
Date:		8/7/2025
Developer:	Tal Hindi
Reviewer: 	Menny Markovich
Status:		Approved
**************************************/

#include <stdlib.h> /* malloc 		*/
#include <assert.h> /* assert 		*/

#include "dll.h" 	/* DLLCreate 	*/

struct node
{
    void* data;
    node_t* next;
    node_t* prev;
};

struct dll
{
    node_t head;
    node_t tail;
};

static dll_iter_t NodeToIter(const node_t* node);
static node_t* IterToNode(dll_iter_t iter);
static node_t* CreateNode(void* data);
static void JoinNode(node_t *left, node_t *right);
static void InsertBetween(node_t *left, node_t *right, node_t *new_node);
static dll_iter_t RemoveNode(node_t* node);
static int CountAction(void *data, void *param);

#ifndef NDEBUG
static int IsHead(dll_iter_t iter);
static int IsTail(dll_iter_t iter);
#endif


dll_t* DLLCreate(void)
{
	dll_t* list = NULL;
	
	list = (dll_t*)malloc(sizeof(dll_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	list->head.data = NULL;
	list->head.prev = NULL;		
	list->tail.next = NULL;
	list->tail.data = NULL;
	
	JoinNode(&list->head, &list->tail);
	
	return list;
}

void DLLDestroy(dll_t* list)
{

    assert(list);

    while (!DLLIsEmpty(list))
    {
      IterToNode(DLLRemove(DLLBegin(list)));
    }

    free(list);
    list = NULL;
}

dll_iter_t DLLBegin(const dll_t* list)
{
    assert(list);
    
    return NodeToIter(list->head.next);
}

dll_iter_t DLLEnd(const dll_t* list)
{
    assert(list);
    
    return NodeToIter(&list->tail);
}

dll_iter_t DLLNext(dll_iter_t curr)
{
    assert(IterToNode(curr));
    
    return NodeToIter(IterToNode(curr)->next);
}

dll_iter_t DLLPrev(dll_iter_t curr)
{
    assert(IterToNode(curr));
    
    return NodeToIter(IterToNode(curr)->prev);
}

int DLLIsEqual(dll_iter_t iter1, dll_iter_t iter2)
{
	assert(IterToNode(iter1));
	assert(IterToNode(iter2));
		
	return (IterToNode(iter1) == IterToNode(iter2));
}

void* DLLGetData(dll_iter_t iter)
{
    assert(IterToNode(iter));
    assert(!IsHead(iter));
    assert(!IsTail(iter));
    
    return IterToNode(iter)->data;
}

void DLLSetData(dll_iter_t iter, void* data)
{
    assert(IterToNode(iter));
    assert(!IsHead(IterToNode(iter)));
    assert(!IsTail(IterToNode(iter)));
    
    IterToNode(iter)->data = data;
}

dll_iter_t DLLInsert(dll_t *list, dll_iter_t where, void *data)
{
    node_t* to_insert = NULL;

    assert(list);
    assert(IterToNode(where));
    assert(!IsHead(where));

    to_insert = CreateNode(data);
    if (NULL == to_insert)
    {
        return DLLEnd(list);
    }

    InsertBetween(IterToNode(where)->prev, IterToNode(where), to_insert);

    return NodeToIter(to_insert);
}


dll_iter_t DLLRemove(dll_iter_t to_remove)
{
    assert(IterToNode(to_remove));
    assert(!IsHead(IterToNode(to_remove)));			
	assert(!IsTail(IterToNode(to_remove)));	

    
    return NodeToIter(RemoveNode(IterToNode(to_remove)));
}

dll_iter_t DLLPushFront(dll_t* list, void* data)
{
	assert(list);
	
	return DLLInsert(list, DLLBegin(list), data);
}

dll_iter_t DLLPushBack(dll_t* list, void* data)
{
	assert(list);
	
	return DLLInsert(list, DLLEnd(list), data);
}

void* DLLPopFront(dll_t* list)
{
	void* data = NULL;
	
    assert(list);
    assert(!DLLIsEmpty(list));
    
	data = DLLGetData(DLLBegin(list));
    DLLRemove(DLLBegin(list));
    
    return data;
}

void* DLLPopBack(dll_t *list)
{
	void* data = NULL;
	dll_iter_t back = NULL;
	
    assert(list);
	assert(!DLLIsEmpty(list));
	
	back = DLLPrev(DLLEnd(list));
	data = DLLGetData(back);
    DLLRemove(back);
    
    return data;
}

size_t DLLCount(const dll_t* list)
{
    size_t count = 0;
    
    assert(list);

    DLLForEach(DLLBegin(list), DLLEnd(list), CountAction, &count);
    
    return count;
}


int DLLIsEmpty(const dll_t* list)
{
    assert(list);
    
    return DLLIsEqual(DLLBegin(list), DLLEnd(list));
}

dll_iter_t DLLFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void* data, const void* param), const void* param)
{
    assert(IterToNode(from));
    assert(IterToNode(to));
    assert(is_match_func_t);

    while (!DLLIsEqual(from, to) && !is_match_func_t(DLLGetData(from), param))
    {
        from = DLLNext(from);
    }

    return from;
}

int DLLMultiFind(dll_iter_t from, dll_iter_t to,int (*is_match_func_t)(const void* data, const void* param), const void* param, dll_t* output)
{
	size_t count = 0;
	dll_iter_t iter = NULL;
		
	assert(output);
	assert(IterToNode(from));
	assert(IterToNode(to));
	assert(is_match_func_t);
	
	while (!DLLIsEqual(from,to))
	{
	
		if(is_match_func_t(DLLGetData(from),param))
		{
			iter = DLLPushBack(output, DLLGetData(from));
					
			if(DLLIsEqual(iter,DLLEnd(output)))
			{
				return (count == 0) ? -1 : -2;
			}
			
			++count;
		}					
		from = DLLNext(from);	
	}
	
	return 0;
}
        
int DLLForEach(dll_iter_t from, dll_iter_t to, int (*action_func_t)(void* data, void* param), void *param)
{
    int status = 0;
    assert(IterToNode(from));
    assert(IterToNode(to));
    assert(action_func_t);
    while (!DLLIsEqual(from, to) && 0 == status)
    {
        status = action_func_t(IterToNode(from)->data, param);
        from   = DLLNext(from);
    }
    return status;
}

dll_iter_t DLLSplice(dll_iter_t where, dll_iter_t from, dll_iter_t to)
{
	dll_iter_t last = NULL;
		
	assert(IterToNode(where));
	assert(IterToNode(from));
	assert(IterToNode(to));
	assert(!DLLIsEqual(from,to));
	
	last = DLLPrev(IterToNode(to));
	
	JoinNode(DLLPrev(from),IterToNode(to));
	JoinNode(DLLPrev(where),IterToNode(from));
	JoinNode(IterToNode(last), IterToNode(where));
	
	return where;
}

/* Implementation of the helper functions */

static dll_iter_t NodeToIter(const node_t* node)
{
	return (dll_iter_t)node;
}

static node_t* IterToNode(dll_iter_t iter)
{
	return (node_t*)iter;
}

static node_t* CreateNode(void* data)
{
	node_t* new_node = NULL;
	
	new_node = (node_t*)malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
	
	return new_node;
}

static void JoinNode(node_t* left,node_t* right)
{
    assert(left && right);
    left->next  = right;
    right->prev = left;
}

static void InsertBetween(node_t *left, node_t *right, node_t *new_node)
{
    JoinNode(left, new_node);
    JoinNode(new_node, right);
}

static dll_iter_t RemoveNode(node_t* node)
{
    node_t* next = NULL;
    next = node->next;
    JoinNode(node->prev, node->next);
    free(node);
    return next;
}

static int CountAction(void *data, void *param)
{
    (void)data;              
    ++*(size_t *)param;       
    return 0;                 
}


#ifndef NDEBUG
static int IsHead(dll_iter_t iter)
{
	return (NULL == IterToNode(iter)->prev);
}
 

static int IsTail(dll_iter_t iter)
{
	return (NULL == IterToNode(iter)->next);
}
#endif


