/**************************************
Exercise: 	Prog - Scheduler
Date:		23/07/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		Approved
**************************************/

#ifndef ILRD_SCHED_H
#define ILRD_SCHED_H

#include <stddef.h> /* size_t 		*/

#include "uid.h"	/* UIDCreate 	*/


typedef enum
{
    SUCCESS = 0,
    PAUSED = 1,
    FAILED_TASKS_EMPTY_SCHED = 2,
    FAILED_TASKS_NON_EMPTY_SCHED = 3,
    FAILED_ALLOC = 4
} run_status_e;


typedef struct sched sched_t;

/*********************************************************
*  Brief:    Create a task scheduler that queues and runs
*            tasks according to their scheduled execution time.
*
*  Return:   Pointer to a new scheduler on success, NULL on failure.
*
*  Complexity: O(1).
*********************************************************/
sched_t* SchedCreate(void);

/*********************************************************
*  Brief:    Destroy the scheduler: clears all remaining tasks
*            and releases the scheduler's internal resources.
*
*  Param:    sch - Scheduler created by SchedCreate(). Undefined
*                   behavior if sch is NULL.
*
*  Complexity: O(1).
*********************************************************/
void SchedDestroy(sched_t* sch);

/*********************************************************
*  Brief:    Get the number of tasks currently stored in sch.
*
*  Param:    sch - Scheduler created by SchedCreate(). Undefined
*                   behavior if sch is NULL.
*
*  Return:   Count of tasks in the scheduler.
*
*  Complexity: O(n).
*********************************************************/
size_t SchedSize(const sched_t* sch);

/*********************************************************
*  Brief:    Check whether sch holds no tasks.
*
*  Param:    sch - Scheduler created by SchedCreate(). Undefined
*                   behavior if sch is NULL.
*
*  Return:   1 if empty, 0 otherwise.
*
*  Complexity: O(1).
*********************************************************/
int SchedIsEmpty(const sched_t* sch);

/*********************************************************
*  Brief:    Add a task to sch to be executed exe_time seconds
*            from the moment of insertion.
*
*  Param:    op_func       - Task function. Return -1 on failure,
*                            0 on success (no reschedule), or the
*                            number of seconds until the next run.
*                            Undefined behavior if op_func is NULL.
*            op_param      - Parameter passed to op_func.
*            exe_time      - Initial delay (in seconds) from now.
*            cleanup_func  - Cleanup routine for freeing resources
*                            used by the task. Undefined behavior
*                            if cleanup_func is NULL.
*            cleanup_param - Parameter passed to cleanup_func.
*
*  Return:   UID of the inserted task, UIDbadUID on failure.
*
*  Complexity: O(n).
*********************************************************/
ilrd_uid_t SchedAdd(sched_t* sch,
                    ssize_t (*op_func)(void* param),
                    void* op_param,
                    size_t exe_time,
                    void (*cleanup_func)(void* param),
                    void* cleanup_param);

/*********************************************************
*  Brief:    Remove a specific task identified by uid.
*
*  Param:    sch - Scheduler created by SchedCreate(). Undefined
*                   behavior if sch is NULL.
*            uid - UID returned by SchedAdd().
*
*  Return:   0 on success (task removed), 1 if no such task exists.
*
*  Complexity: O(n).
*********************************************************/
int SchedRemove(sched_t* sch, ilrd_uid_t uid);

/*********************************************************
*  Brief:    Remove all tasks from sch. After this call, sch is empty.
*
*  Param:    sch - Scheduler created by SchedCreate(). Undefined
*                   behavior if sch is NULL.
*
*  Complexity: O(n).
*********************************************************/
void SchedClear(sched_t* sch);

/*********************************************************
*  Brief:    Run tasks in sch until it becomes empty or until
*            SchedStop() is invoked.
*
*  Param:    sch - Scheduler created by SchedCreate(). Undefined
*                   behavior if sch is NULL.
*
*  Return:   One of the values of enum run_status_e.
*
*  Complexity: O(n) 
*********************************************************/
run_status_e SchedRun(sched_t* sch);

/*********************************************************
*  Brief:    Request SchedRun() to stop. May be called from
*            within a task's op_func.
*
*  Param:    sch - Scheduler created by SchedCreate(). Undefined
*                   behavior if sch is NULL.
*
*  Complexity: O(1).
*********************************************************/
void SchedStop(sched_t* sch);

#endif /* ILRD_SCHED_H */




