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
	if (paramsArray.empty()) // if no nickname
		return server->add_to_out_buf(client->getFd(), ERR_NoNicknameGiven(client));
	newNick = paramsArray[0];
	if (checkFirstNumbSymbol(newNick)) // check if the first char of the nick is a number or symbol
		return server->add_to_out_buf(client->getFd(), ERR_ErroneusNickName(client));
	if (client->getNick() == newNick) // if changing to same nick
		return ;
	if (server->usedNicknames.find(newNick) != server->usedNicknames.end()) // if nickame in use
		return server->add_to_out_buf(client->getFd(), ERR_NickNameInUse(client));
	server->usedNicknames.erase(client->getNick()); // Remove the old nickname from the used set

	if (client->getOldNick().empty())
		client->setOldNick(newNick);
	else
		client->setOldNick(client->getNick());
	client->setNick(newNick);
	if (client->get_channels().empty()) // if client not on any channel
		server->add_to_out_buf(client->getFd(), Nick(client));
	// send message to all clients on every channel the client belongs to
	for (std::vector<std::string>::iterator it = client->get_channels_begin(); it != client->get_channels_end(); it++)
	{
		Channel *chan = server->get_channel(*it);
		if (chan != NULL)
		{
			for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = chan->get_members_begin(); it_members != chan->get_members_end(); it_members++)
			{
				if (std::find(fd_list.begin(), fd_list.end(), it_members->second->getFd()) == fd_list.end()) // create a temp list of fds to which the the NICK message is sent
				{
					server->add_to_out_buf(it_members->second->getFd(), Nick(client));
					fd_list.push_back(it_members->second->getFd());
				}
			}
		}
	}
	if (!(client->getNick().empty() || client->getUsername().empty() || client->getRealname().empty())) // if user is registering for the first time
	{
		if (client->get_registered() == false)
		{
			client->set_registered(true);
			server->add_to_out_buf(client->getFd(), RPL_WELCOME(client));
		}
	}
	server->usedNicknames.insert(newNick); // Add the new nickname to the used set
}