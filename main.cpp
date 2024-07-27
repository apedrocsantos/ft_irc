#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

#define BACKLOG 10
#define MESSAGE_BUFFER_SIZE 512
#define NICKNAME_SIZE 9
#define CHANNEL_NAME_SIZE 50

int server(char *port)
{
    int sockfd, new_fd;
    struct addrinfo hints, *res;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &res))
    {
        std::cerr << "getaddrinfo ERROR\n";
        return (1);
    }
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(bind(sockfd, res->ai_addr, res->ai_addrlen))
    {
        std::cerr << "bind ERROR\n";
        return (1);
    }
    if(listen(sockfd, BACKLOG))
    {
        std::cerr << "listen ERROR\n";
        return (1);
    }

    addr_size = sizeof client_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
    send(new_fd, "blablabla\n", 11, 0);
    char buf[MESSAGE_BUFFER_SIZE];
    recv(new_fd, buf, MESSAGE_BUFFER_SIZE, 0);
    std::cout << buf << " ok\n";
    close(new_fd);
    close(sockfd);
    freeaddrinfo(res);
    return (0);
}

int main(int ac, char **av)
{
    if (ac != 3)
        return 1;
    std::string passwd = av[2];
    if (atoi(av[1]) <= 1024 || atoi(av[1]) > 65535 )
    {
        std::cout << "ERROR\n";
        return 1;
    }
    char *port = av[1];
    server(port);
    return 0;
}