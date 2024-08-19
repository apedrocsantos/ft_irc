#ifndef CMDHANDLER_HPP
#define CMDHANDLER_HPP

#include "main.hpp"

class CmdHandler {
    
    public:
    CmdHandler(class Command *cmd, class Client *client, class Server *server);
    ~CmdHandler() {};
    // void pass(std::string params, Client *client);
    // void nick(std::string params, Client *client);
    // void send_msg(std::string msg, Client *client);
};

#endif