/**************************************
Exercise:	Utils – Watchdog "Shared Lib"
Date:		28/09/2025
Developer:	Tal Hindi
Reviewer:	Shiran Swisa
Status:		
**************************************/

#define _POSIX_C_SOURCE (200809L)

#include <signal.h>		/* sigaction */
#include <stdio.h>		/* printf */
#include <stdlib.h>		/* calloc */
#include <string.h>		/* memset */
#include <unistd.h>		/* getpid */
#include <assert.h>		/* assert */
#include <errno.h>		/* errno */
#include <semaphore.h>	/* sem_t */
#include <fcntl.h>		/* O_CREAT */

#include "wd_lib.h"		/* WDTasksInit */
#include "sched.h"		/* SchedAdd */
#include "uid.h"		/* ilrd_uid_t */

#define WD_BIN_PATH				"./bin/debug/wd"
#define WD_READY_SEM_NAME		"/wd_ready_sem"
#define CLIENT_READY_SEM_NAME	"/client_ready_sem"
#define BUFFER_SIZE 			(32)

typedef struct wd_ctx
{
	sched_t* sched;
	pid_t peer_pid;
	size_t interval_sec;
	size_t tolerance;
	volatile sig_atomic_t counter;		/* missed heartbeats since last tick */
	volatile sig_atomic_t stop_flag;	/* 0=run, 1=request peer stop, 2=stop self */
	int is_wd;							/* 1=WD, 0=CLIENT */
	int argc;
	char** argv;
} wd_ctx_t;


static wd_ctx_t* g_ctx = NULL;

static void SigUsr1Handler(int sig);
static void SigUsr2Handler(int sig);
static ssize_t TaskSendHeartbeat(void* param);
static ssize_t TaskCheckCounter(void* param);
static void NoCleanup(void* param);
static int ReviveProcess(wd_ctx_t* ctx);

int WDTasksInit(sched_t* sched,
				int is_wd,
				pid_t peer_pid,
				size_t interval_sec,
				size_t tolerance,
				int argc,
				char** argv)
{
	ilrd_uid_t uid = UIDbadUID;
	struct sigaction sa;

	assert(NULL != sched);

	g_ctx = (wd_ctx_t*)calloc(1, sizeof(*g_ctx));
	if (NULL == g_ctx)
	{
		return -1;
	}

	g_ctx->sched = sched;
	g_ctx->peer_pid = peer_pid;
	g_ctx->interval_sec = interval_sec;
	g_ctx->tolerance = tolerance;
	g_ctx->counter = 0;
	g_ctx->stop_flag = 0;
	g_ctx->is_wd = is_wd;
	g_ctx->argc = argc;
	g_ctx->argv = argv;

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = SigUsr1Handler;
	sigemptyset(&sa.sa_mask);
	if (0 != sigaction(SIGUSR1, &sa, NULL))
	{
		free(g_ctx);
		g_ctx = NULL;
		return -1;
	}

	sa.sa_handler = SigUsr2Handler;
	if (0 != sigaction(SIGUSR2, &sa, NULL))
	{
		free(g_ctx);
		g_ctx = NULL;
		return -1;
	}

	uid = SchedAdd(sched, TaskSendHeartbeat, g_ctx, 0, NoCleanup, NULL);
	if (UIDIsSame(UIDbadUID, uid))
	{
		free(g_ctx);
		g_ctx = NULL;
		return -1;
	}

	uid = SchedAdd(sched, TaskCheckCounter, g_ctx, 1, NoCleanup, NULL);
	if (UIDIsSame(UIDbadUID, uid))
	{
		free(g_ctx);
		g_ctx = NULL;
		return -1;
	}

	return 0;
}

void WDTasksRequestStop(void)
{
	if (NULL != g_ctx)
	{
		g_ctx->stop_flag = 1;
	}
}

void WDTasksInstallStopHandler(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = SigUsr2Handler;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGUSR2, &sa, NULL);
	
}

static void SigUsr1Handler(int sig)
{
	(void)sig;

	if (NULL != g_ctx)
	{
		g_ctx->counter = 0;
	}
}

static void SigUsr2Handler(int sig)
{
	(void)sig;

	if (NULL != g_ctx)
	{
		g_ctx->stop_flag = 2;

		if ((1 == g_ctx->is_wd) && (0 < g_ctx->peer_pid))
		{
			kill(g_ctx->peer_pid, SIGUSR2);
		}
	}
}

static ssize_t TaskSendHeartbeat(void* param)
{
	wd_ctx_t* ctx = NULL;
	int signal_to_send = 0;

	ctx = (wd_ctx_t*)param;
	if (NULL == ctx)
	{
		return 0;
	}

	if (2 == ctx->stop_flag)
	{
		return 0;
	}

	signal_to_send = (0 == ctx->stop_flag) ? SIGUSR1 : SIGUSR2;

	kill(ctx->peer_pid, signal_to_send);

	++(ctx->counter);

	return (ssize_t)ctx->interval_sec;
}

static ssize_t TaskCheckCounter(void* param)
{
	wd_ctx_t* ctx = NULL;
	sig_atomic_t counter_value = 0;

	ctx = (wd_ctx_t*)param;
	if (NULL == ctx)
	{
		return 0;
	}

	if (2 == ctx->stop_flag)
	{
		SchedClear(ctx->sched);
		return 0;
	}

	counter_value = ctx->counter;

	if ((size_t)counter_value > ctx->tolerance)
	{
		SchedClear(ctx->sched);

		if (0 != ReviveProcess(ctx))
		{
			return 0;
		}

		if (UIDIsSame(UIDbadUID, SchedAdd(ctx->sched, TaskSendHeartbeat, ctx, 0, NoCleanup, NULL)))
		{
			return 0;
		}
		if (UIDIsSame(UIDbadUID, SchedAdd(ctx->sched, TaskCheckCounter, ctx, 0, NoCleanup, NULL)))
		{
			return 0;
		}
	}

	return (ssize_t)ctx->interval_sec;
}

static int ReviveProcess(wd_ctx_t* ctx)
{
	pid_t new_pid = 0;
	sem_t* wd_sem = NULL;
	sem_t* client_sem = NULL;
	char wd_pid_str[BUFFER_SIZE] = {0};
	char ppid_str[BUFFER_SIZE] = {0};
	char interval_str[BUFFER_SIZE] = {0};
	char tolerance_str[BUFFER_SIZE] = {0};
	char buf[BUFFER_SIZE] = {0};
	char** wd_argv = NULL;
	char** client_argv = NULL;
	size_t total = 0;
	size_t i = 0;

	assert(NULL != ctx);

	if (0 < ctx->peer_pid)
	{
		kill(ctx->peer_pid, SIGKILL);
	}

	if (1 == ctx->is_wd)
	{
		client_argv = ctx->argv + 4;

		snprintf(wd_pid_str, sizeof(wd_pid_str), "%d", (int)getpid());
		setenv("WD_PID", wd_pid_str, 1);
		setenv("WD_REVIVED", "1", 1);

		execvp(client_argv[0], client_argv);
		return -1;
	}

	new_pid = fork();
	if (0 > new_pid)
	{
		return -1;
	}

	if (0 == new_pid)
	{
		snprintf(ppid_str, sizeof(ppid_str), "%d", (int)getppid());
		snprintf(interval_str, sizeof(interval_str), "%lu", (unsigned long)ctx->interval_sec);
		snprintf(tolerance_str, sizeof(tolerance_str), "%lu", (unsigned long)ctx->tolerance);

		total = (size_t)(4 + ctx->argc);
		wd_argv = (char**)malloc(sizeof(char*) * (total + 1));
		if (NULL == wd_argv)
		{
			_exit(1);
		}

		wd_argv[0] = WD_BIN_PATH;
		wd_argv[1] = ppid_str;
		wd_argv[2] = interval_str;
		wd_argv[3] = tolerance_str;

		for (i = 0; i < (size_t)ctx->argc; ++i)
		{
			wd_argv[4 + i] = ctx->argv[i];
		}
		wd_argv[total] = NULL;

		execvp(wd_argv[0], wd_argv);
		_exit(1);
	}

	ctx->peer_pid = new_pid;
	ctx->counter = 0;

	snprintf(buf, sizeof(buf), "%d", (int)new_pid);
	setenv("WD_PID", buf, 1);

	wd_sem = sem_open(WD_READY_SEM_NAME, O_CREAT, 0600, 0);
	client_sem = sem_open(CLIENT_READY_SEM_NAME, O_CREAT, 0600, 0);

	if ((SEM_FAILED != wd_sem) && (SEM_FAILED != client_sem))
	{
		sem_wait(client_sem);
		sem_post(wd_sem);

		sem_close(wd_sem);
		sem_close(client_sem);
	}

	return 0;
}

static void NoCleanup(void* param)
{
	(void)param;
}
