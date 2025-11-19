/**************************************
Exercise:   Lib - Socket
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/

#ifndef _ILRD_LIBSOCKET
#define _ILRD_LIBSOCKET

#include <stddef.h>     /* size_t */
#include <netinet/in.h> /* struct sockaddr_in */

#define PORT 5000
#define BUFFER_SIZE 256

/* Binds socket to port, returns 0 on success, -1 on error */
int BindToPort(int socket_fd, int port);

/* Enables broadcast on socket, returns 0 on success, -1 on error */
int EnableBroadcast(int socket_fd);

/* Initializes sockaddr_in with ip and port */
void InitAddress(struct sockaddr_in* addr, const char* ip, int port);

#endif /* _ILRD_LIBSOCKET */
