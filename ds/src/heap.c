/**************************************
Exercise:   DS - Heap
Date:       25/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */

#include "heap.h" /* HeapCreate */

#define LEFT(i) ((size_t)((i) * 2 + 1))
#define RIGHT(i) ((size_t)((i) * 2 + 2))
#define PARENT(i) (((i) - 1) / 2)

struct heap
{
    compare_func compare;
    vector_t* vector;
};

/* prototypes */
static void* GetElem(const heap_t* heap, size_t idx);
static void SetElem(heap_t* heap, size_t idx,void* data);
static void SwapElem(heap_t* heap, size_t i, size_t j);
static void HeapifyUp(heap_t* heap, size_t idx);
static void HeapifyDown(heap_t* heap, size_t idx);
static size_t LargerChild(heap_t *heap, size_t parent_idx, size_t size);

heap_t* HeapCreate(compare_func func)
{
    heap_t* heap = NULL;

    assert(func);

    heap = (heap_t*)malloc(sizeof(heap_t));
    if(NULL == heap)
    {
        return NULL;
    }

    heap->compare = func;
    heap->vector = VectorCreate(DEFAULT_CAPACITY,sizeof(void*));
    if(NULL == heap->vector)
    {
        free(heap);
        return NULL;
    }

    return heap;
}

void HeapDestroy(heap_t* heap)
{
    assert(heap);

    VectorDestroy(heap->vector);

    free(heap);
}

void* HeapPeek(const heap_t* heap)
{
    assert(heap);

    if(HeapIsEmpty(heap))
    {
        return NULL;
    }

    return *(void **)VectorGetAccessToElement(heap->vector,0);
}

int HeapPush(heap_t* heap, void* data)
{
    int status = 0;

    assert(heap);
    
    status = VectorPushBack(heap->vector,&data);
    if(0 != status)
    {
        return status;
    }

    HeapifyUp(heap, VectorSize(heap->vector) - 1);

    return status;
}

void HeapPop(heap_t* heap)
{
    size_t last_ele = 0;

    assert(heap);

    if (HeapIsEmpty(heap))
    {
        return;
    }

    last_ele = VectorSize(heap->vector) - 1;
    SwapElem(heap,(size_t)0,last_ele);
    VectorPopBack(heap->vector);

    if (!HeapIsEmpty(heap))
    {
        HeapifyDown(heap, 0);
    }

}

void HeapRemove(heap_t* heap, const void* data, is_match_func is_match)
{
    size_t i = 0;
    size_t last = 0;

    assert(heap);
    assert(is_match);

    if(HeapIsEmpty(heap))
    {
        return;
    }

    for(i = 0; i < VectorSize(heap->vector); ++i)
    {
        
        if(0 == is_match(GetElem(heap,i),data))
        {
            last = VectorSize(heap->vector) - 1;

            SwapElem(heap, i, last);
            VectorPopBack(heap->vector);

            /* Decide direction once */
            if (i > 0 && heap->compare(GetElem(heap, i), GetElem(heap, PARENT(i))) > 0)
            {
                HeapifyUp(heap, i);
            }
            else
            {
                HeapifyDown(heap, i);
            }

            return;
        }
    }

}

int HeapIsEmpty(const heap_t* heap)
{
    assert(heap);
    return (0 == VectorSize(heap->vector));
}

size_t HeapSize(const heap_t* heap)
{
    assert(heap);
    return VectorSize(heap->vector);
}

/* Helper Functions */
static void* GetElem(const heap_t* heap, size_t idx)
{
    return *(void **)VectorGetAccessToElement(heap->vector,idx);
}

static void SetElem(heap_t* heap, size_t idx,void* data)
{
    *(void **)VectorGetAccessToElement(heap->vector,idx) = data;
}

static void SwapElem(heap_t* heap, size_t i, size_t j)
{
    void* temp = GetElem(heap,i);
    SetElem(heap,i,GetElem(heap,j));
    SetElem(heap,j,temp);
}

static void HeapifyUp(heap_t* heap, size_t idx)
{
    size_t parent_idx = 0;
    void* curr_val = NULL;
    void* parent_val = NULL;

    while(idx > 0)
    {
        parent_idx = PARENT(idx);
        curr_val = GetElem(heap,idx);
        parent_val = GetElem(heap,parent_idx);

        if(heap->compare(curr_val,parent_val) > 0)
        {
            SwapElem(heap,idx,parent_idx);
            idx = parent_idx;
        }
        else
        {
            return;
        }
    }
}

static void HeapifyDown(heap_t* heap, size_t idx)
{
    size_t size = VectorSize(heap->vector);
    size_t larger_child = 0;

    while(LEFT(idx) < size)
    {
        larger_child = LargerChild(heap,idx,size);

        if (heap->compare(GetElem(heap, larger_child), GetElem(heap, idx)) > 0)
        {
            SwapElem(heap, idx, larger_child);
            idx = larger_child;
        }
        else
        {
            return;
        }
    }
}

static size_t LargerChild(heap_t* heap, size_t parent_idx, size_t size)
{
    size_t left_child = LEFT(parent_idx);
    size_t right_child = RIGHT(parent_idx);
    size_t right_is_bigger_valid = 0;

    right_is_bigger_valid = (right_child < size) && (heap->compare(GetElem(heap,right_child),GetElem(heap,left_child)) > 0);
    
    return right_is_bigger_valid ? right_child : left_child;
}