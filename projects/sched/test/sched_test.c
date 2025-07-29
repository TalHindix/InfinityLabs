/**************************************
Exercise: 	Prog - Scheduler
Date:		23/07/2025
Developer:	Tal Hindi
Reviewer:	Avi Tobar 	
Status:		
**************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

#include "sched.h"
#include "uid.h"

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RED   "\033[1;31m"

#define CHECK(cond, msg)                                      \
    do {                                                      \
        if (cond) {                                           \
            ++g_pass;                                         \
            printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", msg); \
        } else {                                              \
            ++g_fail;                                         \
            printf(COLOR_RED "FAIL" COLOR_RESET ": %s (line %d)\n", msg, __LINE__); \
        }                                                     \
    } while (0)

static unsigned g_pass = 0, g_fail = 0;

static void Test_CreateDestroy_EmptyRun(void);
static void Test_Add_Remove_Clear_Size_Empty(void);
static void Test_Run_NoFail_AllDone(void);
static void Test_Run_StopMid_NoFail(void);
static void Test_Run_Fail_EmptyAfter(void);
static void Test_Run_Fail_Leftover(void);
static void Test_Reschedule_SeveralTimes(void);
static void Test_SameTimeTasks_Order(void);
static void Test_Remove_Edges(void);
static void Test_Cleanup_Called(void);

static ssize_t PrintNTimes(void *param);
static ssize_t StopSchedOp(void *param);
static ssize_t FailOp(void *param);
static ssize_t SuccessOp(void *param);
static ssize_t ReschedOp(void *param);
static ssize_t TouchCounterOp(void *param);
static void DummyClean(void *param);
static void CountClean(void *param);
static void PrintSummary(void);

typedef struct
{
    const char *msg;
    size_t times_left;
} print_param_t;

typedef struct
{
    int *heap_num;
    int malloced;
} cleanup_param_t;

typedef struct
{
    size_t *counter;
    size_t repeats;
    ssize_t interval;
} resched_param_t;

int main(void)
{
    printf("=== Scheduler Tests ===\n\n");

    Test_CreateDestroy_EmptyRun();
    Test_Add_Remove_Clear_Size_Empty();
    Test_Run_NoFail_AllDone();
    Test_Run_StopMid_NoFail();
    Test_Run_Fail_EmptyAfter();
    Test_Run_Fail_Leftover();
    Test_Reschedule_SeveralTimes();
    Test_SameTimeTasks_Order();
    Test_Remove_Edges();
    Test_Cleanup_Called();

    PrintSummary();
    return g_fail ? 1 : 0;
}

static void Test_CreateDestroy_EmptyRun(void)
{
    sched_t *sch = SchedCreate();
    CHECK(sch != NULL, "SchedCreate returns pointer");
    CHECK(SchedIsEmpty(sch) == 1, "SchedIsEmpty == 1 on new scheduler");
    CHECK(SchedSize(sch) == 0, "SchedSize == 0 on new scheduler");

    {
        run_status_e ret = SchedRun(sch);
        CHECK(ret == SUCCESS, "SchedRun on empty returns SUCCESS");
    }

    SchedDestroy(sch);
    CHECK(1, "SchedDestroy executed");
}

static void Test_Add_Remove_Clear_Size_Empty(void)
{
    sched_t *sch = SchedCreate();
    ilrd_uid_t u1, u2, u3;
    ilrd_uid_t bad = UIDbadUID;

    u1 = SchedAdd(sch, SuccessOp, NULL, 1, DummyClean, NULL);
    u2 = SchedAdd(sch, SuccessOp, NULL, 2, DummyClean, NULL);
    u3 = SchedAdd(sch, SuccessOp, NULL, 3, DummyClean, NULL);

    CHECK(!UIDIsSame(u1, bad) && !UIDIsSame(u2, bad) && !UIDIsSame(u3, bad), "SchedAdd returned valid UIDs");
    CHECK(SchedSize(sch) == 3, "SchedSize == 3 after adds");
    CHECK(SchedIsEmpty(sch) == 0, "SchedIsEmpty == 0 after adds");

    CHECK(SchedRemove(sch, u2) == 0, "SchedRemove existing middle returns 0");
    CHECK(SchedSize(sch) == 2, "SchedSize == 2 after middle remove");
    CHECK(SchedRemove(sch, u2) == 1, "SchedRemove non-existing returns 1");

    SchedClear(sch);
    CHECK(SchedSize(sch) == 0, "SchedSize == 0 after clear");
    CHECK(SchedIsEmpty(sch) == 1, "SchedIsEmpty == 1 after clear");

    SchedDestroy(sch);
}

static void Test_Run_NoFail_AllDone(void)
{
    sched_t *sch = SchedCreate();
    print_param_t p = {"OK\n", 2};
    time_t start = time(NULL);
    time_t end;
    run_status_e ret;
    double expected = 0;
    double dur = 0;

    SchedAdd(sch, PrintNTimes, &p, 0, DummyClean, NULL);
    SchedAdd(sch, SuccessOp, NULL, 1, DummyClean, NULL); 

    ret = SchedRun(sch);
    end = time(NULL);

    CHECK(ret == SUCCESS, "Run_NoFail_AllDone -> SUCCESS");

    expected = 1;

    dur = difftime(end, start);
    CHECK(fabs(dur - expected) <= 1, "runtime within ±1.0 sec of expected");

    SchedDestroy(sch);
}

static void Test_Run_StopMid_NoFail(void)
{
    sched_t *sch = SchedCreate();
    print_param_t p = {"Hello\n", 5};
    run_status_e ret;

    SchedAdd(sch, PrintNTimes, &p, 0, DummyClean, NULL);
    SchedAdd(sch, StopSchedOp, sch, 1, DummyClean, NULL);
    SchedAdd(sch, SuccessOp, NULL, 2, DummyClean, NULL);

    ret = SchedRun(sch);
    CHECK(ret == PAUSED, "Run_StopMid -> PAUSED");
    CHECK(SchedIsEmpty(sch) == 0, "Queue not empty after stop");

    ret = SchedRun(sch);
    CHECK(ret == SUCCESS, "Second run clears remaining tasks -> SUCCESS");
    CHECK(SchedIsEmpty(sch) == 1, "Queue empty after second run");

    SchedDestroy(sch);
}

static void Test_Run_Fail_EmptyAfter(void)
{
    sched_t *sch = SchedCreate();
    run_status_e ret;

    SchedAdd(sch, FailOp, NULL, 0, DummyClean, NULL);

    ret = SchedRun(sch);
    CHECK(ret == FAILED_TASKS_EMPTY_SCHED, "Fail + empty => FAILED_TASKS_EMPTY_SCHED");

    SchedDestroy(sch);
}

static void Test_Run_Fail_Leftover(void)
{
    sched_t *sch = SchedCreate();
    run_status_e ret;

    SchedAdd(sch, FailOp, NULL, 0, DummyClean, NULL);
    SchedAdd(sch, SuccessOp, NULL, 10, DummyClean, NULL);
    SchedAdd(sch, StopSchedOp, sch, 0, DummyClean, NULL);

    ret = SchedRun(sch);
    CHECK(ret == FAILED_TASKS_NON_EMPTY_SCHED, "Fail + leftover => FAILED_TASKS_NON_EMPTY_SCHED");
    CHECK(SchedIsEmpty(sch) == 0, "Queue not empty after run");

    SchedDestroy(sch);
}

static void Test_Reschedule_SeveralTimes(void)
{
    sched_t *sch = SchedCreate();
    size_t counter = 0;
    run_status_e ret;
    resched_param_t prm;

	prm.counter = &counter;
	prm.repeats = 3;
	prm.interval = 1;

    SchedAdd(sch, ReschedOp, &prm, 0, DummyClean, NULL);
    ret = SchedRun(sch);

    CHECK(ret == SUCCESS, "RescheduleSeveralTimes => SUCCESS");
    CHECK(counter == 3, "Reschedule ran exactly 3 times");

    SchedDestroy(sch);
}

static void Test_SameTimeTasks_Order(void)
{
    sched_t *sch = SchedCreate();
    size_t c1 = 0, c2 = 0, c3 = 0;
    run_status_e ret;

    SchedAdd(sch, TouchCounterOp, &c1, 0, DummyClean, NULL);
    SchedAdd(sch, TouchCounterOp, &c2, 0, DummyClean, NULL);
    SchedAdd(sch, TouchCounterOp, &c3, 0, DummyClean, NULL);

    ret = SchedRun(sch);

    CHECK(ret == SUCCESS, "SameTimeTasks => SUCCESS");
    CHECK(c1 == 1 && c2 == 1 && c3 == 1, "All same-time tasks executed exactly once");

    SchedDestroy(sch);
}

static void Test_Remove_Edges(void)
{
    sched_t *sch = SchedCreate();
    ilrd_uid_t u_first, u_mid, u_last, dummy;
    ilrd_uid_t bad = UIDbadUID;

    u_first = SchedAdd(sch, SuccessOp, NULL, 1, DummyClean, NULL);
    u_mid   = SchedAdd(sch, SuccessOp, NULL, 2, DummyClean, NULL);
    dummy   = SchedAdd(sch, SuccessOp, NULL, 3, DummyClean, NULL);
    u_last  = SchedAdd(sch, SuccessOp, NULL, 4, DummyClean, NULL);
    (void)dummy;

    CHECK(SchedSize(sch) == 4, "Size 4 before removes");
    CHECK(SchedRemove(sch, u_first) == 0, "Remove first ok");
    CHECK(SchedRemove(sch, u_last)  == 0, "Remove last ok");
    CHECK(SchedRemove(sch, u_mid)   == 0, "Remove mid ok");
    CHECK(SchedRemove(sch, u_mid)   == 1, "Remove mid second time -> 1");
    CHECK(UIDIsSame(bad, u_first) == 0, "UIDs are not bad");

    CHECK(SchedSize(sch) == 1, "One item left");
    SchedClear(sch);
    CHECK(SchedIsEmpty(sch) == 1, "Empty after clear");

    SchedDestroy(sch);
}

static void Test_Cleanup_Called(void)
{
    sched_t *sch = SchedCreate();
    cleanup_param_t prm;
    memset(&prm, 0, sizeof(prm));

    SchedAdd(sch, SuccessOp, NULL, 0, CountClean, &prm);
    (void)SchedRun(sch);

    CHECK(prm.malloced == 0, "Cleanup freed allocated memory");
    CHECK(prm.heap_num == NULL, "Pointer nullified");

    SchedDestroy(sch);
}

static ssize_t PrintNTimes(void *param)
{
    print_param_t *p = (print_param_t *)param;
    printf("%s", p->msg);
    if (p->times_left > 1)
    {
        --p->times_left;
        return 1;
    }
    return 0;
}

static ssize_t StopSchedOp(void *param)
{
    SchedStop((sched_t *)param);
    return 0;
}

static ssize_t FailOp(void *param)
{
    (void)param;
    return -1;
}

static ssize_t SuccessOp(void *param)
{
    (void)param;
    return 0;
}

static ssize_t ReschedOp(void *param)
{
    resched_param_t *p = (resched_param_t *)param;
    ++*(p->counter);
    if (*(p->counter) < p->repeats)
    {
        return p->interval;
    }
    return 0;
}

static ssize_t TouchCounterOp(void *param)
{
    size_t *cnt = (size_t *)param;
    ++(*cnt);
    return 0;
}

static void DummyClean(void *param)
{
    (void)param;
}

static void CountClean(void *param)
{
    cleanup_param_t *p = (cleanup_param_t *)param;
    if (!p->malloced)
    {
        p->heap_num = (int *)malloc(sizeof(int));
        if (p->heap_num)
        {
            *(p->heap_num) = 42;
            p->malloced = 1;
        }
    }
    if (p->heap_num)
    {
        free(p->heap_num);
        p->heap_num = NULL;
        p->malloced = 0;
    }
}


static void PrintSummary(void)
{
    printf("\nSummary: %u passed, %u failed\n", g_pass, g_fail);
}



