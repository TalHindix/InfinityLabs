/**************************************
Exercise: 	System Programming - Ping Pong Ex1
Date:		07/09/2025
Developer:	Tal Hindi
Reviewer: 	Tamar Eisenstein
Status:		Approved
**************************************/

#define _POSIX_C_SOURCE (200809L)


#include <stdio.h>      /* printf */
#include <unistd.h>     /* fork */
#include <sys/types.h>  /* pid_t */
#include <sys/wait.h>   /* wait */
#include <signal.h>     /* sigaction */
#include <time.h>       /* time */

volatile sig_atomic_t signal_received = 0;

static void SignalHandler(int sig, siginfo_t* info, void* context);

int main()
{
	struct sigaction sig_action = {0};
	pid_t pid = 0;
	time_t start_time = 0;
    int keep_running = 1;

	sig_action.sa_sigaction = SignalHandler;
	sig_action.sa_flags = SA_SIGINFO;

	pid = fork();
	if (-1 == pid)
	{
		printf("fork failed\n");
		return 1;
	}
    /* This is the child process */
	if (0 == pid)
	{
		
		if (-1 == sigaction(SIGUSR1, &sig_action, NULL))
		{
			printf("sigaction failed\n");
			return 1;
		}

		printf("Child process started\n");

		while (keep_running)
		{
			pause();
			if (signal_received)
			{
				printf("Child: Got PING, sending PONG back\n");
				signal_received = 0;
				kill(getppid(), SIGUSR2);
				sleep(1);
			}
		}
	}
	else /* This is the parent process */
	{
		
		if (-1 == sigaction(SIGUSR2, &sig_action, NULL))
		{
			printf("sigaction failed\n");
			return 1;
		}

		printf("Parent process started, child PID: %d\n", pid);
		sleep(1);
		start_time = time(NULL);

		printf("Parent: Sending first PING\n");
		kill(pid, SIGUSR1);

		while ((time(NULL) - start_time) < 8)
		{
			pause();
			if (signal_received)
			{
				printf("Parent: Got PONG, sending PING\n");
				signal_received = 0;
				sleep(1);
				kill(pid, SIGUSR1);
			}
		}

		printf("Parent: Time's up! Stopping the game\n");
		kill(pid, SIGTERM);
		wait(NULL);
		printf("Game finished\n");
	}

	return 0;
}

static void SignalHandler(int sig, siginfo_t* info, void* context)
{
	(void)info;
	(void)context;
	(void)sig;

	signal_received = 1;
}
