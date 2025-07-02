/**************************************
Exercise: 	ds - SLL
Date:		2/7/2025
Developer:	Tal Hindi
Reviewer: 	Lotem Kitaroo
Status:		
**************************************/

#include "../include/sll.h"

#include <stdlib.h>   /* malloc, free */
#include <stddef.h>
#include <assert.h>


struct node
{
    void* data;
    node_t* next;
};

struct sll
{
    node_t* head;
    node_t* tail;
};

sll_t* SLLCreate(void)                        
{
    sll_t* list = NULL;
    node_t* dummy = NULL;
    
    list =  (sll_t *)malloc(sizeof(sll_t));
    
    if (NULL == list)
    {
        return NULL;
    }
    
    list->head = NULL;
	list->tail = NULL;
	
    dummy = (node_t *)malloc(sizeof(node_t));
    if (NULL == dummy)
    {
        free(list);
        return NULL;
    }
    
    dummy->data = list;
    dummy->next = NULL;

    list->head = dummy;
    list->tail = dummy;
    
    return list;
}

void SLLDestroy(sll_t* list)
{
	assert(list);
	node_t* current_node = NULL;
	
	if (NULL == list)
	{
		return;
	}
	
	current_node = SLLBegin(list);
	
	while(current_node)
	{ 
		node_t* next = SLLNext(current_node);
		free(current_node);
		current_node = next;
	}
	
	free(list);
}

sll_iter_t SLLBegin(const sll_t* list)
{
	assert(list);
	return list->head;
}

sll_iter_t SLLEnd(const sll_t* list)
{
	assert(list);
	return list->tail;
}

sll_iter_t SLLNext(sll_iter_t iter)
{
	assert(iter);
	return iter->next;
}

int SLLIsEqual(sll_iter_t iter1, sll_iter_t iter2)
{
	assert(iter1);
	assert(iter2);
	return iter1 == iter2;	
}

void* SLLGetData(sll_iter_t iter)
{
	assert(iter);
	return iter->data;
}

void SLLSetData(sll_iter_t iter, void* data)
{
	iter->data = data;
}

sll_iter_t SLLInsert(sll_iter_t where, void* data)
{
	node_t* node = NULL;
	node = (node_t*)malloc(sizeof(node_t));
	
	if (NULL == node)
	{
		while (SLLNext(where))
		{
			where = SLLNext(where);
		}		
		return where;
	}
	
	node->data = SLLGetData(where);
	node->next = SLLNext(where);
	where->next = node;
	SLLSetData(where,data);
	
	if (NULL == SLLNext(node))
	{
		((sll_t*)(SLLGetData(node)))->tail = node;
	}
	
	return where;
}

    
sll_iter_t SLLRemove(sll_iter_t to_remove)
{
	node_t* tmp = SLLNext(to_remove);
	assert(NULL != tmp);
	
	to_remove->data = SLLGetData(tmp);
	to_remove->next = SLLNext(tmp);
	
	free(tmp);

	if (NULL == SLLNext(to_remove))
	{
		((sll_t*)(SLLGetData(to_remove)))->tail = to_remove;
	}
		
	return to_remove;
}




int SLLIsEmpty(const sll_t* list)
{
	assert(list);
	return SLLBegin(list) == SLLEnd(list);
}

size_t SLLCount(const sll_t *list)
{
    size_t count = 0;
    node_t* iter = NULL;

    if (NULL == list)
    {
    	return 0;
    }

    for (iter = SLLBegin(list); SLLNext(iter); iter = SLLNext(iter))
    {
        ++count;
    }
    return count;
}

sll_iter_t SLLFind(sll_iter_t from, sll_iter_t to,is_match_func_t is_match, void *param) 
{
    assert(is_match);
    while (0 == SLLIsEqual(from, to))
    {
        if (0 != is_match(from->data, param))
        {
            return from;
        }
        from = SLLNext(from);
    }
    
    return to;
}

int SLLForEach(sll_iter_t from, sll_iter_t to,action_func_t action_func, void *param)
{
    int rc = 0;
    assert(action_func);

    while ((0 == SLLIsEqual(from, to)) && (0 == rc))
    {
        rc = action_func(from->data, param);
        from = SLLNext(from);
    }
    return rc;
}





