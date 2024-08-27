#include "../inc/main.hpp"

Client::Client(int new_fd, std::string hostname) : _fd(new_fd), _hostname(hostname)
{
    std::cout << "Connection accepted from ip " << _hostname << ".\n";
}

void Client::remove_channel(std::string &name)
{
    for (std::vector<std::string>::iterator it = this->_channels.begin(); it != this->_channels.end();)
    {
        if (*it == name)
            it = this->_channels.erase(it);
        else
            it++;
    }
}
