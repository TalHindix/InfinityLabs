/**************************************
Exercise: 	DS - UID
Date:		20/7/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		Approved
**************************************/

#include "uid.h" /* UIDCreate */

#include <string.h>      /* memset */
#include <ifaddrs.h>     /* getifaddrs */
#include <sys/types.h>   /* AF_INET */
#include <sys/socket.h>  /* struct sockaddr */
#include <netinet/in.h>  /* struct sockaddr_in */
#include <arpa/inet.h>   /* inet_ntoa */
#include <unistd.h>      /* */
#include <pthread.h>     /* pthread_mutex */

#define IP_LEN (INET_ADDRSTRLEN)

/* "Bad" UID Indicate failure */
const ilrd_uid_t UIDbadUID = {0, 0, 0, {0}};
static size_t counter = 0;

static time_t GetTimeStamp(void);
static pid_t GetPID(void);
static int GetIPAddress(unsigned char dest[IP_LEN]);

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t new_uid = {0};
    
    pthread_mutex_lock(&lock);
	new_uid.counter = ++counter;
    

	new_uid.timestamp = GetTimeStamp();
	new_uid.pid = GetPID();
	
	if (0 != GetIPAddress(new_uid.ip))
    {
        pthread_mutex_unlock(&lock);
        return UIDbadUID;
    }
    pthread_mutex_unlock(&lock);
    return new_uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{ 

    return (uid1.counter   == uid2.counter)  &&
           (uid1.timestamp == uid2.timestamp) &&
           (uid1.pid       == uid2.pid)       &&
           (0 == strncmp((char*)uid1.ip,(char*)uid2.ip, IP_LEN));
}

static time_t GetTimeStamp(void)
{
	return time(NULL);
}

static pid_t GetPID(void)
{
	return getpid();
}

static int GetIPAddress(unsigned char dest[IP_LEN])
{
    struct ifaddrs *ifaddr = NULL;
    struct ifaddrs *ifa    = NULL;
    int success = 0;

    if (0 != getifaddrs(&ifaddr))
    {
        return -1;
    }

    for (ifa = ifaddr; NULL != ifa; ifa = ifa->ifa_next)
    {
        if (NULL == ifa->ifa_addr)
        {
            continue;
        }

        if (AF_INET == ifa->ifa_addr->sa_family)
        {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            char addr_str[INET_ADDRSTRLEN] = {0}; /* Thread-safe buffer */

            if (NULL != inet_ntop(AF_INET, &(sa->sin_addr),
                                  addr_str, sizeof(addr_str)))
            {
                /* Ignore loopback (127.0.0.1) */
                if (0 != strcmp(addr_str, "127.0.0.1"))
                {
                    strncpy((char *)dest, addr_str, IP_LEN - 1);
                    dest[IP_LEN - 1] = '\0';
                    success = 1;
                    break;
                }
            }
        }
    }

    freeifaddrs(ifaddr);
    return success ? 0 : -1;
}


