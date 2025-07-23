/**************************************
Exercise: 	Prog - Scheduler
Date:		23/07/2025
Developer:	Tal Hindi
Reviewer:	Avi Tobar 	
Status:		
**************************************/

#include <stdio.h>

#include "sched.h"

#define CHECK(cond, msg)                                      \
    do {                                                      \
        if (cond) {                                           \
            ++g_pass;                                         \
            printf("PASS: %s\n", msg);                        \
        } else {                                              \
            ++g_fail;                                         \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);    \
        }                                                     \
    } while (0)
    
static unsigned g_pass = 0, g_fail = 0;

/*
static void TestSchedRemove(void);
static void TestSchedAdd(void);
*/
static void TestCreateDestroy(void);
/*
static ssize_t OpFunc(void *param);
void CleanupFunc(void* param);
ssize_t OpFuncNULL(void* param);
void CleanupFuncNULL(void* param);

*/
int main()
{
	printf("=== Scheduler Tests ===\n");
    TestCreateDestroy();
    /*
	TestSchedAdd();
	TestSchedRemove();
	*/
    printf("\nSummary: %u passed, %u failed\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
	
	return 0;
}

static void TestCreateDestroy(void)
{
    sched_t* sched = SchedCreate();
    CHECK(sched != NULL, "SchedCreate returns non-NULL");

    CHECK(SchedIsEmpty(sched) == 1, "SchedIsEmpty returns 1 after creation");

    SchedDestroy(sched);
    CHECK(1, "SchedDestroy executed successfully");
}

/*
static void TestSchedAdd(void)
{
	ilrd_uid_t uid = {0};
    sched_t* sched = SchedCreate();
    CHECK(sched != NULL, "SchedCreate for Add");

    uid = SchedAdd(sched, DummyTask, NULL, 1, NULL, NULL);
    CHECK(!UIDIsSame(uid, UIDbadUID), "SchedAdd returns valid UID");
    CHECK(SchedIsEmpty(sched) == 0, "SchedIsEmpty after Add");

    SchedDestroy(sched);
}

static void TestSchedRemove(void)
{
	int remove_result = -1;
    sched_t* sched = SchedCreate();
    ilrd_uid_t uid = UIDbadUID;
    CHECK(sched != NULL, "SchedCreate for Remove");

    uid = SchedAdd(sched, DummyTask, NULL, 1, NULL, NULL);
    CHECK(!UIDIsSame(uid, UIDbadUID), "SchedAdd returns valid UID");

    remove_result = SchedRemove(sched, uid);
    CHECK(remove_result == 0, "SchedRemove returns 0 (success)");

    CHECK(SchedIsEmpty(sched) == 1, "SchedIsEmpty returns 1 after Remove");

    SchedDestroy(sched);
}


static ssize_t OpFunc(void *param)
{

}

static void CleanupFunc(void* param)
{
	
}

static ssize_t OpFuncNULL(void* param)
{
	(void)param;
	return 0;
}
static void CleanupFuncNULL(void* param)
{
	(void)param;
}
*/



