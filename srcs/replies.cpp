#include "../inc/main.hpp"
#include "../inc/replies.hpp"

/////////////////////////////////////// MESSAGES

std::string Nick(Client *client) {return ":" + client->getOldNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " NICK :" + client->getNick() + "\r\n";}
std::string User(Client *client) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " USER :" + client->getNick() + "\r\n";}
std::string JOIN(Client *client, Channel *channel) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " JOIN " + channel->get_name() + "\r\n";}
std::string PRIVMSG(Client *client, std::string dest_str, std::string message) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " PRIVMSG " + dest_str + " " + message + "\r\n";}
std::string PONG(Command *cmd) {return ":ircserv PONG " + cmd->get_params() + "\r\n";}
std::string PART(Client *client, Channel *channel, std::string msg) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " PART " + channel->get_name() + " " + msg + "\r\n";}
std::string KICK(Client *client, Channel *channel, std::string user_to_kick, std::string msg) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " KICK " + channel->get_name() + " " + user_to_kick + " " + msg + "\r\n";}
std::string INVITE(Client *client, std::string channel, std::string user_to_invite) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " INVITE " + user_to_invite + " " + channel + "\r\n";}
std::string MODE(Client *client, Channel *channel, std::string params) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " MODE  " + channel->get_name() + " " + params + "\r\n";}
std::string QUIT(Client *client, std::string msg) {return ":" + client->getNick() + "!~" + client->getUsername() + "@" + client->getHostname() + " QUIT " + msg + "\r\n";}
std::string ERROR(std::string msg) {return "ERROR: " + msg + "\r\n";}

/////////////////////////////////////// REPLIES

std::string RPL_WELCOME(Client *client) {return ":ircserv 001 " + client->getNick() + " :Welcome to ircserv, " + client->getUsername() + "\r\n";}
std::string RPL_AWAY(Client *client, Client *dest) {return ":ircserv 301 " + client->getNick() + " " + dest->getNick() + " :" + dest->get_away_msg() + "\r\n";}
std::string RPL_UNAWAY(Client *client) {return ":ircserv 305 " + client->getNick() + " :You are no longer marked as being away\r\n";}
std::string RPL_NOAWAY(Client *client) {return ":ircserv 306 " + client->getNick() + " :You have been marked as being away\r\n";}
std::string RPL_CHANNELMODEIS(Client *client, Channel *channel) {return ":ircserv 324 " + client->getNick() + " " + channel->get_name() + " " + channel->get_modes() + "\r\n";}
std::string RPL_NOTOPIC(Client *client, Channel *channel) {return ":ircserv 331 " + client->getNick() + " " + channel->get_name() + " :No topic is set\r\n";}
std::string RPL_TOPIC(Client *client, Channel *channel) {return ":ircserv 332 " + client->getNick() + " " + channel->get_name() + " " + channel->get_topic() + "\r\n";}
std::string RPL_INVITING(Client *client, std::string user, std::string channel) {return ":ircserv 341 " + client->getNick() + " " + user + " " + channel + "\r\n";}
std::string RPL_NAMREPLY(Client *client, Channel *channel) {return ":ircserv 353 " + client->getNick() + " = " + channel->get_name() + " :" + channel->get_members() + "\r\n";}
std::string RPL_ENDOFNAMES(Client *client, Channel *channel) {return ":ircserv 366 " + client->getNick() + " = " + channel->get_name() + " :End of /NAMES list.\r\n";}

/////////////////////////////////////// ERRORS

std::string ERR_NOSUCHNICK(Client *client, std::string nick) {return ":ircserv 401 " + client->getNick() + " " + nick + " :No such nick/channel\r\n";}
std::string ERR_NOSUCHCHANNEL(Client *client, std::string name) {return ":ircserv 403 " + client->getNick() + " " + name + " :No such channel\r\n";}
std::string ERR_NORECIPIENT(Client *client, Command *cmd) {return ":ircserv 411 " + client->getNick() + " :No recipient given (" + cmd->get_command() + ")\r\n";}
std::string ERR_NOTEXTTOSEND(Client *client) {return ":ircserv 412 " + client->getNick() + " :No text to send\r\n";}
std::string ERR_UNKNOWNCOMMAND(Client *client, Command *cmd) {return ":ircserv 421 " + client->getNick() + " " + cmd->get_command() + " :Unknown command\r\n";}
std::string ERR_NoNicknameGiven(Client *client) {return ":ircserv 431 " + client->getNick() + " :No nickname given\r\n";}
std::string ERR_ErroneusNickName(Client *client) {return ":ircserv 432 " + client->getNick() + " :Erroneus nickname\r\n";}
std::string ERR_NickNameInUse(Client *client) {return ":ircserv 433 " + client->getNick() + " :Nickname is already in use\r\n";}
std::string ERR_NickCollision(Client *client) {return ":ircserv 436 " + client->getNick() + " :Nickname collision KILL from " + client->getUsername() + "@" + client->getHostname() + "\r\n";}
std::string ERR_USERNOTINCHANNEL(Client *client, std::string nick, Channel *channel) {return ":ircserv 441 " + client->getNick() + " " + nick + " " + channel->get_name() + " :They aren't on that channel\r\n";}
std::string ERR_NOTONCHANNEL(Client *client, std::string name) {return ":ircserv 442 " + client->getNick() + " " + name + " :You're not on that channel\r\n";}
std::string ERR_USERONCHANNEL(Client *client, std::string nick, Channel *channel) {return ":ircserv 443 " + client->getNick() + " " + nick + " " + channel->get_name() + " :is already on channel\r\n";}
std::string ERR_NOTREGISTERED(Client *client) {std::string nick = client->getNick(); if (nick.empty()) nick = "*"; return ":ircserv 451 " + nick + " :You have not registered\r\n";}
std::string ERR_NEEDMOREPARAMS(Command *cmd, Client *client) {return ":ircserv 461 " + client->getNick() + " " + cmd->get_command() + " :Not enough parameters\r\n";}
std::string ERR_AlreadyRegistered(Client *client) {return ":ircserv 462 " + client->getNick() + " :You may not reregister\r\n";}
std::string ERR_PASSWDMISMATCH() {return ":ircserv 464 * :Wrong password.\r\n";}
std::string ERR_CHANNELISFULL(Client *client, Channel *channel) {return ":ircserv 471 " + client->getNick() + " " + channel->get_name() + " :Cannot join channel (+l)\r\n";}
std::string ERR_INVITEONLYCHAN(Client *client, Channel *channel) {return ":ircserv 473 " + client->getNick() + " " + channel->get_name() + " :Cannot join channel (+i)\r\n";}
std::string ERR_BADCHANNELKEY(Client *client, Channel *channel) {return ":ircserv 475 " + client->getNick() + " " + channel->get_name() + " :Cannot join channel (+k)\r\n";}
std::string ERR_CHANOPRIVSNEEDED(Client *client, Channel *channel) {return ":ircserv 482 " + client->getNick() + " " + channel->get_name() + " :You're not channel operator\r\n";}
std::string ERR_UMODEUNKNOWNFLAG(Client *client, char flag) {return ":ircserv 501 " + client->getNick() + " :Unknown " + flag + " flag.\r\n";}
