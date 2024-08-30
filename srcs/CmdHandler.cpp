#include "../inc/main.hpp"

CmdHandler::CmdHandler(Command *cmd, Client *client, Server *server)
{
    if (cmd->get_command() == "NICK")
		nick(cmd, client, server);
	if (cmd->get_command() == "USER")
		user(cmd, client, server);
	if (cmd->get_command() == "PASS")
		pass(cmd, client, server);
    if (cmd->get_command() == "PING")
		pong(cmd, client);
    // if (cmd->get_command() == "QUIT")
    if (cmd->get_command() == "JOIN")
        join(cmd, client, server);
    if (cmd->get_command() == "MODE")
        mode(cmd, client, server);
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

	username = paramsArray[0];

	// if the user is already registered
	if (!client->getUsername().empty()) return ERR_AlreadyRegistered(client);

	// if there are missing parameters
	if (!hasCorrectParams(params)) return ERR_NeedMoreParams(cmd, client);

	client->setUser(username);
	client->setReal(paramsArray[3]);

	User(client);
	RPL_WELCOME(client);

	(void)server;
}

void CmdHandler::pass(Command *cmd, Client *client, Server *server) {
	(void) client;
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string pass;

	ss >> pass;

	if (server->get_pwd() != pass) std::cout << "some error idk, maybe ERR_PassWdMisMatch" << std::endl;

	// continue
	// send pass success
}

void CmdHandler::pong(Command *cmd, Client *client)
{
    if (cmd->get_params().empty())
        ERR_NeedMoreParams(cmd, client);
    else
        PONG(cmd, client);
}

void CmdHandler::join(Command *cmd, Client *client, Server *server)
{
    int index = 0;
    std::vector<std::string> names;
    std::vector<std::string> passwords;

    std::string params = cmd->get_params();

    if (params.empty())
    {
        ERR_NeedMoreParams(cmd, client);
        return ;
    }
    std::stringstream ss(cmd->get_params());
    std::string str;
    // get channels
    ss >> str;
    std::stringstream ss2(str);
    while (std::getline(ss2, str, ','))
    {
        if (str[0] == '#' || str[0] == '&')
        {
            if ((str.find(0x20) == std::string::npos) && (str.find(0x07) == std::string::npos) && (str.find(0x2c) == std::string::npos))
                names.push_back(str);
        }
        else
            ERR_NoSuChChannel(client, str);
    }
    // get passwords
    if(ss >> str)
    {
        std::stringstream ss3(str);
        while (std::getline(ss3, str, ','))
            passwords.push_back(str);
    }

    std::map<std::string, class Channel *> list = server->get_channel_list();
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
    {
        try
        {
            list.at(*it);
        }
        catch (const std::out_of_range& e)
        {
            server->add_channel(*it, new Channel (*it));
        }
        list = server->get_channel_list();
        if (list[*it]->get_user_limit() != -1 && list[*it]->get_nb_users() >= list[*it]->get_user_limit())
            ERR_ChannelIsFull(client, list[*it]);
        else if (((int)passwords.size() > index && list[*it]->get_key() != passwords[index]))
            ERR_BadChannelKey(client, list[*it]);
        else if (list[*it]->get_inviteOnly() == true)
            ERR_InviteOnlyChan(client, list[*it]);
        else
        {
            list[*it]->set_member(client);
            Join(client, list[*it]);
            if (!list[*it]->get_topic().empty())
                RPL_TOPIC(client, list[*it]);
            RPL_NAMREPLY(client, list[*it]);
            RPL_ENDOFNAMES(client, list[*it]);
        }
        index++;
    }
}

void CmdHandler::mode(Command *cmd, Client *client, Server *server)
{
    (void) cmd;
    (void) client;
    (void) server;
    std::cout << "MODE\n";
}