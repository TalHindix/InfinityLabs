/**************************************
Exercise: 	DS - SCHED
Date:		21/7/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#ifndef ILRD_SCHED_H
#define ILRD_SCHED_H

typedef struct sched sched_t;

/* O(1) */
sched_t* SchedCreate(void);
/* O(n) */
void SchedDestroy(sched_t* sch);
/* O(1) */
int SchedRun(sched_t* sch);
/* O(1) */
int SchedStop(sched_t* sch);
/* O(n) */
uid_t* SchedAdd(void);
/* O(n) */
void SchedRemove(sched_t* sch, uid_t uid);
/* O(1) */
int SchedIsEmpty(const sched_t* sch);
/* O(n) */
void SchedClear(sched_t* sch);
/* O(n) */
size_t SchedSize(const sched_t* sch);

#endif /* ILRD_SCHED_H */




