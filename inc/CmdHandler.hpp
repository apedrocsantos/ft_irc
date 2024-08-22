#ifndef CMDHANDLER_HPP
#define CMDHANDLER_HPP

#include "main.hpp"

class CmdHandler {
    
    public:
    CmdHandler(class Command *cmd, class Client *client, class Server *server);
    ~CmdHandler() {};
    void nick(Command *cmd, Client *client);

	/**
	 * USER <username> <hostname> <servername> <realname>
	 * USER <username> 0 * :<realname>
	 */
	void user(Command *cmd, Client *client, Server *server);
	void pass(std::string params, Client *client);
	// void send_msg(std::string msg, Client *client);
};

#endif