#include "../../inc/main.hpp"

class CmdHandler;

bool checkFirstNumbSymbol(const std::string &str) {
	char firstChar = str[0];
	return std::isdigit(firstChar) || std::ispunct(firstChar);
}

void CmdHandler::nick(Command *cmd, Client *client, Server *server) {
	std::string params = cmd->get_params();
	std::vector<std::string> paramsArray;
	std::stringstream ss(params);
	std::string word, newNick;
	std::vector<int> fd_list;

	while (ss >> word)
		paramsArray.push_back(word);

	if (paramsArray.empty())
		return server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));

	newNick = paramsArray[0];

	// check if the first char of the nick is a number or symbol
	if (checkFirstNumbSymbol(newNick))
		return server->add_to_out_buf(client->getFd(), ERR_ErroneusNickName(client));

	if (newNick.empty())
		return server->add_to_out_buf(client->getFd(), ERR_NoNicknameGiven(client));

	if (client->getNick() == newNick) return ;

	if (server->usedNicknames.find(newNick) != server->usedNicknames.end())
		return server->add_to_out_buf(client->getFd(), ERR_NickNameInUse(client));

	// Remove the old nickname from the used set
	server->usedNicknames.erase(client->getNick());

	if (client->getOldNick().empty()) client->setOldNick(newNick);
	else client->setOldNick(client->getNick());
	client->setNick(newNick);
	if (client->get_channels().empty())
		server->add_to_out_buf(client->getFd(), Nick(client));
	// send message to all clients on every channel the client belongs to
	for (std::vector<std::string>::iterator it = client->get_channels_begin(); it != client->get_channels_end(); it++)
	{
		Channel *chan = server->get_channel(*it);
		if (chan != NULL)
		{
			for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = chan->get_members_begin(); it_members != chan->get_members_end(); it_members++)
			{
				if (std::find(fd_list.begin(), fd_list.end(), it_members->second->getFd()) == fd_list.end())
				{
					server->add_to_out_buf(it_members->second->getFd(), Nick(client));
					fd_list.push_back(it_members->second->getFd());
				}
			}
		}
	}
	if (!(client->getNick().empty() || client->getUsername().empty() || client->getRealname().empty()))
	{
		if (client->get_registered() == false)
		{
			client->set_registered(true);
			server->add_to_out_buf(client->getFd(), RPL_WELCOME(client));
		}
	}

	// Add the new nickname to the used set
	server->usedNicknames.insert(newNick);
}