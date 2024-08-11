#ifndef CMDLIST_HPP
#define CMDLIST_HPP

#include "main.hpp"

class CmdList {
    
    public:
    CmdList(class Command *cmd, class Client *client);
    ~CmdList() {};
    void pass(std::string params, class Client *client);
    void nick(std::string params, class Client *client);
    void send_msg(std::string msg, class Client *client);
};

#endif