#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::join(Command *cmd, Client *client, Server *server)
{
    int index = 0;
    std::vector<std::string> names;
    std::vector<std::string> passwords;

    std::stringstream ss(cmd->get_params());
    std::string str;
    ss >> str;
    std::stringstream ss2(str);
    while (std::getline(ss2, str, ',')) // get channels
    {
        if ((str[0] == '#' || str[0] == '&') && ((str.find(0x20) == std::string::npos) && (str.find(0x07) == std::string::npos) && (str.find(0x2c) == std::string::npos)))
            names.push_back(str);
        else
            server->add_to_out_buf(client->getFd(),ERR_NOSUCHCHANNEL(client, str));
    }
    if(ss >> str) // get passwords
    {
        std::stringstream ss3(str);
        while (std::getline(ss3, str, ','))
            passwords.push_back(str);
    }

    std::map<std::string, class Channel *> list = server->get_channel_list();
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++) // iterate channel names
    {
        if (list.find(*it) != list.end()) // if channel exists
        {
            if (list[*it]->is_member(client->getNick())) // if nick is already a member, skip
                continue ;
        }
        else // create channel
        {
            server->add_channel(*it, new Channel (*it));
            list = server->get_channel_list();
            list[*it]->set_operator(client->getFd()); // add nick to ops
        }
        if (list[*it]->get_flag('l') == true && list[*it]->get_user_limit() != -1 && list[*it]->get_nb_users() >= list[*it]->get_user_limit()) // if +l and nb users == limit
            server->add_to_out_buf(client->getFd(),ERR_CHANNELISFULL(client, list[*it]));
        else if (list[*it]->get_flag('k') == true && ((int)passwords.size() <= index || list[*it]->get_key() != passwords[index])) // if +r and no or wrong password
            server->add_to_out_buf(client->getFd(),ERR_BADCHANNELKEY(client, list[*it]));
        else if (list[*it]->get_flag('i') == true && !list[*it]->is_invited(client->getFd())) // if +i and client not on invited list
            server->add_to_out_buf(client->getFd(), ERR_INVITEONLYCHAN(client, list[*it]));
        else
        {
            list[*it]->set_member(client); // add client to member list
            for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = list[*it]->get_members_begin(); it_members != list[*it]->get_members_end(); it_members++)
            	server->add_to_out_buf(it_members->second->getFd(), JOIN(client, list[*it])); // msg to all channel members with new member
            client->add_channel(*it); // add channel to member list of channels
            list[*it]->remove_from_invited(client->getFd()); // remove member from invited
            if (!list[*it]->get_topic().empty()) // display topic
                server->add_to_out_buf(client->getFd(),RPL_TOPIC(client, list[*it]));
            server->add_to_out_buf(client->getFd(),RPL_NAMREPLY(client, list[*it])); // show members
            server->add_to_out_buf(client->getFd(),RPL_ENDOFNAMES(client, list[*it]));
        }
        index++;
    }
}