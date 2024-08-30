#include "../../inc/main.hpp"

class CmdHandler;

// TODO: RPL_AWAY)
// TODO: colliding info: RFCs vs modern irc
void CmdHandler::invite(Command *cmd, Client *client, Server *server)
{
    std::string channel;
    std::string str;
    std::string user_to_invite;

    std::stringstream ss(cmd->get_params());
    std::getline(ss, user_to_invite, ' '); // get user to invite
    if (server->get_client(user_to_invite) == NULL)
    {
        ERR_NOSUCHNICK(client, user_to_invite);
        return;
    }
    std::getline(ss, channel, ' '); // get channel
    Client *client_to_invite = server->get_client(user_to_invite);
    std::map<std::string, class Channel *> list = server->get_channel_list();
    if (!server->channel_exists(channel)) // check if channel exists
    {
        INVITE(client, channel, user_to_invite, client_to_invite);
        RPL_INVITING(client, user_to_invite, channel);
        return;
    }
    if (!list.at(channel)->is_member(client->getNick())) //Check if user is member of chan
    {
        ERR_NOTONCHANNEL(client, channel);
        return ;
    }
    if (list.at(channel)->get_flag('i') && !list.at(channel)->is_operator(client->getFd())) //Check if flag +i and user is op in chan
    {
        ERR_CHANOPRIVSNEEDED(client, list.at(channel));
        return ;
    }
    if (list.at(channel)->is_member(user_to_invite)) //check if user to be invited is on channel
    {
        ERR_USERONCHANNEL(client, user_to_invite, list.at(channel));
        return ;
    }
    INVITE(client, channel, user_to_invite, client_to_invite);
    RPL_INVITING(client, user_to_invite, channel);
    list.at(channel)->add_to_invited(client_to_invite->getFd());
}