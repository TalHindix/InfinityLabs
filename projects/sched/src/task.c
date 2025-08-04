/**************************************
Exercise:  Project - Task
Date:      22/7/2025
Developer: Tal Hindi
Reviewer:  Avi Tobar
Status:    Approved
**************************************/

#include <stdlib.h>  	/* malloc 		*/
#include <assert.h>		/* assert 		*/

#include "task.h"		/* TaskCreate 	*/

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

struct task
{
    ilrd_uid_t uid;
	time_t time_to_execute;
	ssize_t (*op_func)(void* op_param);   
	void *op_param;
	void (*cleanup_func)(void* cleanup_param);
	void* cleanup_param;
};


task_t* TaskCreate(ssize_t (*op_func)(void* param), void* op_param, size_t interval_in_sec, void (*cleanup_func)(void* param),void* cleanup_param)
{
	task_t* task = NULL;
	
	assert(op_func);
	assert(cleanup_func);
	
	task = (task_t*)malloc(sizeof(task_t));
	if (!task)
	{
		return NULL;
	}
	
	task->uid = UIDCreate();
	if (UIDIsSame(task->uid, UIDbadUID))
	{
		free(task);
		return NULL;
	}

	task->time_to_execute = time(NULL) + (time_t)interval_in_sec;
	task->op_func = op_func;
	task->op_param = op_param;
	task->cleanup_func = cleanup_func;
	task->cleanup_param = cleanup_param;
	
	return task;

}

void TaskDestroy(task_t* task)
{
	assert(task);
	
	TaskCleanUp(task);
		
	free(task);
}

ilrd_uid_t TaskUID(const task_t* task)
{
    assert(task);

    return task->uid;
}

ssize_t TaskRun(task_t* task)
{
	assert(task);
		
    return task->op_func(task->op_param);
}

void TaskCleanUp(task_t* task)
{
   assert(task);

   task->cleanup_func(task->cleanup_param);
}

void TaskSetTimeToRun(task_t* task, size_t interval_in_sec)
{
    assert(task);
    
    task->time_to_execute = time(NULL) + (time_t)interval_in_sec;
}

size_t TaskGetTimeToRun(const task_t* task)
{
	time_t interval = 0;
	
    assert(task);
    
    interval = task->time_to_execute - time(NULL);
    
    return MAX(interval,0);
}

int TaskIsMatch(const task_t* task, ilrd_uid_t uid)
{
	assert(task);
	
    return UIDIsSame(task->uid,uid);
}

int TaskCmp(const task_t* task1, const task_t* task2)
{
	assert(task1);
	assert(task2);

    return (int)(TaskGetTimeToRun(task1) - TaskGetTimeToRun(task2));
}
