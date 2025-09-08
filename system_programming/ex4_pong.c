/**************************************
Exercise: 	System Programming - Ping Pong Ex 4
Date:		07/09/2025
Developer:	Tal Hindi
Reviewer: 	Tamar Eisenstein
Status:		In Progress
**************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>	/* printf */
#include <stdlib.h> /* atoi */
#include <signal.h> /* sigaction*/
#include <unistd.h> /* kill */   

static volatile sig_atomic_t response_received = 0;
static pid_t target_pid = 0;

static void HandleResponse(int sig);

int main(int argc, char *argv[])
{
    struct sigaction sa = {0};
    
    if (argc != 2) 
    { 
        fprintf(stderr, "Usage: %s <PING_PID>\n", argv[0]); 
        return 1; 
    }
    target_pid = (pid_t)atoi(argv[1]);

    sa.sa_handler = HandleResponse;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR2, &sa, NULL);

    printf("PONG process started - PID: %d, targeting ping: %d\n", 
           (int)getpid(), (int)target_pid);
    
    printf("Initiating ping-pong with SIGUSR1\n");
    kill(target_pid, SIGUSR1);

    while (1)
    {
        pause(); 
        if (response_received)
        {
            response_received = 0;
            printf("Received pong, sending ping back\n");
            kill(target_pid, SIGUSR1);
        }
    }
}

static void HandleResponse(int sig)
{ 
    (void)sig; 
    response_received = 1; 
} 