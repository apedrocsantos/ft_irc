#include "../../inc/main.hpp"

class CmdHandler;

std::string do_modes(std::string &flags, bool cur_mode, std::list<std::string> &etc, Command *cmd, Client *client, Channel *channel, Server *server)
{
    bool mode = true;
	std::string o_flags;
	std::string strings;

    for (int iterator = 0; iterator < (int)flags.size(); iterator++) // iterate flags
    {
        if (cur_mode && (flags[iterator] == 'l') && etc.empty()) // if +l without params
        {
            server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));
            continue;
        }
        if ((flags[iterator] == 'k' || flags[iterator] == 'o') && etc.empty()) // if +/- k/o without params
        {
            server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));
            continue;
        }
        if (flags[iterator] == '+') // mode +
            cur_mode = true;
        else if (flags[iterator] == '-') // mode -
            cur_mode = false;
        else if (flags[iterator] == 'i') // i(nvite) flag
        {
            if (channel->get_flag('i') != cur_mode)
            {
                channel->set_mode('i', cur_mode);
                if (mode != cur_mode)
                {
                    mode = cur_mode;
					cur_mode == true ? o_flags += '+': o_flags += '-';
                }
                o_flags += 'i';
            }
        }
        else if (flags[iterator] == 't') // t(opic) flag
        {
            if (channel->get_flag('t') != cur_mode)
            {
                channel->set_mode('t', cur_mode);
                if (mode != cur_mode)
                {
                    mode = cur_mode;
                    cur_mode == true ? o_flags += '+': o_flags += '-';
                }
                o_flags += 't';
            }
        }
        else if (flags[iterator] == 'k') // k (password) flag
        {
            if (cur_mode == true) // +k
            {
                channel->set_mode(flags[iterator], cur_mode); // set flag mode
                channel->set_key(etc.front()); // add/change key
                if (mode != cur_mode)
                {
                    mode = cur_mode;
                    o_flags += '+';
                }
                o_flags += 'k';
                strings += etc.front() + " ";
            }
            if (cur_mode == false && etc.front() == channel->get_key()) // -k and correct password
            {
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
        }
        else if (flags[iterator] == 'o') // o(perator) flag
        {
            Client *client_to_change = server->get_client(etc.front());
            if (client_to_change == NULL)
            {
                server->add_to_out_buf(client->getFd(), ERR_NOSUCHNICK(client, etc.front()));
                continue; 
            }
			if (!channel->is_member(client_to_change->getNick()))
			{
				server->add_to_out_buf(client->getFd(), ERR_USERNOTINCHANNEL(client, etc.front(), channel));
                continue;
			}
            if (cur_mode == true) // +o
            {
                if (channel->is_member(etc.front()) && !channel->is_operator(client_to_change->getFd())) // if user to change is member of channel and is not operator
                {
                    channel->set_operator(client_to_change->getFd()); // set operator
                    if (mode != cur_mode)
                        o_flags += '+';
                }
            }
            else // -o
            {
                if (channel->is_operator(client_to_change->getFd()))
                {
                    channel->remove_operator(client_to_change->getFd());
                    if (mode != cur_mode)
                        o_flags += '-';
                }
            }
			o_flags += 'o';
            strings += etc.front() + " ";
			etc.pop_front();
        }
        else if (flags[iterator] == 'l') // l(imit) flag
        {
            if (cur_mode == true) // +l
            {
                channel->set_mode(flags[iterator], cur_mode); // set flag mode
				if (std::atoi(etc.front().c_str()) == 0) // if invalid value
				{
					etc.pop_front();
					continue;
				}
                channel->set_limit(std::atoi(etc.front().c_str())); // set/change limit
                strings += ft_itoa(channel->get_limit()) + " ";
                etc.pop_front();
                if (mode != cur_mode)
                {
                    mode = cur_mode;
                    o_flags += '+';
                }
                o_flags += 'l';
            }
            else if (cur_mode == false && channel->get_flag('l') == true)// -l
            {
				channel->set_mode(flags[iterator], cur_mode); // set flag mode
                if (mode != cur_mode)
                {
                    mode = cur_mode;
                    o_flags += '-';
                }
                o_flags += 'l';
            }
        }
    }
	if (!o_flags.empty() && o_flags[0] != '-')
        o_flags.insert(0, "+");
	if (!o_flags.empty())
		return o_flags + " " + strings;
	return "";
}

void CmdHandler::mode(Command *cmd, Client *client, Server *server)
{
    int iterator;
    std::map<std::string, bool> modes;
    bool cur_mode = true;
    std::string itkol = "itkol+-";
    std::string channel;
    std::string flags;
    std::string str;
    std::string output;
    std::list<std::string> etc;

	if (cmd->get_params().empty())
		return server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));
    std::stringstream ss(cmd->get_params());
    std::getline(ss, channel, ' ');
    if (!server->channel_exists(channel)) // check if channel exists
        return server->add_to_out_buf(client->getFd(), ERR_NOSUCHCHANNEL(client, channel));
    Channel *channel_class = server->get_channel(channel);
    std::getline(ss, flags, ' '); // get flags
    if (flags.empty())
        return server->add_to_out_buf(client->getFd(), RPL_CHANNELMODEIS(client, channel_class));
    iterator = 0;
    while (flags[iterator]) // check if flag is valid
    {
        if (itkol.find(flags[iterator]) == std::string::npos)
            return server->add_to_out_buf(client->getFd(), ERR_UMODEUNKNOWNFLAG(client, flags[iterator]));
        iterator++;
    }
    if (!channel_class->is_member(client->getNick())) // check if user is not member of channel
        return server->add_to_out_buf(client->getFd(), ERR_NOTONCHANNEL(client, channel));
    if (!channel_class->is_operator(client->getFd())) // check if user is not op
        return server->add_to_out_buf(client->getFd(), ERR_CHANOPRIVSNEEDED(client, channel_class));
    iterator = 3;
    while (std::getline(ss, str, ' ') && iterator--) // get remaining strings
        etc.push_back(str);
    output = do_modes(flags, cur_mode, etc, cmd, client, channel_class, server);
    if (!output.empty())
        for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = channel_class->get_members_begin(); it_members != channel_class->get_members_end(); it_members++)
            server->add_to_out_buf(it_members->second->getFd(), MODE(client, channel_class, output));
}