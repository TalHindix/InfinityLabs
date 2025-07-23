/**************************************
Exercise: 	DS - UID
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		
**************************************/

#ifndef ILRD_UID_H
#define ILRD_UID_H

#include <time.h> /* time_t */
#include <sys/types.h> /* pid_t */

#define IP_LEN 14

typedef struct uid
{
    size_t counter;
    time_t timestamp;
    pid_t pid;
    unsigned char ip[IP_LEN];
}ilrd_uid_t;

/* Global constant to represent an invalid UID */
extern const ilrd_uid_t UIDbadUID;

/**
 * Function: UIDCreate
 * -------------------
 * Creates a unique identifier (UID) based on:
 *  - Incremental counter
 *  - Current timestamp
 *  - Calling process PID
 *  - Local IP address
 *
 * Returns:
 *  - A valid ilrd_uid_t UID if successful.
 *  - UIDbadUID if any step fails.
 *
 * Complexity: O(1)
 */
ilrd_uid_t UIDCreate(void); /* O(1) */

/**
 * Function: UIDIsSame
 * -------------------
 * Compares two UIDs for equality.
 *
 * Parameters:
 *  - uid1: First UID to compare.
 *  - uid2: Second UID to compare.
 *
 * Returns:
 *  - 1 if the UIDs are equal.
 *  - 0 if the UIDs are different.
 *
 * Complexity: O(1)
 */
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2); /* O(1) */

#endif /* ILRD_UID_H */
