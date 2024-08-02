#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Server
{
    public:
    Server(char *port, std::string pwd);
    void start();

    private:
    char *port;
    std::string pwd;
    int socketId, new_fd; //initial socket fd and client fd
    struct addrinfo hints, *res; //initial socket settings
};

#endif