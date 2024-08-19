#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "main.hpp"

class Client
{
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _hostname;
    std::string _realname;

    public:
    Client(int new_fd, std::string hostname);
    ~Client() {};
    int getFd() const {return this->_fd;};
};

#endif