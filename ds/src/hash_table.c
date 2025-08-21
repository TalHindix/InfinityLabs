/**************************************
Exercise:   DS - Hash Table
Date:       20/8/2025
Developer:  Tal Hindi
Reviewer:   Baruchi Haimson
Status:     
**************************************/

#include <stdlib.h>     /* malloc */
#include <assert.h>     /* assert */
#include <stdio.h>      /* For some testings */

#include "hash_table.h" /* HashTableCreate */

#define CAPACITY(x) ((x)->capacity)

struct hash_table
{
    size_t          capacity;
    hash_func_t     func;
    is_match_func_t is_match;
    sll_t**         buckets;
};

static void       HashFreeBucketsUpTo(sll_t** buckets, size_t count);
static size_t     HashIndex(const hash_table_t* table, const void* key);
static sll_iter_t HashFindInBucket(const hash_table_t* table, sll_t* bucket, const void* key);
static int        HashForEachBucket(sll_t* bucket, action_func_t action_func, void* param);

hash_table_t* HashTableCreate(hash_func_t hash_func, size_t size, is_match_func_t is_match)
{
    hash_table_t* table = NULL;
    size_t i = 0;

    assert(hash_func);
    assert(is_match);

    if (0 == size)
    {
        return NULL;
    }

    table = (hash_table_t*)malloc(sizeof(hash_table_t));
    if (NULL == table)
    {
        return NULL;
    }

    table->buckets = (sll_t**)calloc(size, sizeof(sll_t*));
    if (NULL == table->buckets)
    {
        free(table);
        return NULL;
    }

    table->capacity = size;
    table->func = hash_func;
    table->is_match = is_match;

    for (i = 0; i < size; ++i)
    {
        table->buckets[i] = SLLCreate();
        if (NULL == table->buckets[i])
        {
            HashFreeBucketsUpTo(table->buckets, i);
            free(table->buckets);
            free(table);
            return NULL;
        }
    }

    return table;
}

void HashTableDestroy(hash_table_t* table)
{
    size_t i = 0;

    assert(table);

    if (NULL != table->buckets)
    {
        for (i = 0; i < table->capacity; ++i)
        {
            if (NULL != table->buckets[i])
            {
                SLLDestroy(table->buckets[i]);
                table->buckets[i] = NULL;
            }
        }

        free(table->buckets);
        table->buckets = NULL;
    }

    free(table);
}

int HashTableInsert(hash_table_t* table, void* data)
{
    size_t bucket_index = 0;
    sll_t* bucket = NULL;
    sll_iter_t where_iter = NULL;
    sll_iter_t found_iter = NULL;
    sll_iter_t new_iter = NULL;

    assert(table);
    assert(data);

    bucket_index = HashIndex(table, data);
    bucket = table->buckets[bucket_index];
    assert(bucket);

    found_iter = HashFindInBucket(table, bucket, data);
    assert(SLLIsEqual(found_iter, SLLEnd(bucket)));
    

    where_iter = SLLBegin(bucket);
    new_iter = SLLInsert(where_iter, data);
    if (SLLIsEqual(new_iter, SLLEnd(bucket)))
    {
        return 1;
    }

    return 0;
}

void* HashTableFind(const hash_table_t* table, const void* key)
{
    size_t bucket_index = 0;
    sll_t* bucket = NULL;
    sll_iter_t iter = NULL;

    assert(table);
    assert(key);

    bucket_index = HashIndex(table, key);
    bucket = table->buckets[bucket_index];
    assert(bucket);

    iter = HashFindInBucket(table, bucket, key);
    if (!SLLIsEqual(iter, SLLEnd(bucket)))
    {
        return SLLGetData(iter);
    }

    return NULL;
}

void HashTableRemove(hash_table_t* table, const void* key)
{
    size_t bucket_index = 0;
    sll_t* bucket = NULL;
    sll_iter_t iter = NULL;

    assert(table);
    assert(key);

    bucket_index = HashIndex(table, key);
    bucket = table->buckets[bucket_index];
    assert(bucket);

    iter = HashFindInBucket(table, bucket, key);
    if (!SLLIsEqual(iter, SLLEnd(bucket)))
    {
        SLLRemove(iter);
    }
}

size_t HashTableSize(const hash_table_t* table)
{
    size_t i = 0;
    size_t total = 0;

    assert(table);

    for (i = 0; i < table->capacity; ++i)
    {
        if (NULL != table->buckets[i])
        {
            total += SLLCount(table->buckets[i]);
        }
    }

    return total;
}

int HashTableIsEmpty(const hash_table_t* table)
{
    size_t i = 0;

    assert(table);

    for (i = 0; i < table->capacity; ++i)
    {
        if ((NULL != table->buckets[i]) && (0 == SLLIsEmpty(table->buckets[i])))
        {
            return 0;
        }
    }

    return 1;
}

int HashTableForEach(hash_table_t* table, action_func_t action_func, void* param)
{
    size_t i = 0;
    sll_t* bucket = NULL;

    assert(table);
    assert(action_func);

    for (i = 0; i < table->capacity; ++i)
    {
        bucket = table->buckets[i];
        assert(bucket);

        if (0 != HashForEachBucket(bucket, action_func, param))
        {
            return 1;
        }
    }

    return 0;
}

static void HashFreeBucketsUpTo(sll_t** buckets, size_t count)
{
    size_t i = 0;

    if (NULL == buckets)
    {
        return;
    }

    for (i = 0; i < count; ++i)
    {
        if (NULL != buckets[i])
        {
            SLLDestroy(buckets[i]);
            buckets[i] = NULL;
        }
    }
}

static size_t HashIndex(const hash_table_t* table, const void* key)
{
    size_t hash_code = 0;
    size_t index = 0;

    assert(table);
    assert(key);

    hash_code = table->func(key);
    index = hash_code % table->capacity;

    return index;
}

static sll_iter_t HashFindInBucket(const hash_table_t* table, sll_t* bucket, const void* key)
{

    assert(table);
    assert(bucket);
    assert(key);

    return SLLFind(SLLBegin(bucket), SLLEnd(bucket), table->is_match, (void*)key);
}

static int HashForEachBucket(sll_t* bucket, action_func_t action_func, void* param)
{
    
    assert(bucket);
    assert(action_func);

    return SLLForEach(SLLBegin(bucket), SLLEnd(bucket), action_func, param);
}

/*  HELPER FUNCTION FOR TESTING TO GET ACCESS TO 
    THE STRUCT OF TABLE AND SAVE THE ENCAPSULATION */
void HashDebugPrint(hash_table_t* table, int (*print)(void*, void*))
{
    size_t i = 0;
    sll_t* bucket = NULL;

    if ((NULL == table) || (NULL == print))
    {
        return;
    }

    for (i = 0; i < table->capacity; ++i)
    {
        bucket = table->buckets[i];
        printf("[%lu]: ", (unsigned long)i);
        SLLForEach(SLLBegin(bucket), SLLEnd(bucket), print, NULL);
        printf("NULL\n");
    }
}