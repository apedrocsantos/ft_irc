#include "../inc/main.hpp"
#include "../inc/replies.hpp"

// MESSAGES

void NICK(Client *client) {
	std::string output = ":" + client->getOldNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " NICK :" + client->getNick() + "\r\n";
	send(client->getFd(), output.c_str(), output.size(), 0);
}

void JOIN(Client *client, Channel *channel, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " JOIN " + channel->get_name() + "\r\n"; send(dest->getFd(), output.c_str(), output.size(), 0);}

void PONG(Command *cmd, Client *client) {std::string output = ":ircserv PONG " + cmd->get_params() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void PART(Client *client, Channel *channel, std::string msg, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " PART " + channel->get_name() + " :" + msg + "\r\n"; send(dest->getFd(), output.c_str(), output.size(), 0);}

void KICK(Client *client, Channel *channel, std::string user_to_kick, std::string msg, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " KICK " + channel->get_name() + " " + user_to_kick + " :" + msg + "\r\n"; send(dest->getFd(), output.c_str(), output.size(), 0);}

void INVITE(Client *client, std::string channel, std::string user_to_invite, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " INVITE " + channel + " " + user_to_invite + "\r\n"; send(dest->getFd(), output.c_str(), output.size(), 0);}

void MODE(Client *client, std::string params, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " MODE " + params + "\r\n"; send(dest->getFd(), output.c_str(), output.size(), 0);}

void QUIT(Client *client, std::string msg, Client *dest) {std::string output = ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " QUIT :" + msg + "\r\n"; if(dest->getFd() != client->getFd()) send(dest->getFd(), output.c_str(), output.size(), 0);}

// REPLIES

void RPL_TOPIC(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "332 " + client->getNick() + " " + channel->get_name() + " :" + channel->get_topic() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_NOTOPIC(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "331 " + client->getNick() + " " + channel->get_name() + " :No topic is set\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_NAMREPLY(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "353 " + client->getNick() + " @ " + channel->get_name() + " :" + channel->get_members() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_ENDOFNAMES(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "366 " + client->getNick() + " @ " + channel->get_name() + " :End of /NAMES list.\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_WELCOME(Client *client) {std::string output = std::string(":ircserv ") + "001 " + client->getNick() + " :Welcome to ircserv, " + client->getUsername() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_INVITING(Client *client, std::string user, std::string channel) {std::string output = std::string(":ircserv ") + "341 " + client->getNick() + " " + user + " " + channel + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}

void RPL_CHANNELMODEIS(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "324 " + client->getNick() + " " + channel->get_name() + " " + channel->get_modes() + "\r\n"; send(client->getFd(), output.c_str(), output.size(), 0);}
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

void ERR_UNKNOWNCOMMAND(Client *client, Command *cmd) {std::string output = std::string(":ircserv ") + "421 " + client->getNick() + " " + cmd->get_command() + std::string(" :Unknown command\r\n"); send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_CHANOPRIVSNEEDED(Client *client, Channel *channel) {std::string output = std::string(":ircserv ") + "482 " + client->getNick() + " " + channel->get_name() + std::string(" :You're not channel operator\r\n"); send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_USERNOTINCHANNEL(Client *client, std::string nick, Channel *channel) {std::string output = std::string(":ircserv ") + "441 " + client->getNick() + " " + nick + " " + channel->get_name() + std::string(" :They aren't on that channel\r\n"); send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_USERONCHANNEL(Client *client, std::string nick, Channel *channel) {std::string output = std::string(":ircserv ") + "443 " + client->getNick() + " " + nick + " " + channel->get_name() + std::string(" :is already on channel\r\n"); send(client->getFd(), output.c_str(), output.size(), 0);}

void ERR_NOSUCHNICK(Client *client, std::string nick) {std::string output = std::string(":ircserv ") + "401 " + client->getNick() + " " + nick + std::string(" :No such nick/channel\r\n"); send(client->getFd(), output.c_str(), output.size(), 0);}
