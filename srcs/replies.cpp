#include "../inc/main.hpp"
#include "../inc/replies.hpp"

void RPL_Nick(Command *cmd, Client *client) {
	(void) cmd;
	std::string output = ":" + client->getOldNick() + "!~" + client->getUsername() + "@" + client->getIP() + " NICK :" + client->getNick() + "\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void RPL_Join(Command *cmd, Client *client) {
	std::string output = ":" + client->getNick() + " " + "!~@" + client->getIP() + " " + cmd->get_params() + "\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

//! ERRORS!

void ERR_NickCollision(Client *client) {
	std::string output = ":ircserv 463 " + client->getNick() + " :Nickname collision KILL from " + client->getUsername() + "@" + client->getIP() + "\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void ERR_NickNameInUse(Client *client) {
	std::string output = ":ircserv 433 " + client->getNick() + " :Nickname is already in use\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void ERR_ErroneusNickName(Client *client) {
	std::string output = ":ircserv 432 " + client->getNick() + " :Erroneus nickname\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void ERR_NoNicknameGiven(Client * client) {
	std::string output = ":ircserv 431 :No nickname given\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}


/* void RPL_topic(Client client, Channel channel) {
	std::string output = ":ircserv 332 " + client.getFd() + " " + channel.get_name() + " :" + channel.get_topic() + "\n";
	send(client.getFd(), output.c_str(), output.size(), 0);
} */