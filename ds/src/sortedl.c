/**************************************
Exercise:  DS - SortedL
Date:      8/7/2025
Developer: Tal Hindi
Reviewer:  Yael Keidar
Status:    Approved
**************************************/

#include <stdlib.h>  /* malloc */
#include <assert.h>  /* assert */

#include "sortedl.h" /* SortedLCreate */

struct sortedl
{
    dll_t* list;
    int (*cmp)(const void *data1, const void *data2);
};

struct cmp_wrapper
{
    int (*cmp)(const void *, const void *);
    const void *data;
};

static sorted_iter_t DLLIterToSortedIter(dll_iter_t iter, sortedl_t* list);
static dll_iter_t SortedIterToDLLIter(sorted_iter_t s_iter);
static sorted_iter_t GetSortedPosition(sortedl_t* list, sorted_iter_t from, sorted_iter_t to, const void* data);
static int IsNotLessThanInsertValue(const void *data_in_list, const void *param);

sortedl_t* SortedLCreate(int (*comp)(const void* data1, const void* data2))
{
	sortedl_t* sorted_list = NULL;
	
	if (NULL == comp)
	{
		return NULL;
	}
	
	sorted_list = (sortedl_t*)malloc(sizeof(sortedl_t));
	if (NULL == sorted_list)
	{
		return NULL;
	}
	
    sorted_list->list = DLLCreate();
    if (NULL == sorted_list->list)
    {
    	free(sorted_list);
    	return NULL;
    }
    
    sorted_list->cmp = comp;
    
    return sorted_list;
}

void SortedLDestroy(sortedl_t* list)
{
	assert(list);
	
	DLLDestroy(list->list);
	list->list = NULL;
    list-> cmp = NULL;
    	
	free(list);
	list = NULL;
}

sorted_iter_t SortedLInsert(sortedl_t* list, void *data)
{
    dll_iter_t where = {0};
    dll_iter_t new_dll_iter = {0};
    
    assert(list);
    
	where = GetSortedPosition(list, SortedLBegin(list), SortedLEnd(list), data).iter;
	new_dll_iter = DLLInsert(list->list, where, data);
	
    return DLLIterToSortedIter(new_dll_iter, list);
}

sorted_iter_t SortedLRemove(sorted_iter_t to_remove)
{
	
    sorted_iter_t after_iter = {0};
	
	assert(to_remove.iter);
	assert(to_remove.list);
	
	after_iter = SortedLNext(to_remove);
	
	DLLRemove(to_remove.iter);
	
	return after_iter;

}


size_t SortedLSize(const sortedl_t* list)
{
    assert(list);
    
    return DLLCount(list->list);
}

int SortedLIsEmpty(const sortedl_t* list)
{
    assert(list);
    
    return DLLIsEmpty(list->list);
}

sorted_iter_t SortedLFind(sortedl_t *list, sorted_iter_t from, sorted_iter_t to, const void *target)
{
   	assert(list);
	assert(from.iter);
	assert(from.list);
	assert(to.iter);
	assert(to.list);
   
    from = GetSortedPosition(list, from, to, target);
    
    if (SortedLIsEqual(from, to))
    {
        return to;
    }

    if (0 == list->cmp(target,DLLGetData(SortedIterToDLLIter(from))))
    {
        return from;
    }
    
	return to;
}

sorted_iter_t SortedLFindIf(sorted_iter_t from, sorted_iter_t to, int (*is_match_func)(const void* data, const void* param), const void* param)
{	
	assert(from.iter);
	assert(from.list);
	assert(to.iter);
	assert(to.list);
	assert(is_match_func);
	
	from.iter = DLLFind(from.iter, to.iter, is_match_func, param);
	
	return DLLIterToSortedIter(from.iter, from.list);	
}


int SortedLIsEqual(sorted_iter_t iter1, sorted_iter_t iter2)
{
    return SortedIterToDLLIter(iter1) == SortedIterToDLLIter(iter2);
}

sorted_iter_t SortedLNext(sorted_iter_t curr)
{
    return DLLIterToSortedIter(DLLNext(SortedIterToDLLIter(curr)), curr.list);
}

sorted_iter_t SortedLPrev(sorted_iter_t curr)
{
    return DLLIterToSortedIter(DLLPrev(SortedIterToDLLIter(curr)), curr.list);
}

void* SortedLGetData(sorted_iter_t iter)
{
    return DLLGetData(SortedIterToDLLIter(iter));
}

sorted_iter_t SortedLBegin(const sortedl_t *list)
{
    assert(list);
    return DLLIterToSortedIter(DLLBegin(list->list), (sortedl_t *)list);
}

sorted_iter_t SortedLEnd(const sortedl_t *list)
{
    assert(list);
    return DLLIterToSortedIter(DLLEnd(list->list), (sortedl_t *)list);
}

void* SortedLPopFront(sortedl_t *list)
{
	sorted_iter_t front = SortedLBegin(list);
	
	void* data = SortedLGetData(front);
	
    SortedLRemove(front);    
    return data;
}

void* SortedLPopBack(sortedl_t *list)
{
    sorted_iter_t back = SortedLPrev(SortedLEnd(list));
    
    void* data = SortedLGetData(back);
    
    SortedLRemove(back);
    
    return data;
}

void SortedLMerge(sortedl_t* dest, sortedl_t *src)
{
    sorted_iter_t d 		= {0};
    sorted_iter_t s 		= {0};
    sorted_iter_t end_d 	= {0};
    sorted_iter_t end_s 	= {0};
    sorted_iter_t next_s 	= {0};

    assert(dest);
    assert(src);
    assert(dest->cmp == src->cmp);

    d      			= SortedLBegin(dest);
    end_d 			= SortedLEnd(dest);
    s      			= SortedLBegin(src);
    end_s  			= SortedLEnd(src);

    while (!SortedLIsEqual(s, end_s))
    {
        while (!SortedLIsEqual(d, end_d) && dest->cmp(SortedLGetData(d), SortedLGetData(s)) < 0)
        {
            d = SortedLNext(d);
        }

        next_s = SortedLNext(s);
        DLLSplice(d.iter, s.iter, next_s.iter);
        s = next_s;
    }
}



int SortedLForEach(sorted_iter_t from, sorted_iter_t to, int (*action_func)(void* data, void* param), void* param)
{
	assert(from.list == to.list);
	assert(action_func);

	return DLLForEach(from.iter, to.iter, action_func, param);
}

/***************** HELPER **************************/
static dll_iter_t SortedIterToDLLIter(sorted_iter_t s_iter)
{
	(void)s_iter;
	return s_iter.iter;
}

static sorted_iter_t DLLIterToSortedIter(dll_iter_t iter, sortedl_t* list)
{
	sorted_iter_t s_iter;
	s_iter.iter = iter;
#ifndef NDEBUG
	s_iter.list = list;
#endif
	return s_iter;
}


static int IsNotLessThanInsertValue(const void *data_in_list, const void *param)
{
    const struct cmp_wrapper *wrapper = (const struct cmp_wrapper *)param;

    return wrapper->cmp(data_in_list, wrapper->data) >= 0;
}


static sorted_iter_t GetSortedPosition(sortedl_t* list, sorted_iter_t from, sorted_iter_t to, const void* data)
{
    struct cmp_wrapper param;
    dll_iter_t where;

    param.cmp = list->cmp;
    param.data = data;

    where = DLLFind(from.iter, to.iter, IsNotLessThanInsertValue, &param);

    return DLLIterToSortedIter(where, list);
}







