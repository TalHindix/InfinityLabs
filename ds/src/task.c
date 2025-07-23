/**************************************
Exercise:  Project - Task
Date:      22/7/2025
Developer: Tal Hindi
Reviewer:  
Status:    
**************************************/

#include <stdlib.h>  	/* malloc 		*/
#include <assert.h>		/* assert 		*/
#include <sys/types.h> 	/* ssize_t 		*/

#include "task.h"		/* TaskCreate 	*/

struct task
{
    ilrd_uid_t uid; 
	time_t time_to_execute;    
	ssize_t (*op_func)(void *op_param);   
	void *op_param;                                 
	void (*cleanup_func)(void* clean_up_param);
	void* clean_up_param;    
};

task_t* TaskCreate(ssize_t (*op_func)(void *param),void *op_param,
                   size_t interval_in_sec,
                   void (*cleanup_func)(void *cleanup_param),void *cleanup_param)
{
	task_t* task = {0};
	
	task = (task_t*)malloc(sizeof(task_t));
	if(NULL == task)
	{
		return NULL;
	}
	
	task->uid = UIDCreate();
	if(UIDIsSame(task->uid, UIDbadUID))
	{
		free(task);
		return NULL;
	}
	
	task->op_func = op_func;
	task->op_param = op_param;
	task->time_to_execute = time(NULL) + interval_in_sec;
	task->cleanup_func = cleanup_func;
	task->clean_up_param = cleanup_param;  

    return task;
}

void TaskDestroy(task_t* task)
{
	assert(task);
	
	TaskCleanUp(task);
	
	task->uid = UIDbadUID;
    task->op_func = NULL;
    task->op_param = NULL;
    task->time_to_execute = 0;
    task->cleanup_func = NULL;
    task->clean_up_param = NULL;
	
	free(task);
	task = NULL;
}

ilrd_uid_t TaskUID(const task_t* task)
{
    assert(task);

    return task->uid;
}

ssize_t TaskRun(task_t* task)
{
	assert(task);
	assert(task->op_func);
	
    return task->op_func(task->op_param);
}

void TaskCleanUp(task_t* task)
{
   assert(task);
   assert(task->cleanup_func);
   
   task->cleanup_func(task->clean_up_param);
}

void TaskSetTimeToRun(task_t* task, size_t interval_in_sec)
{
    assert(task);
    
    task->time_to_execute = time(NULL) + interval_in_sec;
}

size_t TaskGetTimeToRun(const task_t* task)
{
    assert(task);
    
    return task->time_to_execute;
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

    return TaskGetTimeToRun(task1) - TaskGetTimeToRun(task2);
}
