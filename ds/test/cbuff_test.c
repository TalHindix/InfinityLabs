/**************************************
Exercise: 	DS - Circuler Buffer
Date:		7/7/2025
Developer:	Tal Hindi
Reviewer: 	Baruch Haimson
Status:		
**************************************/

#include <stdio.h>
#include "cbuff.h"
#include <string.h>
#include <sys/types.h>
#include <assert.h>

static unsigned g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                     \
    do {                                                     \
        if (cond) {                                          \
            ++g_pass;                                        \
            printf("PASS: %s\n", msg);                       \
        } else {                                             \
            ++g_fail;                                        \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);   \
        }                                                    \
    } while (0)


static void TestCreateDestroy(void);
static void TestEmptyAndSize(void);
static void TestWriteReadBasic(void);
static void TestDefaultCapacity(void);

int main(void)
{
    printf("\n===! CBuffer Tests !===\n");

    TestCreateDestroy();
    TestEmptyAndSize();
    TestWriteReadBasic();
    TestDefaultCapacity();

    printf("\nSummary: %u passed, %u failed\n", g_pass, g_fail);
    
    return g_fail ? 1 : 0;
}


static void TestCreateDestroy(void)
{
    printf("-- TestCreateDestroy --");
    {
        cbuff_t *cb = CBuffCreate(16);
        CHECK(cb != NULL, "CBuffCreate returned non-NULL");
        CHECK(CBuffFreeSpace(cb) == 16, "Free space == capacity");
        CBuffDestroy(cb);
    }
}

static void TestEmptyAndSize(void)
{
    const char data[] = "abc";
    char buf[sizeof(data)] = {0};
    cbuff_t *cb = NULL;

    printf("-- TestEmptyAndSize --");
    cb = CBuffCreate(8);

    CHECK(CBuffIsEmpty(cb),          "Buffer is empty after create");
    CHECK(CBuffSize(cb) == 0,        "Size == 0 after create");

    CHECK(CBuffWrite(cb, data, sizeof(data)) == (ssize_t)sizeof(data),"Write succeeded");
    CHECK(!CBuffIsEmpty(cb),"Buffer not empty after write");
    CHECK(CBuffSize(cb) == sizeof(data),"Size updated after write");

    CHECK(CBuffRead(cb, buf, sizeof(buf)) == (ssize_t)sizeof(buf),"Read succeeded");
    CHECK(CBuffIsEmpty(cb),          "Buffer empty after read");
    CHECK(CBuffSize(cb) == 0,        "Size back to 0");

    CBuffDestroy(cb);
}

static void TestWriteReadBasic(void)
{
    const char in[]  = "Hello, world!";
    char out[sizeof(in)] = {0};
    cbuff_t *cb = NULL;

    printf("-- TestWriteReadBasic --");
    cb = CBuffCreate(sizeof(in));

    CHECK(CBuffWrite(cb, in, sizeof(in)) == (ssize_t)sizeof(in),"Wrote full message");
    CHECK(CBuffRead(cb, out, sizeof(out)) == (ssize_t)sizeof(out),"Read full message");
    CHECK(memcmp(in, out, sizeof(in)) == 0, "Data integrity");

    CBuffDestroy(cb);
}




static void TestDefaultCapacity(void)
{
    cbuff_t *cb;

    printf("-- TestDefaultCapacity --");
    cb = CBuffCreate(0);
    CHECK(cb != NULL,"Create with 0 capacity");
    CHECK(CBuffFreeSpace(cb) == DEFAULT_SIZE,"Got DEFAULT_SIZE free space");
    CBuffDestroy(cb);
}
