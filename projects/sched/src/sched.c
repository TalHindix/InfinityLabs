/**************************************
Exercise: 	Prog - Scheduler
Date:		23/07/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		Approved
**************************************/

#include <assert.h> /* assert.h 	*/
#include <unistd.h> /* sleep 		*/
#include <stdlib.h> /* malloc		*/

#include "sched.h" 	/* SchedCreate 	*/
#include "task.h"	/* TaskCreate	*/
#include "heap.h"   /* HeapCreate   */

struct sched
{
    heap_t* heap;
    int stop_flag;
};

static run_status_e ComputeExitStatus(const sched_t *sch, int had_fail);
static void SleepUntil(time_t when);
static int WrapperTaskCmp(const void* task1, const void* task2);
static int WrapperTaskIsMatch(const void* data, const void* param);

sched_t* SchedCreate(void)
{
	sched_t* sched = NULL;
	
	sched = (sched_t*)malloc(sizeof(sched_t));
	if(!sched)
	{
		return NULL;
	}
	
	sched->heap = HeapCreate(WrapperTaskCmp);
	if(!sched->heap)
	{
		free(sched);
		return NULL;
	}
	
	sched->stop_flag = 0;
	
	return sched;
}

void SchedDestroy(sched_t* sch)
{
	assert(sch);
	
	SchedClear(sch);
	
	HeapDestroy(sch->heap);
	free(sch);
}

run_status_e SchedRun(sched_t *sch)
{
    task_t *task = NULL;
    ssize_t result = 0;
    int had_fail = 0;

    assert(sch);
    sch->stop_flag = 0;

    while (!sch->stop_flag && !HeapIsEmpty(sch->heap))
    {
        task = (task_t *)HeapPeek(sch->heap);
		HeapPop(sch->heap);

        SleepUntil((time_t)TaskGetTimeToRun(task));

        result = TaskRun(task);

        if (result < 0)
        {
            had_fail = 1;
            TaskDestroy(task);
            continue;
        }

        if (0 == result)
        {
            TaskDestroy(task);
            continue;
        }

        TaskSetTimeToRun(task, result);
        if (HeapPush(sch->heap, task))
        {
            TaskDestroy(task);
            return FAILED_ALLOC;
        }
    }

    return ComputeExitStatus(sch, had_fail);
}

void SchedStop(sched_t* sch)
{
	assert(sch);
	
	sch->stop_flag = 1;
}
       
ilrd_uid_t SchedAdd(sched_t* sch, ssize_t(*op_func)(void* param), void* param, size_t time_exe, void(*cleanup_func)(void* cleanup_param), void* cleanup_param)
{
	task_t* new_task = NULL;
	
	assert(sch);
	assert(op_func);
	assert(cleanup_func);
	
	new_task = TaskCreate(op_func, param, time_exe, cleanup_func, cleanup_param);	
	
	if (!new_task)
	{
		return UIDbadUID;
	}
	
	if (HeapPush(sch->heap,new_task))
	{
		TaskDestroy(new_task);
		return UIDbadUID;
	}
	
	return TaskUID(new_task);
}

int SchedRemove(sched_t* sch, ilrd_uid_t uid)
{
	task_t* task_to_remove = NULL;
	
	assert(sch);
	
	task_to_remove = HeapRemove(sch->heap, &uid, WrapperTaskIsMatch);
	
	if (NULL == task_to_remove)
	{
		return 1;
	}
	
	TaskDestroy(task_to_remove);
	return 0;
}

int SchedIsEmpty(const sched_t* sch)
{
	assert(sch);
	
	return (HeapIsEmpty(sch->heap));
}

void SchedClear(sched_t* sch)
{
	task_t* task = NULL;
	
	assert(sch);
	
	while(!HeapIsEmpty(sch->heap))
	{
		task = HeapPeek(sch->heap);
		HeapPop(sch->heap);
		TaskDestroy(task);
	}
}

size_t SchedSize(const sched_t* sch)
{
	assert(sch);
	
	return HeapSize(sch->heap);
}

static int WrapperTaskCmp(const void* task1, const void* task2)
{
	return TaskCmp((const task_t*)task2, (const task_t*)task1);
}

static int WrapperTaskIsMatch(const void* data, const void* param)
{	
	return TaskIsMatch((const task_t*)data, *(const ilrd_uid_t*)param);
}

static void SleepUntil(time_t when)
{
    time_t now = time(NULL);
    while (now < when)
    {
        sleep(when - now);
        now = time(NULL);
    }
}

static run_status_e ComputeExitStatus(const sched_t *sch, int had_fail)
{
    if (had_fail)
    {
        return HeapIsEmpty(sch->heap) ? FAILED_TASKS_EMPTY_SCHED
                                  : FAILED_TASKS_NON_EMPTY_SCHED;
    }

    return HeapIsEmpty(sch->heap) ? SUCCESS : PAUSED;
}



