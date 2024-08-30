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

    std::map<std::string, class Channel *> list = server->get_channel_list();
    if (!server->channel_exists(channel)) // check if channel exists
    {
        ERR_NOSUCHCHANNEL(client, channel);
        return;
    }
    if (!list.at(channel)->is_member(client->getNick())) //Check if user is member of chan
    {
        ERR_NOTONCHANNEL(client, channel);
        return ;
    }
    if (list.at(channel)->get_flag('t') && !list.at(channel)->is_operator(client->getFd())) //Check if +t && user is op in chan
    {
        ERR_CHANOPRIVSNEEDED(client, list.at(channel));
        return ;
    }
    if (topic.empty() && list.at(channel)->get_topic().empty()) // check if topic is empty and received message has no topic
    {
        RPL_NOTOPIC(client, list.at(channel));
        return;
    }
    if (topic.empty()) // check if topic empty
    {
        RPL_TOPIC(client, list.at(channel));
        return;
    }
    list.at(channel)->set_topic(topic); // set topic
}