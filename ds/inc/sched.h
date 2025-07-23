/**************************************
Exercise: 	DS - SCHED
Date:		21/7/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#ifndef ILRD_SCHED_H
#define ILRD_SCHED_H

#include "uid.h"
#include "task.h"
#include "pq.h"

#include <stdlib.h>

typedef struct sched sched_t;

/* O(1) */
sched_t* SchedCreate(void);
/* O(n) */
void SchedDestroy(sched_t* sch);
/* O(1) */
int SchedRun(sched_t* sch);
/* O(1) */
void SchedStop(sched_t* sch);
/* O(n) */
ilrd_uid_t SchedAdd(sched_t* sch, ssize_t(*op_func)(void* param), void* param, size_t time_exe, void(*cleanup_func)(void* cleanup_param), void* cleanup_param);
/* O(n) */
void SchedRemove(sched_t* sch, ilrd_uid_t uid);
/* O(1) */
int SchedIsEmpty(const sched_t* sch);
/* O(n) */
void SchedClear(sched_t* sch);
/* O(n) */
size_t SchedSize(const sched_t* sch);

#endif /* ILRD_SCHED_H */




