#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::privmsg(Command *cmd, Client *client, Server *server)
{
    // int index = 0;
    (void) client;
    (void) server;
    std::list<std::string> targets;
    std::string message;

    std::stringstream ss(cmd->get_params());
    std::string str;
    ss >> str;
    std::stringstream ss2(str);
    while (std::getline(ss2, str, ',')) // get targets
            targets.push_back(str);
    std::getline(ss, message); // get message to send

    if (message.empty())
    {
        ERR_NOTEXTTOSEND(client);
        return;
    }
    while (!targets.empty())
    {
        if (targets.front()[0] == '#' || targets.front()[0] == '$') // if target is a channel
        {
            Channel *target = server->get_channel(targets.front());
            if (target == NULL)
                ERR_NOSUCHNICK(client, targets.front());
            else
            {
                for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = target->get_members_begin(); it_members != target->get_members_end(); it_members++)
                    PRIVMSG(client, targets.front(), message, it_members->second);
            }
        }
        else // if target is a user
        {
            Client *c_target = server->get_client(targets.front());
            if (c_target == NULL)
                ERR_NOSUCHNICK(client, targets.front());
            else
                PRIVMSG(client, targets.front(), message, c_target);
        }
        targets.pop_front();
    }
}