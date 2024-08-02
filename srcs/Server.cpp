#include "../inc/Server.hpp"

Server::Server(char *port, std::string pwd)
{
    int optval = 1;
    this->port = port;
    this->pwd = pwd;
    std::memset(&hints, 0, sizeof hints);
    this->hints.ai_family = AF_INET;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &this->res))
        exit_routine("getaddrinfo", socketId);
    this->socketId = socket(this->res->ai_family, this->res->ai_socktype, this->res->ai_protocol);
    if (this->socketId == -1)
        exit_routine("Can't create a socket!", socketId);
    if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        exit_routine("Error setting socket options", socketId);
    if(bind(this->socketId, this->res->ai_addr, this->res->ai_addrlen))
        exit_routine("Can't bind to IP/port", socketId);
    freeaddrinfo(this->res);
    std::cout << "Setup ok\n";
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

    if(listen(this->socketId, BACKLOG))
    {
        std::cerr << "listen ERROR\n";
        exit(1);
    }
    std::cout << "Waiting for connection\n";
    this->new_fd = accept(socketId, (struct sockaddr *)&client_addr, &addr_size);
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
    close(socketId);
}