/**************************************
Exercise: 	DS - DLL
Date:		8/7/2025
Developer:	Tal Hindi
Reviewer: 	Menny Markovich
Status:		
**************************************/

#include <stddef.h> /* size_t 		*/
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

/******* HELPER FUNCTIONS ****************/

static dll_iter_t NodeToIter(node_t* node)
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

static void JoinNode(node_t *left, node_t *right)
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

static dll_iter_t RemoveNode(node_t *n)
{
    node_t *next = n->next;
    JoinNode(n->prev, n->next);
    free(n);
    return next;
}

static int CountAction(void *data, void *param)
{
    (void)data;              
    ++*(size_t *)param;       
    return 0;                 
}



/******* END OF HELPER FUNCTIONS *********/

dll_t* DLLCreate(void)
{
	dll_t* list = (dll_t*)malloc(sizeof(dll_t));
	
	if (NULL == list)
	{
		return NULL;
	}
	
	list->head.data = NULL;
	list->head.prev = NULL;	
	list->head.next = &list->tail;
	
	list->tail.next = NULL;
	list->tail.data = NULL;
	list->tail.prev = &list->head;
	
	return list;
}

void DLLDestroy(dll_t *list)
{
    dll_iter_t iter = NULL;
    dll_iter_t next = NULL;

    assert(list);

    iter = DLLBegin(list);

    while (!DLLIsEqual(iter, DLLEnd(list)))
    {
        next = DLLNext(iter);
        free(IterToNode(iter));
        iter = next;
    }

    free(list);
}

dll_iter_t DLLBegin(const dll_t *list)
{
    assert(list);
    
    return NodeToIter((node_t *)list->head.next);
}

dll_iter_t DLLEnd(const dll_t *list)
{
    assert(list);
    
    return NodeToIter((node_t *)&list->tail);
}

dll_iter_t DLLNext(dll_iter_t curr)
{
    assert(curr);
    
    return NodeToIter(IterToNode(curr)->next);
}

dll_iter_t DLLPrev(dll_iter_t curr)
{
    assert(curr);
    
    return NodeToIter(IterToNode(curr)->prev);
}

int DLLIsEqual(dll_iter_t iter1, dll_iter_t iter2)
{	
	return (iter1 == iter2);
}

void* DLLGetData(dll_iter_t iter)
{
    assert(iter);
    
    return IterToNode(iter)->data;
}

void DLLSetData(dll_iter_t iter, void *data)
{
    assert(iter);
    
    IterToNode(iter)->data = data;
}

dll_iter_t DLLInsert(dll_t *list, dll_iter_t where, void *data)
{
    node_t *n = NULL;

    assert(list);
    assert(where);

    n = CreateNode(data);
    if (!n)
    {
        return DLLEnd(list);
    }

    InsertBetween(IterToNode(where)->prev, IterToNode(where), n);

    return NodeToIter(n);
}


dll_iter_t DLLRemove(dll_iter_t iter)
{
    if (!iter || !IterToNode(iter)->prev || !IterToNode(iter)->next)
    {
        return NULL;
    }
    return NodeToIter(RemoveNode(IterToNode(iter)));
}

dll_iter_t DLLPushFront(dll_t* list, void* data)
{
	node_t* new_node = NULL;
	
	assert(list);
	
	new_node = CreateNode(data);
	if(NULL == new_node)
	{
		return DLLEnd(list);
	}
	
	InsertBetween(&list->head,list->head.next,new_node);
	
	return NodeToIter(new_node);
}

dll_iter_t DLLPushBack(dll_t* list, void* data)
{
	node_t* new_node = NULL;
	
	assert(list);
	
	new_node = CreateNode(data);
	if(NULL == new_node)
	{
		return DLLEnd(list);
	}
	
	InsertBetween(list->tail.prev,&list->tail,new_node);
	
	return NodeToIter(new_node);
}

void* DLLPopFront(dll_t *list)
{
	void* data = NULL;
	
    assert(list);

    if (DLLIsEmpty(list))
    {
        return NULL;
    }
    
   	data = list->head.next->data;
    RemoveNode(list->head.next);
    
    return data;
}

void* DLLPopBack(dll_t *list)
{
	void* data = NULL;
	
    assert(list);

    if (DLLIsEmpty(list))
    {
        return NULL;
    }
    
	data = list->tail.prev->data;
    RemoveNode(list->tail.prev);
    
    return data;
}

size_t DLLCount(const dll_t* list)
{
    size_t count = 0;
    if (!list)
    {
        return 0;
    }

    DLLForEach(DLLBegin(list), DLLEnd(list), CountAction, &count);
    
    return count;
}


int DLLIsEmpty(const dll_t *list)
{
    assert(list);
    
    return DLLIsEqual(DLLBegin(list), DLLEnd(list));
}

dll_iter_t DLLFind(dll_iter_t from, dll_iter_t to, int (*is_match_func_t)(const void* data, const void* param), const void* param)
{
    assert(from && to && is_match_func_t);

    while (!DLLIsEqual(from, to) && !is_match_func_t(DLLGetData(from), param))
    {
        from = DLLNext(from);
    }

    return from;
}

int DLLMultiFind(dll_iter_t from, dll_iter_t to,int (*is_match_func_t)(const void* data, const void* param), const void* param, dll_t* output)
{
	(void)from;
	(void)to;
	(void)param;
	(void)output;
	(void)is_match_func_t;
	
	return 39913;
	
}
        
int DLLForEach(dll_iter_t from, dll_iter_t to, dll_action_t action, void *param)
{
    int status = 0;
    assert(from && to && action);

    while (!DLLIsEqual(from, to) && 0 == status)
    {
        status = action(IterToNode(from)->data, param);
        from   = DLLNext(from);
    }
    return status;
}

dll_t* DLLSplice(dll_iter_t where, dll_iter_t from, dll_iter_t to)
{
	(void)where;
	(void)from;
	(void)to;
	
	return NULL;
}
