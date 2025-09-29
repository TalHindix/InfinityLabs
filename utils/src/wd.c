/**************************************
Exercise:	Utils – Watchdog (Process)
Date:		28/09/2025
Developer:	Tal Hindi
Reviewer:	Shiran Swisa
Status:		Approved
**************************************/

#define _POSIX_C_SOURCE (200809L)

#include <stdio.h>		/* fprintf */
#include <stdlib.h>		/* atoi */
#include <semaphore.h>	/* sem_open */
#include <fcntl.h>		/* O_CREAT */

#include "sched.h"		/* SchedCreate */
#include "wd_lib.h"		/* WDTasksInit */

#define WD_SEM_NAME 		"/wd_ready_sem"
#define CLIENT_SEM_NAME 	"/client_ready_sem"

static void SetHandshake(sem_t* wd_sem, sem_t* client_sem);

int main(int argc, char** argv)
{
	sched_t* sched = NULL;
	pid_t client_pid = 0;
	size_t interval_sec = 0;
	size_t tolerance = 0;
	sem_t* wd_sem = NULL;
	sem_t* client_sem = NULL;

	if (4 > argc)
	{
		fprintf(stderr, "Usage: wd <client_pid> <interval_sec> <tolerance>\n");
		return 1;
	}

	client_pid = (pid_t)atoi(argv[1]);
	interval_sec = (size_t)atoi(argv[2]);
	tolerance = (size_t)atoi(argv[3]);

	WDTasksInstallStopHandler();

	sched = SchedCreate();
	if (NULL == sched)
	{
		return 1;
	}

	if (0 != WDTasksInit(sched, 1, client_pid, interval_sec, tolerance, argc, argv))
	{
		SchedDestroy(sched);
		return 1;
	}

	wd_sem = sem_open(WD_SEM_NAME, O_CREAT, 0600, 0);
	if (SEM_FAILED == wd_sem)
	{
		SchedDestroy(sched);
		return 1;
	}

	client_sem = sem_open(CLIENT_SEM_NAME, O_CREAT, 0600, 0);
	if (SEM_FAILED == client_sem)
	{
		sem_close(wd_sem);
		sem_unlink(WD_SEM_NAME);
		SchedDestroy(sched);
		return 1;
	}

	SetHandshake(wd_sem, client_sem);

	SchedRun(sched);

	sem_close(wd_sem);
	sem_close(client_sem);
	sem_unlink(WD_SEM_NAME);
	sem_unlink(CLIENT_SEM_NAME);
	SchedDestroy(sched);

	return 0;
}

static void SetHandshake(sem_t* wd_sem, sem_t* client_sem)
{
	sem_post(client_sem);
	sem_wait(wd_sem);
}
