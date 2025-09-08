/**************************************
Exercise: 	System Programming - Ping Pong Ex 4
Date:		07/09/2025
Developer:	Tal Hindi
Reviewer: 	Tamar Eisenstein
Status:		In Progress
**************************************/

#define _POSIX_C_SOURCE (200809L)

#include <stdio.h>      /* printf */
#include <unistd.h>     /* pause */
#include <signal.h>     /* sigaction */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset */
#include <time.h>       /* time */

#define PING_SIGNAL (SIGUSR1)
#define PONG_SIGNAL (SIGUSR2)
#define RESPONSE_DELAY_SEC (1)

volatile sig_atomic_t g_signal_received = 0;
volatile sig_atomic_t g_pong_pid = 0;

static void signal_handler(int sig, siginfo_t* info, void* context);

int main(void)
{
	struct sigaction sig_action = {0};
	sigset_t wait_mask = {0};
	sigset_t old_mask = {0};
	struct timespec response_delay = {0};
	
	printf("Ping: Started with PID: %d\n", getpid());
	printf("Ping: Use this PID as argument for the pong process\n");
	printf("Ping: Waiting ..\n");
	printf("Ping: Press Enter to continue...\n");
	getchar(); 
	
	/* Set up signal handler */
	memset(&sig_action, 0, sizeof(sig_action));
	sig_action.sa_sigaction = signal_handler;
	sig_action.sa_flags = SA_SIGINFO;
	
	if (-1 == sigaction(PING_SIGNAL, &sig_action, NULL))
	{
		printf("Ping: sigaction failed");
		return 1;
	}
	
	/* Block signals during setup */
	sigemptyset(&wait_mask);
	sigaddset(&wait_mask, PING_SIGNAL);
	sigprocmask(SIG_BLOCK, &wait_mask, &old_mask);
	
	/* Set up controlled response delay */
	response_delay.tv_sec = RESPONSE_DELAY_SEC;
	response_delay.tv_nsec = 0;
	
	printf("Ping: Ready to receive signals from pong\n");
	printf("Ping: Waiting for first PING from pong...\n");
	
	/* Unblock signals to receive PING */
	sigprocmask(SIG_UNBLOCK, &wait_mask, NULL);
	
	while (1)
	{
		/* Wait for PING signal */
		pause();
		
		if (0 != g_signal_received)
		{
			/* Got a signal - make sure we have the sender PID */
			if (0 == g_pong_pid)
			{
				printf("Ping: Error - no sender PID received\n");
				break;
			}
			
			printf("Ping: Received PING (SIGUSR1) from pong (PID: %d)\n", g_pong_pid);
			g_signal_received = 0;
			
			/* Wait before responding */
			nanosleep(&response_delay, NULL);
			
			printf("Ping: Sending PONG (SIGUSR2) back to pong\n");
			if (-1 == kill(g_pong_pid, PONG_SIGNAL))
			{
				printf("Ping: Failed to send PONG signal");
				break;
			}
		}
		
		/* Check if pong still exists */
		if (0 != g_pong_pid && -1 == kill(g_pong_pid, 0))
		{
			printf("Ping: Pong process no longer exists, exiting\n");
			break;
		}
	}
	
	printf("Ping: Game terminated\n");
	
	return 0;
}

static void signal_handler(int sig, siginfo_t* info, void* context)
{
	(void)context;
	
	if (PING_SIGNAL == sig)
	{
		g_signal_received = 1;
		/* Store the sender's PID for our response */
		if (NULL != info)
		{
			g_pong_pid = info->si_pid;
		}
	}
}