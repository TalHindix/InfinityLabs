/**************************************
Exercise:   Project - Task
Date:       22/07/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     Approved
**************************************/

#ifndef ILRD_TASK_H
#define ILRD_TASK_H

#include <stddef.h>     /* size_t  		*/
#include <sys/types.h>  /* ssize_t 		*/
#include "uid.h"        /* UIDCreate 	*/

typedef struct task task_t;

/**
 * @brief Allocate and initialize a task that will run after a given interval.
 *
 * @param op_func         Function to execute. See return convention above.
 * @param op_param        Argument forwarded to op_func.
 * @param interval_in_sec Initial delay (in seconds) from "now" until first run.
 * @param cleanup_func    Function used to release resources allocated by op_func.
 * @param cleanup_param   Argument forwarded to cleanup_func.
 *
 * @return Pointer to the created task on success, NULL on allocation failure.
 *
 * @note Complexity: O(1)
 */
task_t* TaskCreate(ssize_t (*op_func)(void* param),
                   void* op_param,
                   size_t interval_in_sec,
                   void (*cleanup_func)(void* param),
                   void* cleanup_param);

/**
 * @brief Destroy a task: call its cleanup_func and free its memory.
 *
 * @param task Task returned by TaskCreate().
 *
 * @note Complexity: O(1)
 */
void TaskDestroy(task_t* task);

/**
 * @brief Retrieve the unique ID of a task.
 *
 * @param task Task returned by TaskCreate().
 *
 * @return Task UID.
 *
 * @note Complexity: O(1)
 */
ilrd_uid_t TaskUID(const task_t* task);

/**
 * @brief Execute the task's op_func.
 *
 * @param task Task returned by TaskCreate().
 *
 * @return The value returned by op_func (see convention above).
 *
 * @note Complexity: O(1)
 */
ssize_t TaskRun(task_t* task);

/**
 * @brief Invoke the task's cleanup_func to free resources allocated by op_func.
 *
 * @param task Task returned by TaskCreate().
 *
 * @note Complexity: O(1)
 */
void TaskCleanUp(task_t* task);

/**
 * @brief Update the next time this task should be executed.
 *
 * @param task            Task returned by TaskCreate().
 * @param interval_in_sec Seconds from now until the next run.
 *
 * @note Complexity: O(1)
 */
void TaskSetTimeToRun(task_t* task, size_t interval_in_sec);

/**
 * @brief Get the number of seconds remaining until the task should run.
 *
 * @param task Task returned by TaskCreate().
 *
 * @return Seconds until execution time.
 *
 * @note Complexity: O(1)
 */
size_t TaskGetTimeToRun(const task_t* task);

/**
 * @brief Check if a task matches a given UID.
 *
 * @param task Task returned by TaskCreate().
 * @param uid  UID to compare with.
 *
 * @return 1 if the task's UID equals uid, otherwise 0.
 *
 * @note Complexity: O(1)
 */
int TaskIsMatch(const task_t* task, ilrd_uid_t uid);

/**
 * @brief Compare two tasks by their scheduled execution times.
 *
 * @param task1 First task.
 * @param task2 Second task.
 *
 * @return Positive if task1 is scheduled before task2, 0 if equal,
 *         negative if task2 is scheduled before task1.
 *
 * @note Complexity: O(1)
 */
int TaskCmp(const task_t* task1, const task_t* task2);

#endif /* ILRD_TASK_H */

