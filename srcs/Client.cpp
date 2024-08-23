#include "../inc/main.hpp"

Client::Client(int new_fd, std::string hostname) : _fd(new_fd), _hostname(hostname)
{
    std::cout << "Connection accepted from ip " << _hostname << ".\n";
}