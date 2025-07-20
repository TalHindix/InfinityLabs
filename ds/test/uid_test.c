/**************************************
Exercise: 	DS - UID
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#include <stdio.h>
#include <string.h>
#include "uid.h"


#define ASSERT(desc, cond) do { \
    if (!(cond)) { \
        printf("[FAIL] %s (line %d)\n", desc, __LINE__); \
        ++fails; \
    } else { \
        ++passes; \
    } \
} while (0)

int main(void)
{
    int passes = 0;
    int fails  = 0;

    ilrd_uid_t a = UIDCreate();
    ilrd_uid_t b = UIDCreate();
    ilrd_uid_t copy = a;

    /* 1. copy equals original */
    ASSERT("copy equals a", UIDIsSame(a, copy));

    /* 2. dummy a != b */
    ASSERT("a != b", !UIDIsSame(a, b));

    /* 3. valid a != dummy */
    ASSERT("a != UIDbadUID", !UIDIsSame(a, UIDbadUID));

    /* 4. dumnmy equals itself */
    ASSERT("UIDbadUID == UIDbadUID", UIDIsSame(UIDbadUID, UIDbadUID));

    printf("Summary: passes=%d fails=%d\n", passes, fails);
    return (fails == 0) ? 0 : 1;
}

