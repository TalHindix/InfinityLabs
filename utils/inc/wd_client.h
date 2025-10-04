/**************************************
Exercise:	Utils – Watchdog
Date:		28/09/2025
Developer:	Tal Hindi
Reviewer:	Shiran Swisa
Status:		Approved
**************************************/

#ifndef __ILRD_WD_CLIENT_H__
#define __ILRD_WD_CLIENT_H__

#include <stddef.h> /* size_t */

typedef enum wd_status
{
    WD_SUCCESS = 0,
    WD_FAIL = 1
} wd_status_e;


/*********************************************************
*  Brief:    Start the watchdog protection mechanism for the
*            calling process. Creates a watchdog process that
*            monitors the client process and revives it if it
*            becomes unresponsive.
*
*  Param:    tolerance - Maximum number of missed heartbeats before
*                        revival is triggered. Must be greater than 0.
*            interval - Time interval in seconds between heartbeat
*                       signals. Must be greater than 0.
*            argv - Command-line arguments for process revival,
*                   used to restart the client process if needed.
*            argc - Number of arguments in argv array.
*
*  Return:   WD_SUCCESS (0) if watchdog started successfully,
*            WD_FAIL (1) on failure (invalid parameters, resource
*            allocation error, or process creation failure).
*
*  Notes:    - Spawns a separate watchdog process and monitoring thread.
*            - Establishes bidirectional heartbeat monitoring between
*              client and watchdog processes.
*            - Blocks until initial handshake is complete.
*            - Must call WDStop() to terminate watchdog protection.
*
*  Complexity: O(1).
*********************************************************/
wd_status_e WDStart(size_t tolerance, size_t interval, char** argv, int argc);

/*********************************************************
*  Brief:    Stop the watchdog protection mechanism and terminate
*            the watchdog process and monitoring thread.
*
*  Param:    None.
*
*  Return:   WD_SUCCESS (0) on successful shutdown,
*            WD_FAIL (1) if shutdown encountered issues.
*
*  Notes:    - Sends termination signal to watchdog process.
*            - Waits for monitoring thread to complete.
*            - Cleans up semaphores and resources.
*            - Safe to call even if WDStart() failed.
*
*  Complexity: O(1).
*********************************************************/
wd_status_e WDStop(void);

#endif /* __ILRD_WD_CLIENT_H__ */
