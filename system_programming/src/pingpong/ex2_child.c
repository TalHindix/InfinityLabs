/**************************************
Exercise: 	System Programming - Ping Pong Ex 2
Date:		07/09/2025
Developer:	Tal Hindi
Reviewer: 	Tamar Eisenstein
Status:		In Progress
**************************************/

#define _POSIX_C_SOURCE (200809L)

#include <stdio.h>      /* printf */
#include <signal.h>     /* sigaction */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* kill */
#include <stdlib.h>     /* atoi */

static volatile sig_atomic_t got_usr1 = 0;
static pid_t parent_pid = 0;

static void ChildHandler(int sig);
static void SetHandler(int signum, void (*handler)(int));

int main(int argc, char* argv[])
{
    if (2 != argc)
    {
        printf("Usage: %s <parent_pid>\n", argv[0]);
        return 1;
    }
    
    parent_pid = atoi(argv[1]);
    if (0 >= parent_pid)
    {
        printf("Invalid parent PID\n");
        return 1;
    }
    
    SetHandler(SIGUSR1, ChildHandler);
    printf("Child PID = %d, Parent PID = %d\n", getpid(), parent_pid);
    
    while (1)
    {
        pause();
        if (got_usr1)
        {
            got_usr1 = 0;
            printf("Child got Ping, Sending Pong\n");
            sleep(1);
            kill(parent_pid, SIGUSR2);
        }
    }

    return 0;
}

static void ChildHandler(int sig)
{
    (void)sig;
    got_usr1 = 1;
}

static void SetHandler(int signum, void (*handler)(int))
{
    struct sigaction sig_act;
    sig_act.sa_handler = handler;
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sigaction(signum, &sig_act, NULL);
}

