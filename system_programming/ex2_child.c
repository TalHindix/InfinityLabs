/**************************************
Exercise: 	System Programming - Ping Pong Ex 2
Date:		07/09/2025
Developer:	Tal Hindi
Reviewer: 	Tamar Eisenstein
Status:		In Progress
**************************************/

#define _POSIX_C_SOURCE (200809L)

#include <stdio.h>      /* printf */
#include <unistd.h>     /* fork */
#include <sys/types.h>  /* pid_t */
#include <signal.h>     /* sigaction */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset */
#include <assert.h>     /* assert */

#define PING_SIGNAL (SIGUSR1)
#define PONG_SIGNAL (SIGUSR2)
#define READY_SIGNAL (SIGCONT)
#define RESPONSE_DELAY_SEC (1)
#define WAIT_TIMEOUT_SEC (10)

volatile sig_atomic_t g_signal_received = 0;
pid_t g_parent_pid = 0;

static void signal_handler(int sig, siginfo_t* info, void* context);

int main(int argc, char* argv[])
{
	struct sigaction sig_action = {0};
	sigset_t wait_mask = {0};
	sigset_t old_mask = {0};
	struct timespec response_delay = {0};
	struct timespec timeout = {0};
	int sig_result = 0;

	if (2 != argc)
	{
		printf("Pong: Usage: %s <parent_pid>\n", argv[0]);
		return 1;
	}

	g_parent_pid = atoi(argv[1]);
	if (0 >= g_parent_pid)
	{
		printf("Pong: Invalid parent PID\n");
		return 1;
	}

	printf("Pong: Started with PID: %d \n", getpid());
	printf("Pong: Parent PID: %d\n", g_parent_pid);

	/* Set up signal handler */
	memset(&sig_action, 0, sizeof(sig_action));
	sig_action.sa_sigaction = signal_handler;
	sig_action.sa_flags = SA_SIGINFO;

	/* Block signals during setup */
	sigemptyset(&wait_mask);
	sigaddset(&wait_mask, PING_SIGNAL);
	sigprocmask(SIG_BLOCK, &wait_mask, &old_mask);

	if (-1 == sigaction(PING_SIGNAL, &sig_action, NULL))
	{
		printf("Pong: sigaction failed");
		return 1;
	}

	/* Set up controlled response delay */
	response_delay.tv_sec = RESPONSE_DELAY_SEC;
	response_delay.tv_nsec = 0;

	printf("Pong: Ready to receive PING signals\n");
	
	/* Signal readiness to parent */
	printf("Pong: Sending READY signal to parent\n");
	kill(g_parent_pid, READY_SIGNAL);
	
	printf("Pong: Waiting for signals...\n");

	/* Unblock signals to receive PING */
	sigprocmask(SIG_UNBLOCK, &wait_mask, NULL);

	while (1)
	{
		/* Set timeout for waiting */
		timeout.tv_sec = WAIT_TIMEOUT_SEC;
		timeout.tv_nsec = 0;
		
		/* Wait for PING signal with timeout */
		sigprocmask(SIG_BLOCK, &wait_mask, NULL);
		sig_result = sigtimedwait(&wait_mask, NULL, &timeout);
		sigprocmask(SIG_UNBLOCK, &wait_mask, NULL);
		
		if (PING_SIGNAL == sig_result)
		{
			printf("Pong: Received PING (%d) from parent\n", PING_SIGNAL);
			
			nanosleep(&response_delay, NULL);
			
			printf("Pong: Sending PONG (%d) back to parent\n", PONG_SIGNAL);

			kill(g_parent_pid, PONG_SIGNAL);
		}
		else
		{
			/* No signal received within timeout, check if parent is still alive */
			if (-1 == kill(g_parent_pid, 0))
			{
				printf("Pong: Parent process no longer exists, exiting\n");
				break;
			}
		}
	}

	return 0;
}

static void signal_handler(int sig, siginfo_t* info, void* context)
{
	(void)info;
	(void)context;
	
	if (PING_SIGNAL == sig)
	{
		g_signal_received = 1;
	}
}