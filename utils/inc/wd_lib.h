/**************************************
Exercise:	Utils – Watchdog
Date:		28/09/2025
Developer:	Tal Hindi
Reviewer:	Shiran Swisa
Status:		Approved
**************************************/

#ifndef __ILRD_WD_LIB_H__
#define __ILRD_WD_LIB_H__

#include <stddef.h> 	/* size_t */
#include <sys/types.h> 	/* pid_t */

#include "sched.h" 		/* sched_t */


int WDTasksInit(sched_t* sched,
			  int is_wd,
			  pid_t peer_pid,
			  size_t interval_sec,
			  size_t tolerance,
			  int argc,
			  char** argv);

void WDTasksRequestStop(void);

void WDTasksInstallStopHandler(void);


#endif /* __ILRD_WD_LIB_H__ */


