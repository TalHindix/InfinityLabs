/**************************************
Exercise: 	System Programming - Ping Pong Ex 2
Date:		07/09/2025
Developer:	Tal Hindi
Reviewer: 	Tamar Eisenstein
Status:		Approved
**************************************/

#define _POSIX_C_SOURCE (200809L)

#include <stdio.h>      /* printf */
#include <signal.h>     /* sigaction */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork */

static volatile sig_atomic_t got_usr2 = 0;
static pid_t child_pid = 0;

static void ParentHandler(int sig);

static void SetHandler(int signum, void (*handler)(int))
{
    struct sigaction sig_act;
    sig_act.sa_handler = handler;
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sigaction(signum, &sig_act, NULL);
}

int main()
{
    child_pid = fork();
    if (child_pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (0 == child_pid)
    {
        char pid_str[16] = {0};
        sprintf(pid_str, "%d", getppid());
        execl("./ex2_child", "ex2_child", pid_str, (char*)NULL);
        perror("execl");
        return 1;
    }
    
    SetHandler(SIGUSR2, ParentHandler);
    printf("Parent PID = %d, Child PID = %d\n", getpid(), child_pid);

    /* Give child time to initialize */
    sleep(1);
    printf("Parent sending initial Ping\n");
    kill(child_pid, SIGUSR1);

    while (1)
    {
        pause();
        if (got_usr2)
        {
            got_usr2 = 0;
            printf("Parent got Pong, Sending Ping\n");
            sleep(1);
            kill(child_pid, SIGUSR1);
        }
    }

    return 0;
}


static void ParentHandler(int sig)
{
    (void)sig;
    got_usr2 = 1;
}
