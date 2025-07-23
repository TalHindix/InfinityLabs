/**************************************
Exercise: 	DS - SCHED
Date:		21/7/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <assert.h> /* assert.h 	*/
#include <unistd.h> /* sleep 		*/
#include "sched.h" 	/* SchedCreate 	*/


struct sched
{
    pq_t* pq;
    int stop_flag;
};

static int TaskToCmp(const void* data1, const void* data2)
{
	return TaskCmp((const task_t*)data1, (const task_t*)data2);
}

static int TaskToMatch(const void* data, const void* param)
{
	return TaskIsMatch((task_t*)data, *(ilrd_uid_t*)param);
}

sched_t* SchedCreate(void)
{
	sched_t* sched = NULL;
	
	sched = (sched_t*)malloc(sizeof(sched_t*));
	if(!sched)
	{
		return NULL;
	}
	
	sched->pq = PQCreate(TaskToCmp);
	if(!sched->pq)
	{
		free(sched);
		return NULL;
	}
	
	return sched;
}

void SchedDestroy(sched_t* sch)
{
	assert(sch);
	
	PQDestroy(sch->pq);
	
	free(sch);
}

int SchedRun(sched_t* sch)
{
	ssize_t return_taskrun = 0;
	sch->stop_flag = 0;
	assert(sch);
	
	while(!SchedIsEmpty(sch) && !sch->stop_flag)
	{
		task_t* current_task = (task_t*)PQPeek(sch->pq);
		time_t task_time = TaskGetTimeToRun(current_task);
		time_t now = time(NULL);

		while (task_time > now)
		{
			sleep(task_time - now);
			now = time(NULL);
		}
		
		PQDequeue(sch->pq);
	
		return_taskrun = TaskRun(current_task);
		
		if(return_taskrun > 0)
		{
			TaskSetTimeToRun(current_task, return_taskrun);
			PQEnqueue(sch->pq, current_task);
			continue;
		}
		else
		{
			TaskDestroy(current_task);
		}
		
	}
	
	return 0;
}

void SchedStop(sched_t* sch)
{
	assert(sch);
	
	sch->stop_flag = 0;
}


                
ilrd_uid_t SchedAdd(sched_t* sch, ssize_t(*op_func)(void* param), void* param, size_t time_exe, void(*cleanup_func)(void* cleanup_param), void* cleanup_param)
{
	task_t* new_task = TaskCreate(op_func, param, time_exe, cleanup_func, cleanup_param);
	
	if (UIDIsSame(TaskUID(new_task), UIDbadUID))
	{
		return UIDbadUID;
	}
	
	PQEnqueue(sch->pq,new_task);
	
	return TaskUID(new_task);
}

void SchedRemove(sched_t* sch, ilrd_uid_t uid)
{
	task_t* task_to_remove = NULL;
	
	assert(sch);
	
	task_to_remove = (task_t*)PQErase(sch->pq, TaskToMatch, (const ilrd_uid_t*)&uid);
	
	TaskCleanUp(task_to_remove);
	
	free(task_to_remove);
	
}

int SchedIsEmpty(const sched_t* sch)
{
	assert(sch);
	
	return (PQIsEmpty(sch->pq));
}

void SchedClear(sched_t* sch)
{
	assert(sch);
	
	while(!PQIsEmpty(sch->pq))
	{
		PQDequeue(sch->pq);
	}
}

size_t SchedSize(const sched_t* sch)
{
	assert(sch);
	
	return PQSize(sch->pq);
}



