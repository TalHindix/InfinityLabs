/**************************************
Exercise:   Lib - Socket
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:
Status:
**************************************/

#include <sys/socket.h> /* socket */
#include <string.h>     /* memset */
#include <arpa/inet.h>  /* inet_pton */

#include "lib_socket.h" /* UdpSocket */

int UdpSocket(void)
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

int TcpSocket(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (socket_fd < 0)
    {
        return -1;
    }

    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    return socket_fd;
}

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
