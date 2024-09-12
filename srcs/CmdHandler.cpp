#include "../inc/main.hpp"

CmdHandler::CmdHandler(Command *cmd, Client *client, Server *server)
{
	if (!client)
		return;
	if (cmd->get_command() == "QUIT")
	{
        quit(cmd, client, server);
		return;
	}
    if (cmd->get_command() == "CAP")
		return;
	if (client->get_auth() == false && cmd->get_command() != "PASS")
	{
		server->add_to_out_buf(client->getFd(),ERROR("User not authenticated. Closing connection"));
		client->set_disconnect(true);
		return;
	}
	if (client->get_registered() == false &&  (cmd->get_command() != "PASS" && cmd->get_command() != "NICK" && cmd->get_command() != "USER"))
	{
		server->add_to_out_buf(client->getFd(),ERR_NOTREGISTERED(client));
		return;
	}
    if (cmd->get_command() == "NICK")
		nick(cmd, client, server);
	else if (cmd->get_command() == "USER")
		user(cmd, client, server);
	else if (cmd->get_command() == "PASS")
		pass(cmd, client, server);
    else if (cmd->get_command() == "PING")
		pong(cmd, client, server);
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
    else if (cmd->get_command() == "AWAY")
        away(cmd, client, server);
    else
        server->add_to_out_buf(client->getFd(),ERR_UNKNOWNCOMMAND(client, cmd));
}

void CmdHandler::pong(Command *cmd, Client *client, Server *server) {server->add_to_out_buf(client->getFd(),PONG(cmd));}

void CmdHandler::quit(Command *cmd, Client *client, Server *server)
{
    std::string message = "Client Quit";
    if (!cmd->get_params().empty())
        message = cmd->get_params();
    server->remove_client(client->getFd(), message);
}