#include "../../inc/main.hpp"

class CmdHandler;

//TODO: invalid modes aren't blocked
//TODO: only send MODE message if valid command

void CmdHandler::mode(Command *cmd, Client *client, Server *server)
{
    int iterator;
    std::map<std::string, bool> modes;
    bool cur_mode = true;
    std::string itkol = "itkol";
    std::string channel;
    std::string flags;
    std::string str;
    std::string output;
    std::list<std::string> etc;

    std::stringstream ss(cmd->get_params());
    std::getline(ss, channel, ' ');
    if (!server->channel_exists(channel)) // check if channel exists
    {
        ERR_NOSUCHCHANNEL(client, channel);
        return;
    }
    Channel *channel_class = server->get_channel(channel);
    if (!channel_class->is_member(client->getNick())) // check if user is member of channel
    {
        ERR_NOTONCHANNEL(client, channel);
        return;
    }
    if (!channel_class->is_operator(client->getFd())) // check if user is op
    {
        ERR_CHANOPRIVSNEEDED(client, channel_class);
        return;
    }
    std::getline(ss, flags, ' '); // get flags
    if (flags.empty())
    {
        RPL_CHANNELMODEIS(client, channel_class);
        return;
    }
    
    iterator = 3;
    while (std::getline(ss, str, ' ') && iterator--) // get remaining strings
        etc.push_back(str);

    output = channel + " " + flags + " ";
    for (iterator = 0; iterator < (int)flags.size(); iterator++) // iterate flags
    {
        if (flags[iterator] == '+')
            cur_mode = true;
        if (flags[iterator] == '-')
            cur_mode = false;
        if (itkol.find(flags[iterator]) == std::string::npos) // check if flag is valid
        {
            // Invalid flag;
            continue;
        }
        if (cur_mode && etc.empty() && (flags[iterator] == 'l' || flags[iterator] == 'o'))
        {
            ERR_NEEDMOREPARAMS(cmd, client);
            continue;
        }
        if (etc.empty() && (flags[iterator] == 'k'))
        {
            ERR_NEEDMOREPARAMS(cmd, client);
            continue;
        }
        if (flags[iterator] == 'i' || flags[iterator] == 't')
            channel_class->set_mode(flags[iterator], cur_mode); // set flag mode
        if (flags[iterator] == 'k' && cur_mode) // +k flag
        {
            channel_class->set_key(etc.front()); // add/change key
            channel_class->set_mode(flags[iterator], cur_mode); // set flag mode
        }
        if (flags[iterator] == 'k' && !cur_mode && etc.front() == channel_class->get_key()) // -k flag
        {
            channel_class->set_mode('k', false); // set -k
            channel_class->set_mode(flags[iterator], cur_mode); // set flag mode
        }
        if (flags[iterator] == 'l' && cur_mode) // +l flag
        {
            channel_class->set_limit(etc.front()); // set/change limit
            channel_class->set_mode(flags[iterator], cur_mode); // set flag mode
        }
        if (flags[iterator] == 'o') // +/- o flag
        {
            if (channel_class->is_operator(client->getFd()))
            {
                Client *client_to_change = server->get_client(etc.front());
                if (client_to_change == NULL)
                {
                ERR_NOSUCHNICK(client, etc.front());
                continue; 
                }
                if (cur_mode == true)
                {
                    if (channel_class->is_member(etc.front()) && !channel_class->is_operator(client_to_change->getFd())) // if user to change is member of channel and is not operator
                        channel_class->set_operator(client_to_change->getFd());
                }
                else
                {
                    if (channel_class->is_operator(client_to_change->getFd()))
                        channel_class->remove_operator(client_to_change->getFd());
                }
                channel_class->set_mode(flags[iterator], cur_mode); // set flag mode
            }
        }
        if (flags[iterator] == 'k' || flags[iterator] == 'l' || flags[iterator] == 'o')
        {
            output += etc.front();
            etc.pop_front();
        }
    }
    for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = channel_class->get_members_begin(); it_members != channel_class->get_members_end(); it_members++)
        MODE(client, output, it_members->second);
}