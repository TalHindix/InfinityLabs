/**************************************
Exercise:   Lib - Socket
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/

#include <sys/socket.h> /* socket */
#include <string.h>     /* memset */
#include <arpa/inet.h>  /* inet_pton */
#include <fcntl.h>      /* fcntl */
#include "lib_socket.h" /* UdpSocket */


int BindToPort(int socket_fd, int port)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr));
}

int EnableBroadcast(int socket_fd)
{
    int enable = 1;

    return setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
}

void InitAddress(struct sockaddr_in* addr, const char* ip, int port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    if (ip)
    {
        inet_pton(AF_INET, ip, &addr->sin_addr);
    }
    else
    {
        addr->sin_addr.s_addr = INADDR_ANY;
    }
}

int SetNonBlocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        return -1;
    }

    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}
