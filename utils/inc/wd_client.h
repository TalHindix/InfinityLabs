/**************************************
Exercise:	Utils – Watchdog
Date:		28/09/2025
Developer:	Tal Hindi
Reviewer:	Shiran Swisa
Status:		
**************************************/

#ifndef __ILRD_WD_CLIENT_H__
#define __ILRD_WD_CLIENT_H__

#include <stddef.h> /* size_t */

typedef enum wd_status
{
    WD_SUCCESS = 0,
    WD_FAIL = 1
} wd_status_e;


wd_status_e WDStart(size_t tolerance, size_t interval, char** argv, int argc);
wd_status_e WDStop(void);

#endif /* __ILRD_WD_CLIENT_H__ */
