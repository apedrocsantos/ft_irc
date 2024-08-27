#include "../inc/Channel.hpp"

Channel::Channel(std::string name) : _name(name), _inviteOnly(false), _user_limit(-1)
{
    std::cout << "Creating channel " << this->_name << std::endl;
}

void Channel::remove_member(std::string name)
{
    for (std::map<const std::string*, class Client *>::iterator it = _members.begin(); it != _members.end();)
    {
        if (*it->first == name)
        {
            std::map<const std::string*, class Client *>::iterator temp = it;
            ++temp;
            _members.erase(it);
            it = temp;
        }
        else
            ++it;
    }
}

std::string Channel::get_members()
{
    std::string output = "";
    for (std::map<const std::string *, class Client *>::iterator it_members = _members.begin(); it_members != _members.end(); it_members++)
        output += *it_members->first + " ";
    return output;
}

bool Channel::member_exists(const std::string *name)
{
    std::map<const std::string*, Client*>::iterator it = this->_members.find(name);
    if (it != this->_members.end())
        return true;
    return false;
}