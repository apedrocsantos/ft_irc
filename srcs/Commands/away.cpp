#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::away(Command *cmd, Client *client, Server *server)
{
    if (cmd->get_params().empty()) // if only AWAY is sent
    {
        client->set_away(false);
        return server->add_to_out_buf(client->getFd(), RPL_UNAWAY(client));
    }
    client->set_away(true); // if away msg exists
	std::string message = cmd->get_params();
	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);
    client->set_away_msg(message);
    server->add_to_out_buf(client->getFd(), RPL_NOAWAY(client));
}