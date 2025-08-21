/**************************************
Exercise:   DS - Hash Table
Date:       20/8/2025
Developer:  Tal Hindi
Reviewer:   Baruchi Haimson
Status:     Approved
**************************************/

#include <stdio.h>   /* printf */
#include <string.h>  /* strlen */
#include <stdlib.h>  /* malloc */

#include "hash_table.h" /* HashTableCreate */

#define DJB2_HASH_CONST   5381
#define BUFFER_SIZE       256
#define DICT_CAPACITY     262147
#define DICT_PATH         "/usr/share/dict/words"

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


static int FreeString(void* data, void* param);
static hash_table_t* LoadLinuxDictionary(const char* path);
static void TestLinuxDictionary(void);
static char* StrDup(const char* s);
static size_t StrHash(const void* key);
static int IsStrMatch(const void* data1,const void* data2);

static void TestRunSpellChecker(void);

int main(void)
{
    TestCreateDestroy();
    TestInsertRemove();
    TestFind();
    TestSizeIsEmpty();
    TestForEach();
    TestLinuxDictionary();
    TestRunSpellChecker();

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

    printf("\n[ForEach] sum=%d expected=%d %s\n", sum,expected,(sum == expected)?"OK":"FAIL");

    HashTableDestroy(hash);
}

static void TestLinuxDictionary(void)
{
    hash_table_t* dict = NULL;
    const char* probe = "algorithm";
    void* found = NULL;
    printf("\n");

    dict = LoadLinuxDictionary(NULL);
    if (NULL == dict)
    {
        printf("[Dict] failed to load dictionary\n");
        return;
    }

    printf("[Dict] size=%lu capacity=%lu load=%.3f\n",
       (unsigned long)HashTableSize(dict),
       (unsigned long)DICT_CAPACITY,
       (double)HashTableSize(dict) / (double)DICT_CAPACITY);

    found = HashTableFind(dict, probe);
    printf("[Dict] find(\"%s\"): %s\n", probe, (NULL != found) ? "YES" : "NO");

    (void)HashTableForEach(dict, FreeString, NULL);
    HashTableDestroy(dict);
}

static hash_table_t* LoadLinuxDictionary(const char* path)
{
    const char* path_to_use = DICT_PATH;
    hash_table_t* table = NULL;
    FILE* fp = NULL;
    char line[BUFFER_SIZE];
    size_t inserted = 0;

    if (NULL != path)
    {
        path_to_use = path;
    }

    table = HashTableCreate(StrHash, DICT_CAPACITY, IsStrMatch);
    if (NULL == table)
    {
        return NULL;
    }

    fp = fopen(path_to_use, "r");
    if (NULL == fp)
    {
        HashTableDestroy(table);
        return NULL;
    }

    while (NULL != fgets(line, sizeof(line), fp))
    {
        size_t len = 0;
        char* word = NULL;

        line[sizeof(line) - 1] = '\0';

        len = strlen(line);
        if ((len > 0) && ('\n' == line[len - 1]))
        {
            line[--len] = '\0';
        }
        if ((len > 0) && ('\r' == line[len - 1]))
        {
            line[--len] = '\0';
        }

        if (0 == len)
        {
            continue;
        }

        word = StrDup(line);
        if (NULL == word)
        {
            break;
        }

        if (0 != HashTableInsert(table, word))
        {
            free(word);
        }
        else
        {
            inserted += 1;
        }
    }

    fclose(fp);
    printf("[Dict] loaded %lu words\n", (unsigned long)inserted);
    return table;
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

static size_t StrHash(const void* key)
{
    const unsigned char* s = (const unsigned char*)key;
    size_t h = DJB2_HASH_CONST;
    while ('\0' != *s)
    {
        h = ((h << 5) + h) ^ (size_t)(*s);
        ++s;
    }
    return h;
}

static int IsStrMatch(const void* data1,const void* data2)
{
	return (0 == strcmp((const char*)data1, (const char*)data2));
}

static int FreeString(void* data, void* param)
{
	(void)param;

	free(data);

	return 0;
}

static char* StrDup(const char* s)
{
    size_t n = 0;
    char* p = NULL;

    if (NULL == s)
    {
        return NULL;
    }

    n = strlen(s);
    p = (char*)malloc(n + 1);
    if (NULL == p)
    {
        return NULL;
    }

    memcpy(p, s, n + 1);
    return p;
}

static void TestRunSpellChecker(void)
{
    hash_table_t* dict = NULL;
    char buf[BUFFER_SIZE];
    size_t len = 0;
    void* found = NULL;
    int keep_running = 1;

    printf("\n[Spell] loading dictionary...\n");
    dict = LoadLinuxDictionary(NULL);
    if (NULL == dict)
    {
        printf("[Spell] failed to load dictionary\n");
        return;
    }

    printf("[Spell] ready. type a word (or 'quit'):\n");

    while (keep_running)
    {
        printf("> ");
        fflush(stdout);

        if (NULL == fgets(buf, sizeof(buf), stdin))
        {
            keep_running = 0; /* EOF / read error */
            continue;
        }

        buf[sizeof(buf) - 1] = '\0';

        len = strlen(buf);
        if ((len > 0) && ('\n' == buf[len - 1]))
        {
            buf[--len] = '\0';
        }
        if ((len > 0) && ('\r' == buf[len - 1]))
        {
            buf[--len] = '\0';
        }

        if (0 == len)
        {
            keep_running = 0; 
            continue;
        }

        if (0 == strcmp(buf, "quit"))
        {
            keep_running = 0; 
            continue;
        }

        found = HashTableFind(dict, buf);
        if (NULL != found)
        {
            printf("%s: OK\n", buf);
        }
        else
        {
            printf("%s: MISSPELLING\n", buf);
        }
    }

    HashTableForEach(dict, FreeString, NULL);
    HashTableDestroy(dict);
}
