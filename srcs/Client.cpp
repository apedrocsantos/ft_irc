#include "../inc/Client.hpp"

Client::Client(int new_fd, std::string hostname) : _fd(new_fd), _hostname(hostname)
{
    (void) _fd;
    std::cout << "Connection accepted from ip " << _hostname << ".\n";
}

int Client::getFd()
{
    return this->_fd;
}