/**************************************
Exercise: 	DS - SCHED
Date:		21/7/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <stdio.h> /* printf 		*/

#include "sched.h" /* SchedCreate 	*/



int main(void)
{
	sched_t* sched = SchedCreate();
	SchedDestroy(sched);
	
	return 0;
}
