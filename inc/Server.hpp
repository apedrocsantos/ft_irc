#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Server
{
    public:
    Server(char *port, std::string pwd);
    void start();
    ~Server();

    private:
    char *port;
    std::string pwd;
    int socketId, new_fd; //initial socket fd and client fd
    struct addrinfo hints, *res; //initial socket settings
    char buf[MESSAGE_BUFFER_SIZE];
    std::vector <struct pollfd> pollfds;
    void receive_msg();
    // int read();
    // int send();
};

#endif