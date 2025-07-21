/**************************************
Exercise: 	DS - SCHED
Date:		21/7/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include "sched.h" /* SchedCreate */

sched_t* SchedCreate(void)
{
	return NULL;
}

void SchedDestroy(sched_t* sch)
{
	(void)sch;
	return;
}

int SchedRun(sched_t* sch)
{
	(void)sch;
	return 0;
}

int SchedStop(sched_t* sch)
{
	(void)sch;
	return 0;
}

uid_t* SchedAdd(sched_t* sch, int(op_func*)(void* param), void* param)
{
	(void)sch;
	(void)op_func;
	(void)param;
	
	return NULL;
}

void SchedRemove(sched_t* sch, uid_t uid)
{
	(void)sch;
	(void)uid;
	
	return;
}

int SchedIsEmpty(const sched_t* sch)
{
	(void)sch;
	return 0;
}

void SchedClear(sched_t* sch)
{
	(void)sch;
	return;
}

size_t SchedSize(const sched_t* sch)
{
	(void)sch;	
	return 5;
}

#endif /* ILRD_SCHED_H */


