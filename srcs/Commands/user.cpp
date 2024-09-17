#include "../../inc/main.hpp"

class CmdHandler;

// Function to split a string by spaces
std::vector<std::string> splitString(const std::string& str) {
	std::vector<std::string> result;
	std::istringstream ss(str);
	std::string token;

	while (ss >> token)
		result.push_back(token);

	return result;
}

// Function to check if the string contains all the correct parameters
bool hasCorrectParams(const std::string& str) {
	std::vector<std::string> parameters = splitString(str);

	// Check if there are exactly four parameters
	if (parameters.size() < 4) return false;

	// checks if the realname starts with the ":"
	if (parameters[3][0] != ':') return false;

	parameters[3] = parameters[3].substr(1);

	return true;
}

// USER username o * :realname
void CmdHandler::user(Command *cmd, Client *client, Server *server) {
	std::string params = cmd->get_params();
	std::vector<std::string> paramsArray;
	std::stringstream ss(params);
	std::string word, username, realname;

	while (ss >> word)
	{
		paramsArray.push_back(word);
		if (paramsArray.size() == 3)
		{
			std::getline(ss, realname);
			if (realname.size() > 0)
				paramsArray.push_back(realname);
			break;
		}
	}
	
	if (!client->getUsername().empty()) // if the user is already registered
		return server->add_to_out_buf(client->getFd(), ERR_AlreadyRegistered(client));

	if (paramsArray.empty())
		return server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));

	username = paramsArray[0];
	if (!hasCorrectParams(params))	// if there are missing parameters
		return server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));

	client->setUser(username);
	client->setReal(paramsArray[3]);

	if (!(client->getNick().empty() || client->getUsername().empty() || client->getRealname().empty()))
	{
		if (client->get_registered() == false)
		{
			client->set_registered(true);
			server->add_to_out_buf(client->getFd(), RPL_WELCOME(client));
		}
	}

}