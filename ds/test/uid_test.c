/**************************************
Exercise: 	DS - UID
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		In Progress
**************************************/

#include <stdio.h>      /* printf */
#include <pthread.h>    /* pthread_create */
#include "uid.h"        /* UIDCreate */

#define ASSERT(desc, cond) do { \
    if (!(cond)) { \
        printf("[FAIL] %s (line %d)\n", desc, __LINE__); \
        ++fails; \
    } else { \
        ++passes; \
    } \
} while (0)

#define NUM_THREADS 5
#define UIDS_PER_THREAD 10

static void* ThreadFunc(void* arg);

int main(void)
{
    int passes = 0;
    int fails  = 0;
    pthread_t threads[NUM_THREADS] = {0};
    size_t indices[NUM_THREADS] = {0};
    size_t i = 0;

    ilrd_uid_t a = UIDCreate();
    ilrd_uid_t b = UIDCreate();
    ilrd_uid_t copy = a;

    for (i = 0; i < NUM_THREADS; ++i)
    {
        indices[i] = i;
        pthread_create(&threads[i], NULL, ThreadFunc, &indices[i]);
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    ASSERT("copy equals a", UIDIsSame(a, copy));
    ASSERT("a != b", !UIDIsSame(a, b));
    ASSERT("a != UIDbadUID", !UIDIsSame(a, UIDbadUID));
    ASSERT("UIDbadUID == UIDbadUID", UIDIsSame(UIDbadUID, UIDbadUID));

    printf("Summary: passes=%d fails=%d\n", passes, fails);

    return (fails == 0) ? 0 : 1;
}

static void* ThreadFunc(void* arg)
{
    size_t index = *(size_t*)arg;
    size_t i = 0;

    for (i = 0; i < UIDS_PER_THREAD; ++i)
    {
        ilrd_uid_t uid = UIDCreate();
        printf("Thread:%lu created UID counter=%lu, pid=%d\n",
        (unsigned long)index,
        (unsigned long)uid.counter,
        (int)uid.pid);
    }

    return NULL;
}
