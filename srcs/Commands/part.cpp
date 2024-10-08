#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::part(Command *cmd, Client *client, Server *server)
{
    std::vector<std::string> names;
    std::string str;
    std::string message = "ircserv";

	if (cmd->get_params().empty())
		return server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));
    std::stringstream ss(cmd->get_params());
    ss >> str;
    std::stringstream ss2(str);
	while (std::getline(ss2, str, ',')) // get channels
		names.push_back(str);
	ss >> std::ws;
    if (std::getline(ss, str)) // get message
		message = str;
	if (!message.empty() && message[0] != ':')
		message.insert(message.begin(), ':');
    std::map<std::string, class Channel *> list = server->get_channel_list();
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
    {
        if (list.find(*it) == list.end()) // check if channel doesn't exist
            return server->add_to_out_buf(client->getFd(), ERR_NOSUCHCHANNEL(client, *it));
        if (list.at(*it)->member_exists(client->getNick())) // check if user is member of channel
        {
            for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = list[*it]->get_members_begin(); it_members != list[*it]->get_members_end(); it_members++)
                server->add_to_out_buf(it_members->second->getFd(), PART(client, list.at(*it), message)); // send part to all members of channel
            client->remove_channel(*it); // remove channel from client channel list
            list.at(*it)->remove_member(client->getNick()); // remove client from channel member list
            list.at(*it)->remove_operator(client->getFd()); // remove client from channel op list
            if (list.at(*it)->get_nb_users() == 0) // delete channel if empty
                server->remove_channel(*it);
        }
        else
            server->add_to_out_buf(client->getFd(), ERR_NOTONCHANNEL(client, *it));
    }
}