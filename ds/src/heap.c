/**************************************
Exercise:   DS - Heap
Date:       25/8/2025
Developer:  Tal Hindi
Reviewer:   Baruchi Haimson
Status:     
**************************************/

#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */

#include "heap.h" /* HeapCreate */

#define LEFT(i) ((size_t)((i) * 2 + 1))
#define RIGHT(i) ((size_t)((i) * 2 + 2))
#define PARENT(i) (((i) - 1) / 2)

#define DEF_CAPACITY 128

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
    heap->vector = VectorCreate(DEF_CAPACITY,sizeof(void*));
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
    assert(!HeapIsEmpty(heap));

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
    assert(!HeapIsEmpty(heap));

    last_ele = VectorSize(heap->vector) - 1;
    SwapElem(heap,(size_t)0,last_ele);
    VectorPopBack(heap->vector);

    if (!HeapIsEmpty(heap))
    {
        HeapifyDown(heap, 0);
    }

}

void* HeapRemove(heap_t* heap, const void* data, is_match_func is_match)
{
    size_t i = 0;
    size_t last = 0;
    void* removed = NULL;

    assert(heap);
    assert(is_match);

    if(HeapIsEmpty(heap))
    {
        return NULL;
    }

    for(i = 0; i < VectorSize(heap->vector); ++i)
    {
        
        if(0 == is_match(GetElem(heap,i),data))
        {
            last = VectorSize(heap->vector) - 1;

            removed = GetElem(heap, i);

            if (i == last)
            {
                VectorPopBack(heap->vector);
                return removed;
            }
            
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

            return removed;
        }
    }

    return NULL;
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

static void SetElem(heap_t* heap, size_t idx, void* data)
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
    size_t size = HeapSize(heap);
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
    size_t left_child_idx = LEFT(parent_idx);
    size_t right_child_idx = RIGHT(parent_idx);

    /* A node must have at least a left child to be a non-leaf node.
    If the left child is out of bounds, the parent doesn't have any children. */
    if (left_child_idx >= size)
    {
        return parent_idx; /* or some error indicator, as this shouldn't be called on a leaf */
    }

    /* If the right child is out of bounds, the left child is the only option. */
    if (right_child_idx >= size)
    {
        return left_child_idx;
    }

    /* Compare the two children to find the larger one. */
    if (heap->compare(GetElem(heap, right_child_idx), GetElem(heap, left_child_idx)) > 0)
    {
        return right_child_idx;
    }

    return left_child_idx;
}