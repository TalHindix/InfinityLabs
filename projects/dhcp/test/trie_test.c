/**************************************
Exercise: 	DS - TRIE
Date:	    27/8/2025
Developer:	Tal Hindi
Reviewer: 	Daniel Gamliel
Status:		
**************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc */

#include "trie.h"   /* TRIECreate */

#define PASS "\033[32mPASS\033[0m"
#define FAIL "\033[31mFAIL\033[0m"

static void TestTrieCreate(void);
static void TestBasicInsert(void);
static void TestSearch(void);
static void TestFreeAndReuse(void);
static void TestCountFree(void);
static void PrintTestResult(const char* test_name, int passed);

int main(void)
{
    printf("=== TRIE Test  ===\n\n");
    
    TestTrieCreate();
    TestBasicInsert();
    TestSearch();
    TestFreeAndReuse();
    TestCountFree();
    
    printf("\n=== Test Complete ===\n");
    return 0;
}

static void TestTrieCreate(void)
{
    trie_node_t* trie = NULL;
    size_t free_count = 0;

    printf("Test 1: Trie Creation\n");
    
    trie = TRIECreate();
    
    PrintTestResult("TRIECreate returns valid pointer", NULL != trie);
    
    if (NULL != trie) 
    {
        free_count = TRIECountFree(trie, 8);
        PrintTestResult("Initial free count > 0", 0 < free_count);
        TRIEDestroy(trie);
    }
    
    printf("\n");
}

static void TestBasicInsert(void)
{
    trie_node_t* trie = NULL;
    unsigned int allocated = 0;
    trie_status_e result = TRIE_SUCCESS;
    
    printf("Test 2: Basic Insert\n");
    
    trie = TRIECreate();
    
    result = TRIEInsert(trie, &allocated, 42, 8);
    
    PrintTestResult("Insert value 42", TRIE_SUCCESS == result);
    PrintTestResult("Allocated matches requested", 42 == allocated);
    
    TRIEDestroy(trie);
    printf("\n");
}

static void TestSearch(void)
{
    trie_node_t* trie = NULL;
    unsigned int allocated = 0;
    
    printf("Test 3: Search Functionality\n");
    
    trie = TRIECreate();
    
    PrintTestResult("Search non-existent value", 0 == TRIESearch(trie, 42, 8));
    
    TRIEInsert(trie, &allocated, 42, 8);
    
    PrintTestResult("Search existing value", 1 == TRIESearch(trie, 42, 8));
    PrintTestResult("Search different value", 0 == TRIESearch(trie, 43, 8));
    
    TRIEDestroy(trie);
    printf("\n");
}

static void TestFreeAndReuse(void)
{
    trie_node_t* trie = NULL;
    unsigned int allocated = 0;
    trie_status_e result = TRIE_SUCCESS;
    
    printf("Test 4: Free and Reuse\n");
    
    trie = TRIECreate();
    
    TRIEInsert(trie, &allocated, 42, 8);
    
    result = TRIEFree(trie, 42, 8);
    PrintTestResult("Free allocated value", TRIE_SUCCESS == result);
    
    PrintTestResult("Value no longer found after free", 0 == TRIESearch(trie, 42, 8));
    
    result = TRIEInsert(trie, &allocated, 42, 8);
    PrintTestResult("Can reuse freed value", result == TRIE_SUCCESS && 42 == allocated);
    
    result = TRIEFree(trie, 42, 8);
    result = TRIEFree(trie, 42, 8);
    PrintTestResult("Double free detected", TRIE_DOUBLE_FREE == result);
    
    TRIEDestroy(trie);
    printf("\n");
}

static void TestCountFree(void)
{
    trie_node_t* trie = NULL;
    unsigned int allocated = 0;
    size_t initial = 0;
    size_t after_insert = 0;
    size_t after_free = 0;
    
    printf("Test 5: Count Free Values\n");
    
    trie = TRIECreate();
    
    initial = TRIECountFree(trie, 4);
    printf("Initial free count (depth 4): %lu\n", (unsigned long)initial);
    
    TRIEInsert(trie, &allocated, 5, 4);
    after_insert = TRIECountFree(trie, 4);
    PrintTestResult("Free count decreased after insert", after_insert == initial - 1);
    
    TRIEFree(trie, 5, 4);
    after_free = TRIECountFree(trie, 4);
    PrintTestResult("Free count increased after free", after_free == initial);
    
    TRIEDestroy(trie);
    printf("\n");
}

static void PrintTestResult(const char* test_name, int passed)
{
    printf("  %s: %s\n", test_name, passed ? PASS : FAIL);
}