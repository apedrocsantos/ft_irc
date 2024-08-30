#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::kick(Command *cmd, Client *client, Server *server)
{
    std::string channel;
    std::string user;
    std::string str;
    std::string message = "ircserv";
    std::vector<std::string> users_to_kick;

    std::stringstream ss(cmd->get_params());
    std::getline(ss, str, ' '); // get channel
    channel = str;
    std::getline(ss, str, ' ');
    std::stringstream ss2(str);
    while (std::getline(ss2, user, ',')) // get users
        users_to_kick.push_back(user);
    if (users_to_kick.empty())
    {
        ERR_NEEDMOREPARAMS(cmd, client);
        return ;
    }
    std::getline(ss, message); // get message
    for (std::vector<std::string>::iterator it_user = users_to_kick.begin(); it_user != users_to_kick.end(); it_user++)
    {
        Client *client_to_kick = server->get_client(*it_user);
        std::map<std::string, class Channel *> list = server->get_channel_list();
        if (list.find(channel) == list.end()) // check if channel exists
        {
            ERR_NOSUCHCHANNEL(client, channel);;
            return;
        }
        if (!list.at(channel)->is_member(client->getNick())) //Check if user is member of chan
        {
            ERR_NOTONCHANNEL(client, channel);
            continue ;
        }
        if (!list.at(channel)->is_operator(client->getFd())) //Check if user is op in chan
        {
            ERR_CHANOPRIVSNEEDED(client, list.at(channel));
            continue ;
        }
        if (!list.at(channel)->is_member(*it_user)) //check if user to be kicked is in channel
        {
            ERR_USERNOTINCHANNEL(client, *it_user, list.at(channel));
            continue ;
        }
        // kick user and send msg to all members of channel
        for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = list[channel]->get_members_begin(); it_members != list[channel]->get_members_end(); it_members++)
            KICK(client, list.at(channel), *it_user, message, it_members->second);
        client_to_kick->remove_channel(channel); // remove channel from client's list
        list.at(channel)->remove_member(client_to_kick->getNick()); // remove client from channel members
        list.at(channel)->remove_operator(client_to_kick->getFd()); // remove client from channel operators
        if (list.at(channel)->get_nb_users() == 0) // if channel is empty, remove channel
            server->remove_channel(channel);
    }
}