/**************************************
Exercise:   DS - Hash Table
Date:       20/8/2025
Developer:  Tal Hindi
Reviewer:   Baruchi Haimson
Status:     
**************************************/

#include <stdio.h>   /* printf */

#include "hash_table.h" /* HashTableCreate */

static size_t IntHash(const void* data);
static int IsMatch(const void* d1,const void* d2);
static int PrintElement(void* data,void* unused);
static int SumElement(void* data,void* param);

void HashDebugPrint(hash_table_t* table,int (*print)(void*,void*));

static void TestCreateDestroy(void);
static void TestInsertRemove(void);
static void TestFind(void);
static void TestSizeIsEmpty(void);
static void TestForEach(void);

int main(void)
{
    TestCreateDestroy();
    TestInsertRemove();
    TestFind();
    TestSizeIsEmpty();
    TestForEach();

    return 0;
}


static void TestCreateDestroy(void)
{
    hash_table_t* hash = NULL;
    int x = 42;

    hash = HashTableCreate(IntHash,8,IsMatch);
    HashTableInsert(hash,&x);

    printf("\n[Create/Destroy]\n");
    HashDebugPrint(hash,PrintElement);

    HashTableDestroy(hash);
}

static void TestInsertRemove(void)
{
    hash_table_t* hash = NULL;
    int values[] = {8,3,10,1,6,14,4,7,13};
    size_t i = 0;
    size_t size = sizeof(values) / sizeof(values[0]);

    hash = HashTableCreate(IntHash,size,IsMatch);

    for (i = 0;i < size;++i)
    {
        HashTableInsert(hash,&values[i]);
    }

    printf("\n[Insert/Remove] after inserts:\n");
    HashDebugPrint(hash,PrintElement);

    for (i = 0;i < size;++i)
    {
        HashTableRemove(hash,&values[i]);
    }

    printf("\n[Insert/Remove] after removes:\n");
    HashDebugPrint(hash,PrintElement);

    HashTableDestroy(hash);
}

static void TestFind(void)
{
    hash_table_t* hash = NULL;
    int values[] = {5,11,17};
    int not_there = 99;
    void* found = NULL;

    hash = HashTableCreate(IntHash,4,IsMatch);
    HashTableInsert(hash,&values[0]);
    HashTableInsert(hash,&values[1]);

    found = HashTableFind(hash,&values[1]);
    printf("[Find] Looking for 11  %s\n",(NULL != found)?"FOUND":"NOT FOUND");

    found = HashTableFind(hash,&not_there);
    printf("[Find] Looking for 99  %s\n",(NULL != found)?"FOUND (ERROR!)":"NOT FOUND (OK)");

    HashTableDestroy(hash);
}

static void TestSizeIsEmpty(void)
{
    hash_table_t* hash = NULL;
    int a = 1;
    int b = 2;

    hash = HashTableCreate(IntHash,4,IsMatch);

    printf("\n[Size/Empty] empty=%d size=%lu\n",
           HashTableIsEmpty(hash),(unsigned long)HashTableSize(hash));

    HashTableInsert(hash,&a);
    HashTableInsert(hash,&b);

    printf("[Size/Empty] empty=%d size=%lu\n",
           HashTableIsEmpty(hash),(unsigned long)HashTableSize(hash));

    HashTableRemove(hash,&a);
    HashTableRemove(hash,&b);

    printf("[Size/Empty] empty=%d size=%lu\n",
           HashTableIsEmpty(hash),(unsigned long)HashTableSize(hash));

    HashTableDestroy(hash);
}

static void TestForEach(void)
{
    hash_table_t* hash = NULL;
    int values[] = {2,4,6};
    int sum = 0;
    int expected = 12;
    size_t i = 0;

    hash = HashTableCreate(IntHash,4,IsMatch);

    for (i = 0;i < sizeof(values)/sizeof(values[0]);++i)
    {
        HashTableInsert(hash,&values[i]);
    }

    HashTableForEach(hash,SumElement,&sum);

    printf("\n[ForEach] sum=%d expected=%d %s\n",
           sum,expected,(sum == expected)?"OK":"FAIL");

    HashTableDestroy(hash);
}


static size_t IntHash(const void* data)
{
    return (size_t)(*(const int*)data);
}

static int IsMatch(const void* d1,const void* d2)
{
    return (*(const int*)d1 == *(const int*)d2);
}

static int PrintElement(void* data,void* unused)
{
    (void)unused;
    printf("%d -> ",*(int*)data);
    return 0;
}

static int SumElement(void* data,void* param)
{
    *(int*)param += *(int*)data;
    return 0;
}
