#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::invite(Command *cmd, Client *client, Server *server)
{
    std::string channel;
    std::string user_to_invite;

	if (cmd->get_params().empty())
		return server->add_to_out_buf(client->getFd(),ERR_NEEDMOREPARAMS(cmd, client));
    std::stringstream ss(cmd->get_params());
    std::getline(ss, user_to_invite, ' '); // get user to invite
    if (server->get_client(user_to_invite) == NULL) // if user doesn't exist
        return server->add_to_out_buf(client->getFd(),ERR_NOSUCHNICK(client, user_to_invite));
    std::getline(ss, channel, ' '); // get channel
    Client *client_to_invite = server->get_client(user_to_invite);
    std::map<std::string, class Channel *> list = server->get_channel_list();
    if (!server->channel_exists(channel)) // if channel doesn't exist
		return server->add_to_out_buf(client->getFd(),ERR_NOSUCHCHANNEL(client, channel)); // only allow invite if channel exists
    if (!list.at(channel)->is_member(client->getNick())) //if user is not member of chan
        return server->add_to_out_buf(client->getFd(), ERR_NOTONCHANNEL(client, channel));
    if (list.at(channel)->get_flag('i') && !list.at(channel)->is_operator(client->getFd())) //if flag +i and user is not op in chan
        return server->add_to_out_buf(client->getFd(), ERR_CHANOPRIVSNEEDED(client, list.at(channel)));
    if (list.at(channel)->is_member(user_to_invite)) //if user to be invited is on channel
        return server->add_to_out_buf(client->getFd(), ERR_USERONCHANNEL(client, user_to_invite, list.at(channel)));
    server->add_to_out_buf(client_to_invite->getFd(), INVITE(client, channel, user_to_invite));
    server->add_to_out_buf(client->getFd(), RPL_INVITING(client, user_to_invite, channel));
    if (client_to_invite->get_away() == true)
        server->add_to_out_buf(client->getFd(), RPL_AWAY(client, client_to_invite));
    list.at(channel)->add_to_invited(client_to_invite->getFd());
}