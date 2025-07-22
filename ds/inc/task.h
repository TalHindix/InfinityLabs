/**************************************
Exercise:  Project - Task
Date:      22/7/2025
Developer: Tal Hindi
Reviewer:  
Status:    
**************************************/

#ifndef ILRD_TASK_H
#define ILRD_TASK_H

#include <stddef.h> /* size_t */

#include "uid.h"

typedef struct task task_t;

/**
 * @brief Create a new task .
 *
 * @param op_func 
 * @param op_param 
 * @param interval_in_sec 
 * @param cleanup_func 
 * @param cleanup_param 
 * @return task_t* 
 */
task_t* TaskCreate(ssize_t (*op_func)(void *param),  
                   void *op_param,
                   size_t interval_in_sec,
                   void (*cleanup_func)(void* cleanup_param),
                   void* cleanup_param);

/**
 * @brief 
 *
 * @param task 
 */
void TaskDestroy(task_t* task);

/**
 * @brief 
 *
 * @param task 
 * @return ilrd_uid_t 
 */
ilrd_uid_t TaskUID(const task_t* task);

/**
 * @brief 
 *
 * @param task 
 * @return int 
 */
int TaskRun(task_t* task);

/**
 * @brief 
 *
 * @param task 
 */
void TaskCleanUp(task_t* task);

/**
 * @brief 
 *
 * @param task 
 * @param interval_in_sec 
 */
void TaskSetTimeToRun(task_t* task, size_t interval_in_sec);

/**
 * @brief 
 *
 * @param task 
 * @return size_t 
 */
size_t TaskGetTimeToRun(const task_t* task);

/**
 * @brief 
 *
 * @param task1 
 * @param task2 
 * @return int 
 */
int TaskIsMatch(const task_t* task1, const task_t* task2);

/**
 * @brief 
 *
 * @param task1 
 * @param task2 
 * @return int 
 */
int TaskCmp(const task_t* task1, const task_t* task2);

#endif /* ILRD_TASK_H */




