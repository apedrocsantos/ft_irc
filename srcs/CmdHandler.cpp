#include "../inc/CmdHandler.hpp"

std::set<std::string> usedNicknames;

CmdHandler::CmdHandler(Command *cmd, Client *client, Server *server)
{
    (void) cmd;
    (void) client;
    (void) server;
    std::string password = "pwd";
	std::cout << cmd->get_command() << std::endl;
	if (cmd->get_command() == "NICK")
		nick(cmd, client);
	if (cmd->get_command() == "USER")
		user(cmd, client, server);
	if (cmd->get_command() == "PASS")
		pass(cmd, client);
    /* if (cmd->get_command() == "QUIT") */
}

bool checkFirstNumbSymbol(const std::string &str) {
	char firstChar = str[0];
	return std::isdigit(firstChar) || std::ispunct(firstChar);
}

void CmdHandler::nick(Command *cmd, Client *client) {
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string newNick;

	// Remove the first word and store it in newNick
	ss >> newNick;

	// check if the first char of the nick is a number or symbol

	if (checkFirstNumbSymbol(newNick)) return ERR_ErroneusNickName(client);

	if (newNick.empty()) return ERR_NoNicknameGiven(client);

	if (usedNicknames.find(newNick) != usedNicknames.end()) {
		std::cout << "Error: Nickname is already in use." << std::endl;
		return ERR_NickNameInUse(client);
	}

	// Remove the old nickname from the used set
	usedNicknames.erase(client->getNick());

	if (client->getOldNick().empty()) client->setOldNick(newNick);
	else client->setOldNick(client->getNick());
	client->setNick(newNick);
	RPL_Nick(client);

	// Add the new nickname to the used set
	usedNicknames.insert(newNick);

	std::cout << "params: " << params << " new nickinC: " << client->getNick() <<  std::endl;
	// :justshush!~justshush@<IP> NICK :justshush4
	// :<OLDNICK/CURRENTNICK>!~<user>@<IP> NICK :<NEWNICK>

	// JOIN  :<OLDNICK>!~<USER>@<IP> JOIN #world
}


// Function to split a string by spaces
std::vector<std::string> splitString(const std::string& str) {
	std::vector<std::string> result;
	std::istringstream iss(str);
	std::string token;

	while (iss >> token) {
		result.push_back(token);
	}
return result;
}

// Function to check if the string contains all the correct parameters
bool hasCorrectParams(const std::string& str) {
	std::vector<std::string> parameters = splitString(str);

	// Check if there are exactly four parameters
	if (parameters.size() != 5) return false;

	// checks if the realname starts with the ":"
	if (parameters[3][0] != ':') return false;

	parameters[3] = parameters[3].substr(1);

	return true;
}

void CmdHandler::user(Command *cmd, Client *client, Server *server) {
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string username;
	
	ss >> username;

	// if the user is already registered
	if (!client->getUsername().empty()) return ERR_AlreadyRegistered(client);

	// if there are missing parameters
	if (!hasCorrectParams(params)) return ERR_NeedMoreParams(cmd, client);

	client->setUser(username);

	(void)client;
	(void)server;
}

void pass(Command *cmd, Client *client) {
	
}

// pass primeiro cmd, para verificar a pass
// por na espera na fd poll e so depois mandar para a client se a pass for correta
// void CmdHandler::pass(std::string params, class Client *client)
// {
//     if (params.empty())
//         send_msg("461 [nickname] :Please provide a password\r\n", client);
//     else if ((int)params.find(32) != -1 || params.empty() || params.compare("pass"))
//         send_msg("464 [nickname] :Wrong password\r\n", client);
// }

// void CmdHandler::send_msg(std::string msg, class Client *client)
// {
//     int sent_length;
//     int len = msg.length();

//     std::cout << "SENDING msg\n";
//     sent_length = send(client->getFd(), msg.c_str(), len, 0);
//     if (sent_length != len)
//         std::cerr << "couldn't send complete msg\n";
//     if (sent_length == -1)
//         std::cerr << "send ERROR\n";
//     if (sent_length != len)
//         std::cout << "ERROR: Couldn't send full message.\n";
// }