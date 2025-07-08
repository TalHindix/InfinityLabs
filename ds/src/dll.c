/**************************************
Exercise: 	DS - DLL
Date:		8/7/2025
Developer:	Tal Hindi
Reviewer: 	Menny Markovich
Status:		
**************************************/

#include "dll.h"
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

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
static node_t* CreateNode(void* data)
{
	node_t* new_node = NULL;
	
	new_node = (node_t*)malloc(sizeof(node_t*));
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
	
	return new_node;
}

static void InsertBetween(dll_iter_t left, dll_iter_t right, dll_iter_t new_node)
{
	new_node->prev 	= left;
	new_node->next 	= right;
	left->next 		= new_node;
	right->prev 	= new_node;
}

static dll_iter_t FindEnd(dll_iter_t iter)
{
	while(DLLNext(iter))
	{
		iter = DLLNext(iter);
	}
	return iter;
}

static dll_iter_t NodeToIter(node_t* node)
{
	return (dll_iter_t)node;
}

static node_t* IterToNode(dll_iter_t iter)
{
	return (node_t*)iter;
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
	list->head.next = &list->tail;
	list->head.prev = NULL;	
	list->tail.prev = &list->head;
	list->tail.next = NULL;
	list->tail.data = NULL;
	
	return list;
}

void DLLDestroy(dll_t* list)
{
	dll_iter_t iter = NULL;
	dll_iter_t next = NULL;
	
	assert(list);
	
	iter = DLLBegin(list);
	
	while(iter != DLLEnd(list))
	{
		next = DLLNext(iter);
		free(iter);
		iter = next;
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
	assert(curr);
	
	return NodeToIter(curr->next);
}

dll_iter_t DLLPrev(dll_iter_t curr)
{
	assert(curr);
	
	return NodeToIter(curr->prev);
}

int DLLIsEqual(dll_iter_t iter1, dll_iter_t iter2)
{	
	return (iter1 == iter2);
}

void* DLLGetData(dll_iter_t iter)
{
	assert(iter);
	
	return iter->data;
}

void DLLSetData(dll_iter_t iter, void* data)
{
	assert(iter);
	assert(data);
	
	iter->data = data;
}

dll_iter_t DLLInsert(dll_t* list, dll_iter_t where, void* data)
{
	dll_iter_t new_node = NULL;
	(void)list;
	
	new_node = CreateNode(data);
	if (new_node == NULL)
	{
		return FindEnd(where);
	}
	
	InsertBetween(where->prev,where,new_node);
	return NodeToIter(new_node);
}

dll_iter_t DLLRemove(dll_iter_t to_remove)
{
	dll_iter_t next_node = DLLNext(to_remove);
	
	to_remove->prev->next = DLLNext(to_remove);
	to_remove->next->prev = DLLPrev(to_remove);
	
	to_remove->next = NULL;
	to_remove->prev = NULL;
	
	free(to_remove);
	
	return next_node;
}

dll_iter_t DLLPushFront(dll_t* list, void* data)
{
	(void)list;
	(void)data;
	
	return NULL;
}

dll_iter_t DLLPushBack(dll_t* list, void* data)
{
	(void)list;
	(void)data;
	
	return NULL;
}

void *DLLPopFront(dll_t* list) 
{
	(void)list;
	
	return NULL;
}
void *DLLPopBack(dll_t* list)
{
	(void)list;
	
	return NULL;
}
size_t DLLCount(const dll_t* list)
{
	(void)list;
	
	return 5102;
}
int DLLIsEmpty(const dll_t* list)
{	
	return DLLBegin(list) == DLLEnd(list);
}

dll_iter_t DLLFind(dll_iter_t from, dll_iter_t to,int (*is_match_func_t)(const void* data, const void* param), const void* param)
{
	(void)from;
	(void)to;
	(void)param;
	(void)is_match_func_t;
	
	return NULL;
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
        
int DLLForEach(dll_iter_t from, dll_iter_t to,int (*action_func_t)(void* data, void* param), void* param)
        
{
	(void)from;
	(void)to;
	(void)param;
	(void)action_func_t;
	
	return 5881;
}

dll_t* DLLSplice(dll_iter_t where, dll_iter_t from, dll_iter_t to)
{
	(void)where;
	(void)from;
	(void)to;
	
	return NULL;
}
