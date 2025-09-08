/**************************************
Exercise:   System Programming - Simple Shell
Date:       3/9/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system */
#include <string.h>     /* strcmp */
#include <unistd.h>     /* fork */
#include <sys/wait.h>   /* waitpid */

#define MAX_COMMAND_LENGTH 1024
#define EXIT_COMMAND "exit"

typedef enum
{
    EXEC_FORK,
    EXEC_SYSTEM
} exec_method_e;

static void ExecuteWithFork(const char* command);
static void ExecuteWithSystem(const char* command);

int main(int argc, char* argv[])
{
    char command_buffer[MAX_COMMAND_LENGTH] = {0};
    exec_method_e exec_method = EXEC_FORK;

    if (2 != argc || 
        (0 != strcmp("fork", argv[1]) && 0 != strcmp("system", argv[1])))
    {
        printf("Usage: %s <fork|system>\n", argv[0]);
        return 1;
    }

    exec_method = (0 == strcmp("fork", argv[1])) ? EXEC_FORK : EXEC_SYSTEM;
    
    printf("Simple Shell (using %s)\nType 'exit' to quit.\n\n", 
           (EXEC_FORK == exec_method) ? "fork" : "system");

    while (1)
    {
        printf("shell> ");

        if (NULL == fgets(command_buffer, MAX_COMMAND_LENGTH, stdin))
        {
            break;
        }

        command_buffer[strcspn(command_buffer, "\n")] = '\0';

        if (0 == strcmp(EXIT_COMMAND, command_buffer))
        {
            break;
        }

        if (0 == strlen(command_buffer))
        {
            continue;
        }

        if (EXEC_FORK == exec_method)
        {
            ExecuteWithFork(command_buffer);
        }
        else
        {
            ExecuteWithSystem(command_buffer);
        }
    }

    printf("Shell exiting...\n");
    return 0;
}

static void ExecuteWithFork(const char* command)
{
    pid_t child_pid = 0;
    int status = 0;

    child_pid = fork();
    
    if (-1 == child_pid)
    {
        printf("fork failed");
        return;
    }

    if (0 == child_pid)
    {
        execl("/bin/sh", "sh", "-c", command, NULL); 
        printf("execl failed");
        exit(1);
    }

    waitpid(child_pid, &status, 0);
}

static void ExecuteWithSystem(const char* command)
{
    system(command);
}