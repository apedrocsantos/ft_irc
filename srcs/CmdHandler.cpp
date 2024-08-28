#include "../inc/main.hpp"

std::set<std::string> usedNicknames;

CmdHandler::CmdHandler(Command *cmd, Client *client, Server *server)
{
    if (cmd->get_command() == "NICK")
		nick(cmd, client);
	else if (cmd->get_command() == "USER")
		user(cmd, client, server);
	// else if (cmd->get_command() == "PASS")
	// 	pass(cmd, client);
    else if (cmd->get_command() == "PING")
		pong(cmd, client);
    else if (cmd->get_command() == "JOIN")
        join(cmd, client, server);
    else if (cmd->get_command() == "PART")
        part(cmd, client, server);
    else if (cmd->get_command() == "KICK")
        kick(cmd, client, server);
    else if (cmd->get_command() == "QUIT")
        quit(cmd, client, server);
    else if (cmd->get_command() == "MODE")
        mode(cmd, client, server);
    else
        ERR_UNKNOWNCOMMAND(client, cmd);
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
	//std::cout << "old nick: " << client->getNick() << std::endl;
	client->setNick(newNick);
	NICK(client);
	//std::string output = Nick(cmd, client);
	//send(client->getFd(), output.c_str(), output.size(), 0);

	// Add the new nickname to the used set
	usedNicknames.insert(newNick);
	// std::cout << "params: " << params << " new nickinC: " << client->getNick() <<  std::endl;
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
	if (parameters.size() != 4) return false;

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
	if (!hasCorrectParams(params)) return ERR_NEEDMOREPARAMS(cmd, client);

	client->setUser(username);

    RPL_WELCOME(client);

	(void)server;
}

// void CmdHandler::pass(Command *cmd, Client *client) {

// }

void CmdHandler::pong(Command *cmd, Client *client)
{
    if (cmd->get_params().empty())
        ERR_NEEDMOREPARAMS(cmd, client);
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
        ERR_NEEDMOREPARAMS(cmd, client);
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
            ERR_NOSUCHCHANNEL(client, str);
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
            if (list[*it]->is_member(client->getNick()))
                continue ;
        }
        catch (const std::out_of_range& e)
        {
            server->add_channel(*it, new Channel (*it));
            list = server->get_channel_list();
            list[*it]->set_operator(client->getFd());
        }
        if (list[*it]->get_user_limit() != -1 && list[*it]->get_nb_users() >= list[*it]->get_user_limit())
            ERR_CHANNELISFULL(client, list[*it]);
        else if (((int)passwords.size() > index && list[*it]->get_key() != passwords[index]))
            ERR_BADCHANNELKEY(client, list[*it]);
        else if (list[*it]->get_inviteOnly() == true)
            ERR_INVITEONLYCHAN(client, list[*it]);
        else
        {
            list[*it]->set_member(client);
            for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = list[*it]->get_members_begin(); it_members != list[*it]->get_members_end(); it_members++)
                JOIN(client, list[*it], it_members->second);
            client->add_channel(*it);
            if (!list[*it]->get_topic().empty())
                RPL_TOPIC(client, list[*it]);
            RPL_NAMREPLY(client, list[*it]);
            RPL_ENDOFNAMES(client, list[*it]);
        }
        index++;
    }
}

// TODO: Check if user in op_list and remove.
void CmdHandler::part(Command *cmd, Client *client, Server *server)
{
    std::vector<std::string> names;
    std::string message = "ircserv";

    std::string params = cmd->get_params();

    if (params.empty())
    {
        ERR_NEEDMOREPARAMS(cmd, client);
        return ;
    }
    std::stringstream ss(cmd->get_params());
    std::string str;
    // get channels
    ss >> str;
    std::stringstream ss2(str);
    while (std::getline(ss2, str, ','))
        names.push_back(str);
    // get message
    if (std::getline(ss, str))

        message = str;

    std::map<std::string, class Channel *> list = server->get_channel_list();
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
    {
        try
        {
            list.at(*it);
            if (list.at(*it)->member_exists(client->getNick()))
            {
                for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = list[*it]->get_members_begin(); it_members != list[*it]->get_members_end(); it_members++)
                    PART(client, list.at(*it), message, it_members->second);
                client->remove_channel(*it);
                list.at(*it)->remove_member(client->getNick());
            }
            else
                ERR_NOTONCHANNEL(client, *it);
        }
        catch (const std::out_of_range& e)
        {
            ERR_NOSUCHCHANNEL(client, *it);
        }
    }
}

// TODO: Check if user in op_list and remove.
// FIXME: Só kika do primeiro canal da lista
void CmdHandler::kick(Command *cmd, Client *client, Server *server)
{
    std::string channel;
    std::string user;
    std::string message = "ircserv";
    std::vector<std::string> users_to_kick;

    std::string params = cmd->get_params();

    if (params.empty())
    {
        ERR_NEEDMOREPARAMS(cmd, client);
        return ;
    }
    std::stringstream ss(params);
    std::getline(ss, channel, ' ');
    std::stringstream ss2(channel);
    std::getline(ss2, channel, ',');
    std::cout << "Channel: " << channel << std::endl;
    // // get user_to_kick
    while (std::getline(ss, user, ','))
        users_to_kick.push_back(user);
    if (users_to_kick.empty())
    {
        ERR_NEEDMOREPARAMS(cmd, client);
        return ;
    }
    // // get message
    // ss >> message;
    // std::getline(ss, message);
    //kick user
    for (std::vector<std::string>::iterator it_user = users_to_kick.begin(); it_user != users_to_kick.end(); it_user++)
    {
        Client *to_kick = server->get_client(*it_user);
        std::map<std::string, class Channel *> list = server->get_channel_list();
        try
        {
            //Check if channel exists
            list.at(channel);
            //Check if user is member of chan
            if (!list.at(channel)->is_member(client->getNick()))
            {
                ERR_NOTONCHANNEL(client, channel);
                continue ;
            }
            //Check if user is op in chan
            if (!list.at(channel)->is_operator(client->getFd()))
            {
                ERR_CHANPRIVISNEEDED(client, list.at(channel));
                continue ;
            }
                //check if user to be kicked is in channel
            if (!list.at(channel)->is_member(*it_user))
            {
                ERR_USERNOTINCHANNEL(client, *it_user, list.at(channel));
                continue ;
            }
            std::cout << "kicking " << *it_user << std::endl;
            for (std::list<std::pair<std::string*, class Client *> >::iterator it_members = list[channel]->get_members_begin(); it_members != list[channel]->get_members_end(); it_members++)
                KICK(client, list.at(channel), *it_user, message, it_members->second);
            to_kick->remove_channel(channel);
            list.at(channel)->remove_member(to_kick->getNick());
        }
        catch (const std::out_of_range& e)
        {
            ERR_NOSUCHCHANNEL(client, channel);
        }
    }
}

void CmdHandler::quit(Command *cmd, Client *client, Server *server)
{
    std::string message = "Client Quit";
    if (!cmd->get_params().empty())
        message = cmd->get_params();
    server->remove_client(client->getFd(), message);
}



void CmdHandler::mode(Command *cmd, Client *client, Server *server)
{
    (void) cmd;
    (void) client;
    (void) server;
    std::cout << "MODE\n";
}