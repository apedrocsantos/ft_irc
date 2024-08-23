#include "../inc/Channel.hpp"

Channel::Channel(std::string name) : _name(name), _inviteOnly(false), _user_limit(-1)
{
    std::cout << "Creating channel " << this->_name << std::endl;
}
