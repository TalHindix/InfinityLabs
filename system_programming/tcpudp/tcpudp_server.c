
/**************************************
Exercise:   Ex - Server (UDP/TCP)
Date:       19/11/2025
Developer:  Tal Hindi
Reviewer:   Menny Markovich
Status:     In Progress
**************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc */
#include <unistd.h>     /* FD_ISSET */
#include <sys/select.h> /* fd_set */
#include <errno.h>      /* EAGAIN*/
#include "lib_socket.h" /* InitAddress */
#include <string.h>     /* strcmp */
#include <arpa/inet.h>  /* inet_ntoa */

#define BACKLOG (10)
#define MAX_CLIENTS (10)

int g_clients[MAX_CLIENTS];

static void PrepareSelectFds(fd_set* read_fds, int tcp_sock_fd, int udp_sock_fd, int* max_fd);
static void HandleStdin(int* is_running);
static void HandleTcpAccept(int tcp_sock_fd);
static void HandleUdp(int udp_sock_fd);
static void HandleTcpClients(fd_set* read_fds);

int main(int argc, char *argv[])
{
    int tcp_sock_fd = 0;
    int udp_sock_fd = 0;
    int is_running = 1;
    size_t i = 0;
    int port = (argc > 1) ? atoi(argv[1]) : PORT;

    tcp_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock_fd < 0)
    {
        perror("[Server] -> TCP Socket failed .\n");
        return -1;
    }

    if(BindToPort(tcp_sock_fd,port) < 0)
    {
        perror("[Server] -> TCP Bind to port failed .\n");
        close(tcp_sock_fd);
        return -1;
    }

    if(listen(tcp_sock_fd,BACKLOG) < 0)
    {
        perror("[Server] -> TCP Listen mode failed .\n");
        close(tcp_sock_fd);
        return -1;
    }

    SetNonBlocking(tcp_sock_fd);

    udp_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_sock_fd < 0)
    {
        perror("[Server] -> UDP Socket failed .\n");
        close(udp_sock_fd);
        close(tcp_sock_fd);
        return -1;
    }

    if(BindToPort(udp_sock_fd, port) < 0)
    {
        perror("[Server] -> UDP Bind to port failed .\n");
        close(udp_sock_fd);
        close(tcp_sock_fd);
    }

    EnableBroadcast(udp_sock_fd);
    SetNonBlocking(udp_sock_fd);


    for(i = 0; i < MAX_CLIENTS; ++i)
    {
        g_clients[i] = -1;
    }

    printf("[Server] -> is ready on port %d\n", port);

    while(is_running)
    {
        fd_set read_fds;
        struct timeval timeout = {7, 0};
        int max_fd = 0;
        int select_ret = 0;

        PrepareSelectFds(&read_fds, tcp_sock_fd, udp_sock_fd, &max_fd);

        select_ret = select(max_fd + 1, &read_fds, NULL, NULL , &timeout);

        if(0 == select_ret)
        {
            printf("[Server] -> Time out! 7 seconds passed \n");
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            HandleStdin(&is_running);
        }

        if (FD_ISSET(tcp_sock_fd, &read_fds))
        {
            HandleTcpAccept(tcp_sock_fd);
        }

        if (FD_ISSET(udp_sock_fd, &read_fds))
        {
            HandleUdp(udp_sock_fd);
        }

        HandleTcpClients(&read_fds);

    }

    printf("\n[Server] -> Shutting down...\n");

     for (i = 0; i < MAX_CLIENTS; ++i)
    {
        if (g_clients[i] != -1)
        {
            close(g_clients[i]);
        }
    }

    close(tcp_sock_fd);
    close(udp_sock_fd);

    return 0;
}

static void PrepareSelectFds(fd_set* read_fds, int tcp_sock_fd, int udp_sock_fd, int* max_fd)
{
    size_t i = 0;

    FD_ZERO(read_fds);
    FD_SET(STDIN_FILENO, read_fds);
    FD_SET(tcp_sock_fd, read_fds);
    FD_SET(udp_sock_fd, read_fds);

    *max_fd = tcp_sock_fd > udp_sock_fd ? tcp_sock_fd : udp_sock_fd;

    for (i = 0; i < MAX_CLIENTS; ++i)
    {
        if (-1 != g_clients[i])
        {
            FD_SET(g_clients[i], read_fds);
            if (g_clients[i] > *max_fd)
            {
                *max_fd = g_clients[i];
            }
        }
    }
}

static void HandleStdin(int* is_running)
{
    char buffer[BUFFER_SIZE] = {0};

    if (fgets(buffer, BUFFER_SIZE, stdin))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("[Server] -> STDIN %s\n", buffer);

        if (0 == strcmp(buffer, "ping"))
        {
            printf("[Server] STDOUT <- pong\n");
        }
        else if (0 == strcmp(buffer, "quit"))
        {
            *is_running = 0;
        }
    }
}

static void HandleTcpAccept(int tcp_sock_fd)
{
    int client_fd = accept(tcp_sock_fd, NULL, NULL);
    size_t slot = 0;

    if (client_fd < 0)
    {
        perror("[Server] -> TCP Accept failed");
        return;
    }

    SetNonBlocking(client_fd);

    for (slot = 0; slot < MAX_CLIENTS; ++slot)
    {
        if (-1 == g_clients[slot])
        {
            g_clients[slot] = client_fd;
            printf("[Server] -> TCP Client %ld connected fd num %d\n", slot , client_fd);
            return;
        }
    }

    printf("[Server] -> TCP Max clients reached\n");
    close(client_fd);
}

static void HandleUdp(int udp_sock_fd)
{
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in sender_addr = {0};
    socklen_t addr_len = sizeof(sender_addr);
    ssize_t bytes_received = 0;

    bytes_received = recvfrom(udp_sock_fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&sender_addr, &addr_len);

    if (bytes_received <= 0)
    {
        return;
    }

    buffer[bytes_received] = '\0';
    printf("[Server] -> UDP Client %s:%d -> %s\n", inet_ntoa(sender_addr.sin_addr), ntohs(sender_addr.sin_port), buffer);

    if (0 == strcmp(buffer, "ping"))
    {
        sendto(udp_sock_fd, "pong", 4, 0, (struct sockaddr*)&sender_addr, addr_len);
        printf("[Server] -> UDP Client <- pong\n");
    }
}

static void HandleTcpClients(fd_set* read_fds)
{
    size_t i = 0;

    for (i = 0; i < MAX_CLIENTS; ++i)
    {
        if (-1 != g_clients[i] && FD_ISSET(g_clients[i], read_fds))
        {
            char buffer[BUFFER_SIZE] = {0};
            ssize_t bytes_received = 0;

            bytes_received = recv(g_clients[i], buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received <= 0)
            {
                if (0 == bytes_received || (errno == EAGAIN && errno == EWOULDBLOCK))
                {
                    printf("[Server] -> TCP Client %ld died\n", i + 1);
                    close(g_clients[i]);
                    g_clients[i] = -1;
                }
            }
            else
            {
                buffer[bytes_received] = '\0';
                printf("[Server] -> TCP Client %ld -> %s\n", i + 1, buffer);

                if (0 == strcmp(buffer, "ping"))
                {
                    send(g_clients[i], "pong", 4, 0);
                    printf("[Server] -> TCP Client %ld <- pong\n", i + 1);
                }
            }
        }
    }
}
