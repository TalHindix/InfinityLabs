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
#include <sys/wait.h>   /* wait */
#include <signal.h>     /* sigaction */
#include <time.h>       /* time */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset */
#include <assert.h>     /* assert */

#define GAME_DURATION_SEC (15)
#define PING_SIGNAL (SIGUSR1)
#define PONG_SIGNAL (SIGUSR2)
#define READY_SIGNAL (SIGCONT)
#define WAIT_TIMEOUT_SEC (5)
#define RESPONSE_TIMEOUT_SEC (2)

volatile sig_atomic_t g_signal_received = 0;
volatile sig_atomic_t g_child_ready = 0;

static void signal_handler(int sig, siginfo_t* info, void* context);

int main(void)
{
	struct sigaction sig_action = {0};
	pid_t child_pid = 0;
	time_t start_time = 0;
	char pid_str[32] = {0};
	sigset_t wait_mask = {0};
	sigset_t old_mask = {0};
	struct timespec timeout = {0};
	int sig_result = 0;
	
	/* Set up signal handlers */
	memset(&sig_action, 0, sizeof(sig_action));
	sig_action.sa_sigaction = signal_handler;
	sig_action.sa_flags = SA_SIGINFO;
	
	if (-1 == sigaction(PONG_SIGNAL, &sig_action, NULL))
	{
		printf("Parent: sigaction failed for PONG signal");
		return 1;
	}
	
	if (-1 == sigaction(READY_SIGNAL, &sig_action, NULL))
	{
		printf("Parent: sigaction failed for READY signal");
		return 1;
	}
	
	/* Block signals during setup */
	sigemptyset(&wait_mask);
	sigaddset(&wait_mask, PONG_SIGNAL);
	sigaddset(&wait_mask, READY_SIGNAL);
	sigprocmask(SIG_BLOCK, &wait_mask, &old_mask);

	child_pid = fork();
	if (-1 == child_pid)
	{
		printf("Parent: fork failed");
		sigprocmask(SIG_SETMASK, &old_mask, NULL);
		return 1;
	}

	/* Child process - exec the pong program */
	if (0 == child_pid)
	{
		sprintf(pid_str, "%d", getppid());
		printf("Child: About to exec pong program with parent PID: %s\n", pid_str);
		
		/* Restore signal mask before exec */
		sigprocmask(SIG_SETMASK, &old_mask, NULL);
		
		execl("./pong_process", "pong_process", pid_str, NULL);
		
		/* If we reach here, exec failed */
		printf("Child: exec failed");
		exit(1);
	}
	else /* Parent process */
	{
		printf("Parent: Started, child PID: %d\n", child_pid);
		printf("Parent: My PID: %d\n", getpid());
		
		printf("Parent: Waiting for child to be ready...\n");
		
		/* Wait for READY signal from child with timeout */
		timeout.tv_sec = WAIT_TIMEOUT_SEC;
		timeout.tv_nsec = 0;
		
		sigprocmask(SIG_UNBLOCK, &wait_mask, NULL);
		
		/* Wait for READY signal with timeout */
		while (0 == g_child_ready)
		{
			nanosleep(&timeout, NULL);
			if (0 == g_child_ready)
			{
				printf("Parent: Timed out waiting for child signal\n");
				kill(child_pid, SIGTERM);
				wait(NULL);
				return 1;
			}
		}
		
		/* Block signals again for controlled processing */
		sigprocmask(SIG_BLOCK, &wait_mask, NULL);
		
		start_time = time(NULL);
		printf("Parent: Child is ready, starting ping-pong game\n");
		
		printf("Parent: Sending first PING (%d)\n", PING_SIGNAL);
		kill(child_pid, PING_SIGNAL);
		
		while ((time(NULL) - start_time) < GAME_DURATION_SEC)
		{
			/* Set timeout for waiting */
			timeout.tv_sec = RESPONSE_TIMEOUT_SEC;
			timeout.tv_nsec = 0;
			
			/* Wait for PONG signal with timeout */
			sigprocmask(SIG_UNBLOCK, &wait_mask, NULL);
			sig_result = sigtimedwait(&wait_mask, NULL, &timeout);
			sigprocmask(SIG_BLOCK, &wait_mask, NULL);
			
			if (PONG_SIGNAL == sig_result)
			{
				printf("Parent: Received PONG (%d)\n", PONG_SIGNAL);
				printf("Parent: Sending PING (%d)\n", PING_SIGNAL);
				kill(child_pid, PING_SIGNAL);
			}
		}
		
		printf("Parent: Game time finished, terminating child\n");
		kill(child_pid, SIGTERM);
		wait(NULL);
		
		/* Restore original signal mask */
		sigprocmask(SIG_SETMASK, &old_mask, NULL);
		
		printf("Parent: Game completed\n");
	}

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
	else if (READY_SIGNAL == sig)
	{
		g_child_ready = 1;
	}
}