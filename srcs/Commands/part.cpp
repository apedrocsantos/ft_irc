#include "../../inc/main.hpp"

class CmdHandler;

// TODO: Check full message?
void CmdHandler::part(Command *cmd, Client *client, Server *server)
{
    std::vector<std::string> names;
    std::string message = "ircserv";

    std::stringstream ss(cmd->get_params());
    std::string str;
    ss >> str;
    std::stringstream ss2(str);
    while (std::getline(ss2, str, ',')) // get channels
        names.push_back(str);
    if (std::getline(ss, str)) // get message
        message = str;

    std::map<std::string, class Channel *> list = server->get_channel_list();
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
    {
        if (list.find(*it) == list.end()) // check if channel exists
        {
            ERR_NOSUCHCHANNEL(client, *it);;
            return;
        }
        if (list.at(*it)->member_exists(client->getNick())) // check if user is member of channel
        {
            for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = list[*it]->get_members_begin(); it_members != list[*it]->get_members_end(); it_members++)
                PART(client, list.at(*it), message, it_members->second); // send part to all members of channel
            client->remove_channel(*it); // remove channel from client channel list
            list.at(*it)->remove_member(client->getNick()); // remove client from channel member list
            list.at(*it)->remove_operator(client->getFd()); // remove client from channel op list
            if (list.at(*it)->get_nb_users() == 0) // delete channel if empty
                server->remove_channel(*it);
        }
        else
            ERR_NOTONCHANNEL(client, *it);
    }
}