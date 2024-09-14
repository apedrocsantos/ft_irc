#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::topic(Command *cmd, Client *client, Server *server)
{
    std::string channel;
    std::string str;
    std::string topic;
	std::map<std::string, class Channel *> list = server->get_channel_list();

	if (cmd->get_params().empty())
		return server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client)); 
    std::stringstream ss(cmd->get_params());
    std::getline(ss, channel, ' '); // get channel
	ss >> std::ws;
    std::getline(ss, topic); // get topic
    if (!server->channel_exists(channel)) // check if channel doesn't exist
		return server->add_to_out_buf(client->getFd(), ERR_NOSUCHCHANNEL(client, channel));
	Channel *chan = server->get_channel(channel);
    if (!list.at(channel)->is_member(client->getNick())) //Check if user is not member of chan
		return server->add_to_out_buf(client->getFd(), ERR_NOTONCHANNEL(client, channel));
    if (list.at(channel)->get_flag('t') == true && !topic.empty() && list.at(channel)->is_operator(client->getFd()) == false) //Check if +t && user is not op in chan
		return server->add_to_out_buf(client->getFd(), ERR_CHANOPRIVSNEEDED(client, list.at(channel)));
	if (topic.empty() && list.at(channel)->get_topic().empty()) // check if topic is empty and received message has no topic
		return server->add_to_out_buf(client->getFd(), RPL_NOTOPIC(client, list.at(channel)));
	if (topic == ":" || topic == "::") // set to no topic
	{
		list.at(channel)->set_topic(topic.erase());
		for (std::list<std::pair<std::string*, class Client *> >::iterator it = chan->get_members_begin(); it != chan->get_members_end(); it++)
			server->add_to_out_buf(it->second->getFd(), RPL_TOPIC(client, list.at(channel)));
		return;
	}
	if (topic.empty()) // check if topic empty
		return server->add_to_out_buf(client->getFd(), RPL_TOPIC(client, list.at(channel)));
	if (!topic.empty() && topic[0] != ':')
		topic.insert(topic.begin(), ':');
    list.at(channel)->set_topic(topic); // set topic
	for (std::list<std::pair<std::string*, class Client *> >::iterator it = chan->get_members_begin(); it != chan->get_members_end(); it++)
		server->add_to_out_buf(it->second->getFd(), RPL_TOPIC(client, list.at(channel)));
}