#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>

#include "lib_socket.h"

#define BROADCAST_PORT (PORT + 1)

static void* HandleTcpClient(void* arg);
static int SetupSockets(int* tcp_sock, int* udp_sock, int* broadcast_sock, int port);
static void HandleUdpMessage(int udp_sock);
static void HandleBroadcastMessage(int broadcast_sock);
static int SetNonBlocking(int fd);


int main(int argc, char *argv[])
{
    int tcp_sock = 0;
    int udp_sock = 0;
    int broadcast_sock = 0;
    int port = PORT;

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    printf("Multi-Protocol Server starting...\n");
    printf("TCP Port: %d\n", port);
    printf("UDP Port: %d\n", port);
    printf("Broadcast Port: %d\n", BROADCAST_PORT);



    return 0;
}

static int SetupSockets(int* tcp_sock, int* udp_sock, int* broadcast_sock, int port)
{
    *tcp_sock = TcpSocket();
    if (*tcp_sock < 0 || BindToPort(*tcp_sock, port) < 0)
    {
        perror("Setup failed");
        return 1;
    }

    if (listen(*tcp_sock, 5) < 0)
    {
        perror("Listen failed");
        return 1;
    }

    *udp_sock = UdpSocket();
    if (*udp_sock < 0 || BindToPort(*udp_sock, port) < 0)
    {
        perror("Setup failed");
        return 1;
    }

    printf("UDP Server listening on port %d\n", port);


}

static int SetNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
