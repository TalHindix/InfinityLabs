/**************************************
Exercise:  Project - Task
Date:      22/7/2025
Developer: Tal Hindi
Reviewer:  
Status:    
**************************************/

#include <stdio.h> 		/* printf */
#include <unistd.h>
#include <time.h>

#include "task.h"


ssize_t PrintMsg(void *param);
void CleanupMsg(void *param);

int main()
{
    task_t *task = NULL;
    ilrd_uid_t task_uid = {0};
    size_t time_to_run = 0;

    printf("Creating task...\n");
    task = TaskCreate(PrintMsg, "Starting Task", 2, CleanupMsg, "CleanUP Message Task");
    if (NULL == task)
    {
        printf("Task creation failed.\n");
        return 1;
    }

    task_uid = TaskUID(task);
    printf("Task created successfully.\n");

    time_to_run = TaskGetTimeToRun(task);
    printf("Initial Time to Run: %lu\n", time_to_run);

    sleep(2);

    printf("Running task now:\n");
    TaskRun(task);

    TaskSetTimeToRun(task, 2);
    printf("New Time to Run: %lu\n", TaskGetTimeToRun(task));

    printf("Checking UID match (should be 1): %d\n", TaskIsMatch(task, task_uid));

    printf("Destroying task...\n");
    TaskDestroy(task);

    printf("Done!\n");
    return 0;
}


ssize_t PrintMsg(void *param)
{
    printf("Running task: %s\n", (char *)param);
    return 0;
}

void CleanupMsg(void *param)
{
    printf("Cleaning up: %s\n", (char *)param);
}

