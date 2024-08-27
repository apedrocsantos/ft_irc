#include "../inc/main.hpp"
#include "../inc/replies.hpp"

// MESSAGES

void NICK(Client *client) {
	std::string output = ":" + client->getOldNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " NICK :" + client->getNick() + "\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void JOIN(Client *client, Channel *channel, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " JOIN " + channel->get_name() + "\r\n"; send(dest->getFd(), output.c_str(), output.size(), 0);}

void PONG(Command *cmd, Client *client) {std::string output = ":ircserv PONG " + cmd->get_params() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

// CORRECT if(dest... etc)
void PART(Client *client, Channel *channel, std::string msg, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " PART " + channel->get_name() + " " + msg + "\r\n"; if(dest->getFd() != client->getFd()) send(dest->getFd(), output.c_str(), output.size(), 0);}

// REPLIES

void RPL_TOPIC(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "332 " + client->getNick() + " " + channel->get_name() + " :" + channel->get_topic() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_NAMREPLY(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "353 " + client->getNick() + " @ " + channel->get_name() + " :" + channel->get_members() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_ENDOFNAMES(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "366 " + client->getNick() + " @ " + channel->get_name() + " :End of /NAMES list.\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_WELCOME(Client *client) {std::string output = std::string(":ircserv ") + "001 " + client->getNick() + " :Welcome to ircserv, " + client->getUsername() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

//ERRORS
//* not done, idk how to do it yet
void ERR_NickCollision(Client *client) {
	std::string output = ":ircserv 463 " + client->getNick() + " :Nickname collision KILL from " + client->getUsername() + "@" + client->getHostname() + "\r\n";
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

void ERR_NoNicknameGiven(Client *client) {
	std::string output = ":ircserv 431 :No nickname given\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void ERR_AlreadyRegistered(Client *client) {
	std::string output = ":ircserv 462 :You may not reregister\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void ERR_NEEDMOREPARAMS(Command *cmd, Client *client) {std::string output = std::string(":ircserv ") + "461 " + client->getNick() + " " + cmd->get_command() + std::string(" :Not enough parameters\r\n"); send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_BADCHANNELKEY(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "475 " + client->getNick() + " " + channel->get_name() + " :Cannot join channel (+k)\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_CHANNELISFULL(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "471 " + client->getNick() + " " + channel->get_name() + " :Cannot join channel (+l)\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_INVITEONLYCHAN(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "473 " + client->getNick() + " " + channel->get_name() + " :Cannot join channel (+i)\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_NOSUCHCHANNEL(Client *client, std::string name) {std::string output = std::string(":ircserv ") + "403 " + client->getNick() + " " + name + " :No such channel\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_NOTONCHANNEL(Client *client, std::string name) {std::string output = std::string(":ircserv ") + "442 " + client->getNick() + " " + name + " :You're not on that channel\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

/* void ERR_NotRegistered(Client *client) {
	std::string output = ":ircserv 451 :You have not registered\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
} */

// void RPL_Join(Command *cmd, Client *client) {
// 	std::string output = ":" + client->getNick() + " " + "!~@" + client->getHostname() + " " + cmd->get_params() + "\n";
// 	send(client->getFd(), output.c_str(), output.size(), 0);
// }

//! ERRORS!


/* void RPL_topic(Client client, Channel channel) {
	std::string output = ":ircserv 332 " + client.getFd() + " " + channel.get_name() + " :" + channel.get_topic() + "\n";
	send(client.getFd(), output.c_str(), output.size(), 0);
} */