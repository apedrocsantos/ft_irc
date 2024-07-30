#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "lists.h"

#define BACKLOG 10
#define MESSAGE_BUFFER_SIZE 512
#define NICKNAME_SIZE 9
#define CHANNEL_NAME_SIZE 50

bool substring(char *str, const char *subst);

class Server
{
    public:
    Server(char *port);
    void start();

    private:
    int sockfd, new_fd; //initial socket fd and client fd
    struct addrinfo hints, *res; //initial socket settings
};

Server::Server(char *port)
{
    std::memset(&hints, 0, sizeof hints);
    this->hints.ai_family = AF_INET;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &this->res))
    {
        std::cerr << "getaddrinfo ERROR\n";
        exit(1);
    }
    this->sockfd = socket(this->res->ai_family, this->res->ai_socktype, this->res->ai_protocol);
    if(bind(this->sockfd, this->res->ai_addr, this->res->ai_addrlen))
    {
        std::cerr << "bind ERROR\n";
        exit(1);
    }
    freeaddrinfo(this->res);
    std::cout << "setup ok\n";
}

void Server::start()
{
    int msg_size = 1;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    char buf[MESSAGE_BUFFER_SIZE];
    int len;
    // const char *sep = "\r\n";
    // int sent_length;

    if(listen(this->sockfd, BACKLOG))
    {
        std::cerr << "listen ERROR\n";
        exit(1);
    }
    std::cout << "Waiting for connection\n";
    this->new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
    if (this->new_fd == -1)
    {
        std::cerr << "accept ERROR\n";
        exit(1);
    }

    while (true)
    {
        // recv
        msg_size = recv(this->new_fd, buf, MESSAGE_BUFFER_SIZE, 0);
        if (msg_size == -1)
        {
            std::cerr << "recv ERROR\n";
            break;
        }
        if (!msg_size)
            break;
        std::cout << buf;
        std::string str;
        str = buf;
        if(!str.compare(0, 4, "PING"))
        {
            std::string coiso = "PONG 127.0.0.1\r\n";
            len = coiso.length();
            send(this->new_fd, coiso.c_str(), len, 0);
            std::cout << "GOT PING\n";
        }
        std::memset(buf, 0, sizeof buf);

        // send
        const char *msg = "001 kifer :WELCOME!\r\n";
        int len = strlen(msg);
        int sent_length = send(this->new_fd, msg, len, 0);
        if (sent_length == -1)
        {
            std::cerr << "send ERROR\n";
            break;
        }
        if (sent_length != len)
            std::cout << "ERROR: Couldn't send full message.\n";
        std::cout << "RPL sent\n";
    }
    std::cout << "CLOSING\n";
    close(this->new_fd);
    close(sockfd);
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
    Server server(av[1]);
    server.start();
    return 0;
}