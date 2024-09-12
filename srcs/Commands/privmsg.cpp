#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::privmsg(Command *cmd, Client *client, Server *server)
{
    std::list<std::string> targets;
    std::string message;

    std::stringstream ss(cmd->get_params());
    std::string str;
    ss >> str;
    std::stringstream ss2(str);
    while (std::getline(ss2, str, ',')) // get targets
            targets.push_back(str);
    std::getline(ss, message); // get message to send

	while (message[0] == ' ')
		message.erase(0, message.size() - 1);
    if (message.empty())
        return server->add_to_out_buf(client->getFd(), ERR_NOTEXTTOSEND(client));
	else if (message[0] != ':')
		message.insert(message.begin(), ':');
    while (!targets.empty())
    {
        if (targets.front()[0] == '#' || targets.front()[0] == '$') // if target is a channel
        {
            Channel *target = server->get_channel(targets.front());
            if (target == NULL)
                server->add_to_out_buf(client->getFd(), ERR_NOSUCHNICK(client, targets.front()));
            else
            {
                for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = target->get_members_begin(); it_members != target->get_members_end(); it_members++)
					if (it_members->second->getFd() != client->getFd())
                    	server->add_to_out_buf(it_members->second->getFd(), PRIVMSG(client, targets.front(), message));
            }
        }
        else // if target is a user
        {
            Client *c_target = server->get_client(targets.front());
            if (c_target == NULL)
                server->add_to_out_buf(client->getFd(), ERR_NOSUCHNICK(client, targets.front()));
            else
			{
                server->add_to_out_buf(c_target->getFd(), PRIVMSG(client, targets.front(), message));
				if (c_target->get_away() == true)
        			server->add_to_out_buf(client->getFd(), RPL_AWAY(c_target, client));
			}
        }
        targets.pop_front();
    }
}