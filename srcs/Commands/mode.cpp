#include "../../inc/main.hpp"

class CmdHandler;

void do_modes(std::string &flags, std::string &o_flags, std::string &strings, bool cur_mode, std::list<std::string> &etc, Command *cmd, Client *client, Channel *channel, Server *server)
{
    bool mode = true;

    for (int iterator = 0; iterator < (int)flags.size(); iterator++) // iterate flags
    {
        if (cur_mode && etc.empty() && (flags[iterator] == 'l'))
        {
            ERR_NEEDMOREPARAMS(cmd, client);
            continue;
        }
        if (etc.empty() && (flags[iterator] == 'k' || flags[iterator] == 'o'))
        {
            ERR_NEEDMOREPARAMS(cmd, client);
            continue;
        }
        switch(flags[iterator])
        {
            case '+':
            {
                cur_mode = true;
                break;
            }
            case '-':
            {
                cur_mode = false;
                break;
            }
            case 'i':
            {
                if (channel->get_flag('i') != cur_mode)
                {
                    channel->set_mode('i', cur_mode);
                    if (mode != cur_mode)
                    {
                        mode = cur_mode;
                        if (cur_mode == true)
                            o_flags += '+';
                        else
                            o_flags += '-';
                    }
                    o_flags += 'i';
                }
                break;
            }
            case 't':
            {
                if (channel->get_flag('t') != cur_mode)
                {
                    channel->set_mode('t', cur_mode);
                    if (mode != cur_mode)
                    {
                        mode = cur_mode;
                        if (cur_mode == true)
                            o_flags += '+';
                        else
                            o_flags += '-';
                    }
                    o_flags += 't';
                }
                break;
            }
            case 'k':
            {
                if (cur_mode == true) // +k flag
                {
                    channel->set_key(etc.front()); // add/change key
                    channel->set_mode(flags[iterator], cur_mode); // set flag mode
                    if (mode != cur_mode)
                    {
                        mode = cur_mode;
                        o_flags += '+';
                    }
                    o_flags += 'k';
                    strings += etc.front() + " ";
                }
                if (cur_mode == false && etc.front() == channel->get_key())
                {
                    channel->set_mode('k', false); // set -k
                    channel->set_mode(flags[iterator], cur_mode); // set flag mode
                    if (mode != cur_mode)
                    {
                        mode = cur_mode;
                        o_flags += '-';
                    }
                    o_flags += 'k';
                    strings += "* ";
                }
                etc.pop_front();
                break;
            }
            case 'o':
            {
                Client *client_to_change = server->get_client(etc.front());
                if (client_to_change == NULL)
                {
                    ERR_NOSUCHNICK(client, etc.front());
                    continue; 
                }
                if (cur_mode == true) // +o
                {
                    if (channel->is_member(etc.front()) && !channel->is_operator(client_to_change->getFd())) // if user to change is member of channel and is not operator
                    {
                        channel->set_operator(client_to_change->getFd());
                        if (mode != cur_mode)
                            o_flags += '+';
                        o_flags += 'o';
                        strings += etc.front() + " ";
                    }
                }
                else // -o
                {
                    if (channel->is_operator(client_to_change->getFd()))
                    {
                        channel->remove_operator(client_to_change->getFd());
                        if (mode != cur_mode)
                            o_flags += '-';
                        o_flags += 'o';
                        strings += etc.front() + " ";
                    }
                }
                channel->set_mode(flags[iterator], cur_mode); // set flag mode
               break; 
            }
            case 'l':
            {
                bool flag = false;
                if (cur_mode == true) // +l
                {
                    for (int i = 0; i < (int)etc.front().size(); i++)
                        if (std::isdigit(etc.front()[i]) == false)
                        {
                            flag = true;
                            break;
                        }
                    if (flag)
                    {
                        etc.pop_front();
                        continue;
                    }
                    channel->set_limit(etc.front()); // set/change limit
                    channel->set_mode(flags[iterator], cur_mode); // set flag mode
                    strings += etc.front() + " ";
                    etc.pop_front();
                    if (mode != cur_mode)
                    {
                        mode = cur_mode;
                        if (cur_mode == true)
                            o_flags += '+';
                        else
                            o_flags += '-';
                    }
                    o_flags += 'l';
                }
                else // -l
                {
                    channel->set_mode(flags[iterator], cur_mode); // set flag mode
                    if (channel->get_flag('l') != cur_mode)
                    {
                        if (mode != cur_mode)
                        {
                            mode = cur_mode;
                            if (cur_mode == true)
                                o_flags += '+';
                            else
                                o_flags += '-';
                        }
                        o_flags += 'l';
                    }
                }
                break;
            }
        }
    }
}

void CmdHandler::mode(Command *cmd, Client *client, Server *server)
{
    int iterator;
    std::map<std::string, bool> modes;
    bool cur_mode = true;
    std::string itkol = "itkol+-";
    std::string channel;
    std::string flags;
    std::string o_flags;
    std::string str;
    std::string output;
    std::string strings;
    std::list<std::string> etc;

    std::stringstream ss(cmd->get_params());
    std::getline(ss, channel, ' ');
    if (!server->channel_exists(channel)) // check if channel exists
    {
        ERR_NOSUCHCHANNEL(client, channel);
        return;
    }
    Channel *channel_class = server->get_channel(channel);
    std::getline(ss, flags, ' '); // get flags
    if (flags.empty())
    {
        RPL_CHANNELMODEIS(client, channel_class);
        return;
    }
    iterator = 0;
    while (flags[iterator]) // check if flag is valid
    {
        if (itkol.find(flags[iterator]) == std::string::npos)
        {
            ERR_UMODEUNKNOWNFLAG(client, flags[iterator]);
            return;
        }
        iterator++;
    }
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
    iterator = 3;
    while (std::getline(ss, str, ' ') && iterator--) // get remaining strings
        etc.push_back(str);
    do_modes(flags, o_flags, strings, cur_mode, etc, cmd, client, channel_class, server);
    if (!o_flags.empty() && o_flags[0] != '-')
        o_flags.insert(0, "+");
    if (!o_flags.empty())
    {
        output = o_flags + " " + strings;
        for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = channel_class->get_members_begin(); it_members != channel_class->get_members_end(); it_members++)
            MODE(client, channel_class, output, it_members->second);
    }
}