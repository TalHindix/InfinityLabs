/**************************************
Exercise:	Utils – Watchdog
Date:		28/09/2025
Developer:	Tal Hindi
Reviewer:	Shiran Swisa
Status:		
**************************************/

#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep */

#include "wd_client.h" /* WDStart */

#define SLEEP_TIME (5)

static void DoWork(unsigned int seconds);

int main(int argc, char** argv)
{
	WDStart(5, 3, argv, argc);

	printf(">>> starting user job\n");

	DoWork(SLEEP_TIME);

	WDStop();

	printf(">>> user job done\n");

	return 0;
}

static void DoWork(unsigned int seconds)
{
	unsigned int remaining = seconds;

	while (remaining)
	{
		remaining = sleep(remaining);
	}
}
