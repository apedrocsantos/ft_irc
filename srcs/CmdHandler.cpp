#include "../inc/main.hpp"

CmdHandler::CmdHandler(Command *cmd, Client *client, Server *server)
{
    if (cmd->get_command() == "CAP")
		return;
	if (client->get_auth() == false && cmd->get_command() != "PASS")
	{
		ERROR(client, "User not authenticated. Closing connection");
		server->remove_client(client->getFd(), "");
		return;
	}
	if (client->get_registered() == false &&  (cmd->get_command() != "PASS" && cmd->get_command() != "NICK" && cmd->get_command() != "USER"))
	{
		ERR_NOTREGISTERED(client);
		return;
	}
    else if (cmd->get_command() == "NICK")
		nick(cmd, client, server);
	else if (cmd->get_command() == "USER")
		user(cmd, client, server);
	else if (cmd->get_command() == "PASS")
		pass(cmd, client, server);
    else if (cmd->get_command() == "PING")
		pong(cmd, client);
    else if (cmd->get_command() == "JOIN")
        join(cmd, client, server);
    else if (cmd->get_command() == "PRIVMSG")
        privmsg(cmd, client, server);
    else if (cmd->get_command() == "PART")
        part(cmd, client, server);
    else if (cmd->get_command() == "KICK")
        kick(cmd, client, server);
    else if (cmd->get_command() == "INVITE")
        invite(cmd, client, server);
    else if (cmd->get_command() == "TOPIC")
        topic(cmd, client, server);
    else if (cmd->get_command() == "MODE")
        mode(cmd, client, server);
    else if (cmd->get_command() == "QUIT")
        quit(cmd, client, server);
    // else if (cmd->get_command() == "AWAY")
    //     away(cmd, client);
    else
        ERR_UNKNOWNCOMMAND(client, cmd);
}

bool checkFirstNumbSymbol(const std::string &str) {
	char firstChar = str[0];
	return std::isdigit(firstChar) || std::ispunct(firstChar);
}

void CmdHandler::nick(Command *cmd, Client *client, Server *server) {
	std::string params = cmd->get_params();
	std::vector<std::string> paramsArray;
	std::stringstream ss(params);
	std::string word, newNick;

	while (ss >> word)
		paramsArray.push_back(word);

	if (paramsArray.empty())
		return ERR_NEEDMOREPARAMS(cmd, client);

	newNick = paramsArray[0];

	// check if the first char of the nick is a number or symbol
	if (checkFirstNumbSymbol(newNick)) return ERR_ErroneusNickName(client);

	if (newNick.empty()) return ERR_NoNicknameGiven(client);

	if (server->usedNicknames.find(newNick) != server->usedNicknames.end()) {
		std::cout << "Error: Nickname is already in use." << std::endl;
		return ERR_NickNameInUse(client);
	}

	// Remove the old nickname from the used set
	server->usedNicknames.erase(client->getNick());

	if (client->getOldNick().empty()) client->setOldNick(newNick);
	else client->setOldNick(client->getNick());
	client->setNick(newNick);
	Nick(client);
	if (!(client->getNick().empty() || client->getUsername().empty() || client->getRealname().empty()))
	{
		if (client->get_registered() == false)
		{
			client->set_registered(true);
			RPL_WELCOME(client);
		}
	}

	// Add the new nickname to the used set
	server->usedNicknames.insert(newNick);
	// std::cout << "params: " << params << " new nickinC: " << client->getNick() <<  std::endl;
	// :justshush!~justshush@<IP> NICK :justshush4
	// :<OLDNICK/CURRENTNICK>!~<user>@<IP> NICK :<NEWNICK>
	// JOIN  :<OLDNICK>!~<USER>@<IP> JOIN #world
}

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
	if (parameters.size() != 4) return false;

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
	std::string word, username;

	while (ss >> word)
		paramsArray.push_back(word);

	if (paramsArray.empty())
		return ERR_NEEDMOREPARAMS(cmd, client);

	username = paramsArray[0];

	// if the user is already registered
	if (!client->getUsername().empty()) return ERR_AlreadyRegistered(client);

	// if there are missing parameters
	if (!hasCorrectParams(params)) return ERR_NEEDMOREPARAMS(cmd, client);

	client->setUser(username);
	client->setReal(paramsArray[3]);

	// User(client);
	if (!(client->getNick().empty() || client->getUsername().empty() || client->getRealname().empty()))
	{
		client->set_registered(true);
		RPL_WELCOME(client);
	}

	(void)server;
}

void CmdHandler::pass(Command *cmd, Client *client, Server *server) {
	(void) client;
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string pass;

	ss >> pass;

	if (client->get_auth() == false)
	{
		if (server->get_pwd() != pass)
			ERR_PASSWDMISMATCH(client);
		else
			client->set_auth(true);
	}
	else
		ERR_AlreadyRegistered(client);
}

void CmdHandler::pong(Command *cmd, Client *client) {PONG(cmd, client);}

void CmdHandler::quit(Command *cmd, Client *client, Server *server)
{
    std::string message = ":Client Quit";
    if (!cmd->get_params().empty())
        message = cmd->get_params();
    server->remove_client(client->getFd(), message);
}