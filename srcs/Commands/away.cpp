#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::away(Command *cmd, Client *client)
{
    if (cmd->get_params().empty())
    {
        client->set_away(false);
        RPL_UNAWAY(client);
        return;
    }
    client->set_away(true);
    client->set_away_msg(cmd->get_params());
    RPL_NOAWAY(client);
}