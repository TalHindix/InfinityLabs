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
#include <stdlib.h>     /* atoi */
#include <string.h>     /* memset */
#include <time.h>       /* nanosleep */

#define PING_SIGNAL (SIGUSR1)
#define PONG_SIGNAL (SIGUSR2)
#define RESPONSE_DELAY_SEC (1)

volatile sig_atomic_t g_signal_received = 0;
volatile sig_atomic_t g_ping_pid = 0;

static void signal_handler(int sig, siginfo_t* info, void* context);

int main(int argc, char* argv[])
{
	struct sigaction sig_action = {0};
	sigset_t wait_mask = {0};
	sigset_t old_mask = {0};
	struct timespec response_delay = {0};
	
	/* Check command line arguments */
	if (2 != argc)
	{
		printf("Pong: Usage: %s <ping_pid>\n", argv[0]);
		return 1;
	}
	
	g_ping_pid = atoi(argv[1]);
	if (0 >= g_ping_pid)
	{
		printf("Pong: Invalid ping PID\n");
		return 1;
	}
	
	printf("Pong: Started with PID: %d\n", getpid());
	printf("Pong: Will communicate with ping process (PID: %d)\n", g_ping_pid);
	printf("Pong: Waiting ...\n");
	printf("Pong: Press Enter to continue...\n");
	getchar(); /* Wait for user to press Enter */
	
	/* Set up signal handler */
	memset(&sig_action, 0, sizeof(sig_action));
	sig_action.sa_sigaction = signal_handler;
	sig_action.sa_flags = SA_SIGINFO;
	
	if (-1 == sigaction(PONG_SIGNAL, &sig_action, NULL))
	{
		printf("Pong: sigaction failed");
		return 1;
	}
	
	/* Block signals during setup */
	sigemptyset(&wait_mask);
	sigaddset(&wait_mask, PONG_SIGNAL);
	sigprocmask(SIG_BLOCK, &wait_mask, &old_mask);
	
	/* Set up controlled response delay */
	response_delay.tv_sec = RESPONSE_DELAY_SEC;
	response_delay.tv_nsec = 0;
	
	/* Check if ping process exists */
	if (-1 == kill(g_ping_pid, 0))
	{
		printf("Pong: Ping process does not exist");
		return 1;
	}
	
	/* Send first PING to start the game */
	printf("Pong: Sending first PING (SIGUSR1) to ping\n");
	if (-1 == kill(g_ping_pid, PING_SIGNAL))
	{
		printf("Pong: Failed to send initial PING signal");
		return 1;
	}
	
	/* Unblock signals to receive PONG responses */
	sigprocmask(SIG_UNBLOCK, &wait_mask, NULL);
	
	while (1)
	{
		/* Wait for PONG signal */
		pause();
		
		if (0 != g_signal_received)
		{
			printf("Pong: Received PONG (SIGUSR2) from ping\n");
			g_signal_received = 0;
			
			/* Wait briefly before responding */
			nanosleep(&response_delay, NULL);
			
			printf("Pong: Sending PING (SIGUSR1) to ping\n");
			if (-1 == kill(g_ping_pid, PING_SIGNAL))
			{
				printf("Pong: Failed to send PING signal");
				break;
			}
		}
		
		/* Check if ping still exists */
		if (-1 == kill(g_ping_pid, 0))
		{
			printf("Pong: Ping process no longer exists, exiting\n");
			break;
		}
	}
	
	printf("Pong: Game terminated\n");
	
	return 0;
}

static void signal_handler(int sig, siginfo_t* info, void* context)
{
	(void)info;
	(void)context;
	
	if (PONG_SIGNAL == sig)
	{
		g_signal_received = 1;
	}
}