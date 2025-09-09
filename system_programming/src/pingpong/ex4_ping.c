/**************************************
Exercise: 	System Programming - Ping Pong Ex 4
Date:		07/09/2025
Developer:	Tal Hindi
Reviewer: 	Tamar Eisenstein
Status:		Approved
**************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>  /* printf */
#include <signal.h> /* sigaction */
#include <unistd.h> /* kill */
#include <stdlib.h> /* atoi */

static volatile sig_atomic_t signal_received = 0;
static volatile sig_atomic_t partner_pid = 0;

static void HandleSignal(int sig, siginfo_t* info, void* context);

int main(int argc, char **argv)
{
    struct sigaction sa = {0};
    
    sa.sa_sigaction = HandleSignal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);

    if (argc == 2) 
    {
        partner_pid = (sig_atomic_t)atoi(argv[1]);
    }

    printf("PING started - PID: %d (partner: %d)\n", 
           (int)getpid(), (int)partner_pid);

    while (1)
    {
        pause();
        
        if (signal_received)
        {
            signal_received = 0;
            printf("Got ping from %d, responding with pong\n", (int)partner_pid);
            
            if (partner_pid > 0) 
            {
                kill(partner_pid, SIGUSR2);
            }
        }
    }
    
    return 0;
}

static void HandleSignal(int sig, siginfo_t* info, void* context)
{
    (void)sig; 
    (void)context;
    
    if (info && !partner_pid) 
    {
        partner_pid = info->si_pid;
    }
    signal_received = 1;
}