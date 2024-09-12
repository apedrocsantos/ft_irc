#ifndef CMDHANDLER_HPP
#define CMDHANDLER_HPP

#include "main.hpp"

class CmdHandler {
    
    public:
    CmdHandler(class Command *cmd, class Client *client, class Server *server);
    ~CmdHandler() {};
    void nick(Command *cmd, Client *client, Server *server);
    void user(Command *cmd, Client *client, Server *server);
	void pass(Command *cmd, Client *client, Server *server);
    void pong(Command *cmd, Client *client, Server *server);
    void join(Command *cmd, Client *client, Server *server);
    void privmsg(Command *cmd, Client *client, Server *server);
    void part(Command *cmd, Client *client, Server *server);
    void kick(Command *cmd, Client *client, Server *server);
    void invite(Command *cmd, Client *client, Server *server);
    void topic(Command *cmd, Client *client, Server *server);
    void quit(Command *cmd, Client *client, Server *server);
    void mode(Command *cmd, Client *client, Server *server);
    void away(Command *cmd, Client *client, Server *server);
};

#endif