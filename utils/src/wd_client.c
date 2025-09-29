/**************************************
Exercise:	Utils – Watchdog Client
Date:		28/09/2025
Developer:	Tal Hindi
Reviewer:	Shiran Swisa
Status:		Approved
**************************************/

#define _POSIX_C_SOURCE (200809L)

#include <stdio.h>		/* perror */
#include <stdlib.h>		/* calloc */
#include <unistd.h>		/* fork */
#include <pthread.h>	/* pthread_create */
#include <signal.h>		/* kill */
#include <semaphore.h>	/* sem_t */
#include <fcntl.h>		/* O_CREAT */
#include <errno.h>		/* errno */
#include <assert.h>		/* assert */
#include <string.h>		/* snprintf */

#include "sched.h"		/* SchedCreate */
#include "wd_client.h"	/* WDStart */
#include "wd_lib.h"		/* WDTasksInit */

#define WD_SEM_NAME			"/wd_ready_sem"
#define CLIENT_SEM_NAME		"/client_ready_sem"
#define BUFFER_SIZE			(32)
#define WD_BIN_PATH			"./bin/debug/wd"

typedef struct client_args
{
	sched_t* sched;
	pid_t wd_pid;
	size_t tolerance;
	size_t interval_sec;
	int argc;
	char** argv;
	sem_t* user_sem;
} client_args_t;

static pthread_t g_client_tid = 0;
static pid_t g_wd_pid = 0;

static void* ClientThread(void* arg);
static pid_t SpawnWatchdog(const client_args_t* args);
static void SetHandshake(sem_t* client_sem, sem_t* wd_sem);

wd_status_e WDStart(size_t tolerance, size_t interval_sec, char** argv, int argc)
{
	pthread_t tid = 0;
	client_args_t* args = NULL;
	sem_t user_sem;
	int ret = 0;

	if ((0 == tolerance) || (0 == interval_sec))
	{
		return WD_FAIL;
	}

	/* unnamed semaphore for thread handshake */
	if (0 != sem_init(&user_sem, 0, 0))
	{
		return WD_FAIL;
	}

	args = (client_args_t*)calloc(1, sizeof(*args));
	if (NULL == args)
	{
		sem_destroy(&user_sem);
		return WD_FAIL;
	}
	args->tolerance = tolerance;
	args->interval_sec = interval_sec;
	args->argc = argc;
	args->argv = argv;
	args->user_sem = &user_sem;

	args->sched = SchedCreate();
	if (NULL == args->sched)
	{
		free(args);
		sem_destroy(&user_sem);
		return WD_FAIL;
	}

	args->wd_pid = SpawnWatchdog(args);
	if (0 >= args->wd_pid)
	{
		SchedDestroy(args->sched);
		free(args);
		sem_destroy(&user_sem);
		return WD_FAIL;
	}

	g_wd_pid = args->wd_pid;

	if (0 != WDTasksInit(args->sched, 0, args->wd_pid,
	                     args->interval_sec, args->tolerance,
	                     args->argc, args->argv))
	{
		kill(args->wd_pid, SIGKILL);
		SchedDestroy(args->sched);
		free(args);
		sem_destroy(&user_sem);
		return WD_FAIL;
	}

	if (0 != pthread_create(&tid, NULL, ClientThread, args))
	{
		kill(args->wd_pid, SIGKILL);
		SchedDestroy(args->sched);
		free(args);
		sem_destroy(&user_sem);
		return WD_FAIL;
	}
	g_client_tid = tid;

	ret = sem_wait(&user_sem);
	sem_destroy(&user_sem);
	if (0 != ret)
	{
		WDTasksRequestStop();
		kill(g_wd_pid, SIGUSR2);
		return WD_FAIL;
	}

	return WD_SUCCESS;
}

wd_status_e WDStop(void)
{
	pthread_t tid = 0;
	pid_t wd_pid = 0;

	tid = g_client_tid;
	wd_pid = g_wd_pid;

	if (0 < wd_pid)
	{
		kill(wd_pid, SIGUSR2);
	}

	WDTasksRequestStop();

	if (0 != tid)
	{
		pthread_join(tid, NULL);
	}

	sem_unlink(WD_SEM_NAME);
	sem_unlink(CLIENT_SEM_NAME);

	g_client_tid = 0;
	g_wd_pid = 0;

	return WD_SUCCESS;
}

static void* ClientThread(void* arg)
{
	client_args_t* args = NULL;
	sem_t* wd_sem = NULL;
	sem_t* client_sem = NULL;

	args = (client_args_t*)arg;
	if (NULL == args)
	{
		return NULL;
	}

	wd_sem = sem_open(WD_SEM_NAME, 0);
	if (SEM_FAILED == wd_sem)
	{
		sem_post(args->user_sem);
		SchedDestroy(args->sched);
		free(args);
		return NULL;
	}

	client_sem = sem_open(CLIENT_SEM_NAME, 0);
	if (SEM_FAILED == client_sem)
	{
		sem_close(wd_sem);
		sem_post(args->user_sem);
		SchedDestroy(args->sched);
		free(args);
		return NULL;
	}

	SetHandshake(client_sem, wd_sem);

	sem_post(args->user_sem);

	SchedRun(args->sched);

	sem_close(wd_sem);
	sem_close(client_sem);
	SchedDestroy(args->sched);
	free(args);

	return NULL;
}

static pid_t SpawnWatchdog(const client_args_t* args)
{
	pid_t pid = 0;
	char ppid_str[BUFFER_SIZE] = {0};
	char interval_str[BUFFER_SIZE] = {0};
	char tolerance_str[BUFFER_SIZE] = {0};
	char buf[BUFFER_SIZE] = {0};
	char** wd_argv = NULL;
	size_t total = 0;
	size_t i = 0;

	assert(NULL != args);

	pid = fork();
	if (0 > pid)
	{
		return -1;
	}

	if (0 == pid)
	{
		snprintf(ppid_str, sizeof(ppid_str), "%d", (int)getppid());
		snprintf(interval_str, sizeof(interval_str), "%lu",
		         (unsigned long)args->interval_sec);
		snprintf(tolerance_str, sizeof(tolerance_str), "%lu",
		         (unsigned long)args->tolerance);

		total = (size_t)(4 + args->argc);
		wd_argv = (char**)malloc(sizeof(char*) * (total + 1));
		if (NULL == wd_argv)
		{
			_exit(1);
		}

		wd_argv[0] = WD_BIN_PATH;
		wd_argv[1] = ppid_str;
		wd_argv[2] = interval_str;
		wd_argv[3] = tolerance_str;

		for (i = 0; i < (size_t)args->argc; ++i)
		{
			wd_argv[4 + i] = args->argv[i];
		}
		wd_argv[total] = NULL;

		execvp(wd_argv[0], wd_argv);
		_exit(1);
	}


	snprintf(buf, sizeof(buf), "%d", (int)pid);
	setenv("WD_PID", buf, 1);

	return pid;
}

static void SetHandshake(sem_t* client_sem, sem_t* wd_sem)
{
	sem_wait(client_sem);
	sem_post(wd_sem);
}
