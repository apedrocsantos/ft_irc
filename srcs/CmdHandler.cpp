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
    /* if (cmd->get_command() == "PASS")
        //pass(cmd->get_params(), client);
    if (cmd->get_command() == "USER")
    if (cmd->get_command() == "QUIT") */
}

void CmdHandler::nick(Command *cmd, Client *client) {
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string newNick;

	// Remove the first word and store it in newNick
	ss >> newNick;

	// check if the first char of the nick is a number or symbol

	if (usedNicknames.find(newNick) != usedNicknames.end()) {
		std::cout << "Error: Nickname is already in use." << std::endl;
		return ERR_NickNameInUse(cmd, client);
	}

	// Remove the old nickname from the used set
	usedNicknames.erase(client->getNick());

	if (client->getOldNick().empty()) client->setOldNick(newNick);
	else client->setOldNick(client->getNick());
	//std::cout << "old nick: " << client->getNick() << std::endl;
	client->setNick(newNick);
	RPL_Nick(client);
	//std::string output = Nick(cmd, client);
	//send(client->getFd(), output.c_str(), output.size(), 0);

	// Add the new nickname to the used set
	usedNicknames.insert(newNick);

	std::cout << "params: " << params << " new nickinC: " << client->getNick() <<  std::endl;
	// :justshush!~justshush@<IP> NICK :justshush4
	// :<OLDNICK/CURRENTNICK>!~<user>@<IP> NICK :<NEWNICK>

	// JOIN  :<OLDNICK>!~<USER>@<IP> JOIN #world
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