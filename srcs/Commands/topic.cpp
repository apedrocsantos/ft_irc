#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::topic(Command *cmd, Client *client, Server *server)
{
    std::string channel;
    std::string str;
    std::string topic;

    std::stringstream ss(cmd->get_params());
    std::getline(ss, channel, ' '); // get channel
    std::getline(ss, topic); // get topic
	if (topic[0] != ':')
		topic.insert(topic.begin(), ':');

    std::map<std::string, class Channel *> list = server->get_channel_list();
    if (!server->channel_exists(channel)) // check if channel exists
        return server->add_to_out_buf(client->getFd(), ERR_NOSUCHCHANNEL(client, channel));
    if (!list.at(channel)->is_member(client->getNick())) //Check if user is member of chan
        return server->add_to_out_buf(client->getFd(), ERR_NOTONCHANNEL(client, channel));
    if (list.at(channel)->get_flag('t') && !list.at(channel)->is_operator(client->getFd())) //Check if +t && user is op in chan
        return server->add_to_out_buf(client->getFd(), ERR_CHANOPRIVSNEEDED(client, list.at(channel)));
    if (topic.empty() && list.at(channel)->get_topic().empty()) // check if topic is empty and received message has no topic
        return server->add_to_out_buf(client->getFd(), RPL_NOTOPIC(client, list.at(channel)));
    if (topic.empty()) // check if topic empty
        return server->add_to_out_buf(client->getFd(), RPL_TOPIC(client, list.at(channel)));
    list.at(channel)->set_topic(topic); // set topic
}