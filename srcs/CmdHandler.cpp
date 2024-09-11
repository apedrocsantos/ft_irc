#include "../inc/main.hpp"

CmdHandler::CmdHandler(Command *cmd, Client *client, Server *server)
{
    if (cmd->get_command() == "CAP")
		return;
	if (client->get_auth() == false && cmd->get_command() != "PASS")
	{
		ERROR(client, "User not authenticated. Closing connection");
		server->remove_client(client->getFd(), "");
		return;
	}
	if (client->get_registered() == false &&  (cmd->get_command() != "PASS" && cmd->get_command() != "NICK" && cmd->get_command() != "USER"))
	{
		ERR_NOTREGISTERED(client);
		return;
	}
    if (cmd->get_command() == "NICK")
		nick(cmd, client, server);
	else if (cmd->get_command() == "USER")
		user(cmd, client, server);
	else if (cmd->get_command() == "PASS")
		pass(cmd, client, server);
    else if (cmd->get_command() == "PING")
		pong(cmd, client);
    else if (cmd->get_command() == "JOIN")
        join(cmd, client, server);
    else if (cmd->get_command() == "PRIVMSG")
        privmsg(cmd, client, server);
    else if (cmd->get_command() == "PART")
        part(cmd, client, server);
    else if (cmd->get_command() == "KICK")
        kick(cmd, client, server);
    else if (cmd->get_command() == "INVITE")
        invite(cmd, client, server);
    else if (cmd->get_command() == "TOPIC")
        topic(cmd, client, server);
    else if (cmd->get_command() == "MODE")
        mode(cmd, client, server);
    else if (cmd->get_command() == "QUIT")
        quit(cmd, client, server);
    // else if (cmd->get_command() == "AWAY")
    //     away(cmd, client);
    else
        ERR_UNKNOWNCOMMAND(client, cmd);
}

void CmdHandler::pong(Command *cmd, Client *client) {PONG(cmd, client);}

void CmdHandler::quit(Command *cmd, Client *client, Server *server)
{
    std::string message = ":Client Quit";
    if (!cmd->get_params().empty())
        message = cmd->get_params();
    server->remove_client(client->getFd(), message);
}