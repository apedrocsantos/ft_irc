#include "../inc/Channel.hpp"

Channel::Channel(std::string name) : _name(name), _user_limit(-1)
{
    std::cout << "Creating channel " << this->_name << std::endl;
    _flags.insert(_flags.end(), std::make_pair('i', false));
    _flags.insert(_flags.end(), std::make_pair('t', false));
    _flags.insert(_flags.end(), std::make_pair('k', false));
    _flags.insert(_flags.end(), std::make_pair('l', false));
}

void Channel::remove_member(std::string name)
{
    for (std::list<std::pair<std::string*, class Client *> >::iterator it = _members.begin(); it != _members.end();)
    {
        if (*it->first == name)
        {
            std::list<std::pair<std::string*, class Client *> >::iterator temp = it;
            ++temp;
            _members.erase(it);
            it = temp;
        }
        else
            ++it;
    }
}

void Channel::remove_operator(int fd)
{
    for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); it++)
    {
        if (*it == fd)
        {
            _operators.erase(it);
            return ;
        }
    }
}

std::string Channel::get_members()
{
    std::string output = "";
    for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = _members.begin(); it_members != _members.end(); it_members++)
    {
        if (std::find(_operators.begin(), _operators.end(), it_members->second->getFd()) != _operators.end())
            output += "@";
        output += *it_members->first + " ";
    }
    return output;
}

bool Channel::member_exists(const std::string name)
{
    for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = _members.begin(); it_members != _members.end(); it_members++)
    {
        if (*it_members->first == name)
            return true;
    }
    return false;
}