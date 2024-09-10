#pragma once

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "main.hpp"

class Command;
class Client;
class Server;
class Channel;

// COMMANDS
/**
 * :<OLDNICK/CURRENTNICK>!~<user>@<IP> NICK :<NEWNICK>
 * @param client The user that ran the cmd
 */
void Nick(Client *client);
void User(Client *client);
void JOIN(Client *client, Channel *channel, Client *dest);
void PONG(Command *cmd, Client *client);
void PART(Client *client, Channel *channel, std::string msg, Client *dest);
void KICK(Client *client, Channel *channel, std::string user_to_kick, std::string msg, Client *dest);
void INVITE(Client *client, std::string channel, std::string user_to_invite, Client *dest);
void MODE(Client *client, std::string params, Client *dest);
void QUIT(Client *client, std::string msg, Client *dest);
void ERROR(Client *client, std::string msg);


// RPLs

void RPL_TOPIC(Client *client, Channel *channel);
void RPL_NOTOPIC(Client *client, Channel *channel);
void RPL_NAMREPLY(Client *client, Channel *channel);
void RPL_ENDOFNAMES(Client *client, Channel *channel);
void RPL_WELCOME(Client *client);
void RPL_INVITING(Client *client, std::string user, std::string channel);
void RPL_CHANNELMODEIS(Client *client, Channel *channel);
void RPL_UNAWAY(Client *client);
void RPL_NOAWAY(Client *client);
void RPL_AWAY(Client *client, Client *dest);

//! ERRORS!

/**
 * Returned by a server to a client when it detects a nickname collision (registered of a NICK that already exists by another server).
 * The text used in the last param of this message may vary.
 * @param cmd The command
 * @param client The user that ran the cmd
 */
void ERR_NickCollision(Client *client);

/**
 * Returned when a NICK command cannot be successfully completed as the desired nickname is already in use on the network.
 * The text used in the last param of this message may vary.
 * @param client The user that ran the cmd
 */
void ERR_NickNameInUse(Client *client);

/**
 * Returned when a NICK command cannot be successfully completed as the desired nickname contains characters that are disallowed by the server.
 * See the NICK command for more information on characters which are allowed in various IRC servers.
 * The text used in the last param of this message may vary.
 * @param client The user that ran the cmd
 */
void ERR_ErroneusNickName(Client *client);

/**
 * Returned when a nickname parameter is expected for a command but isn’t given.
 * @param client The user that ran the cmd
 */
void ERR_NoNicknameGiven(Client * client);

/**
 * Returned when a client command cannot be parsed because not enough parameters were supplied.
 * The text used in the last param of this message may vary.
 * @param client The user that ran the cmd
 */
void ERR_NeedMoreParams(Command *cmd, Client *client);

/**
 * Returned when a client tries to change a detail that can only be set during registration
 * (such as resending the PASS or USER after registration).
 * The text used in the last param of this message varies.
 * @param client The user that ran the cmd
 */
void ERR_AlreadyRegistered(Client *client);


/**
 * Returned when a client command cannot be parsed as they are not yet registered.
 * Servers offer only a limited subset of commands until clients are properly registered to the server.
 * The text used in the last param of this message may vary.
 */
//void ERR_NotRegistered(Client *client)

void ERR_NEEDMOREPARAMS(Command *cmd, Client *client);
void ERR_BADCHANNELKEY(Client *client, Channel *channel);
void ERR_CHANNELISFULL(Client *client, Channel *channel);
void ERR_INVITEONLYCHAN(Client *client, Channel *channel);
void ERR_NOSUCHCHANNEL(Client *client, std::string name);
void ERR_NOTONCHANNEL(Client *client, std::string name);
void ERR_UNKNOWNCOMMAND(Client *client, Command *cmd);
void ERR_CHANOPRIVSNEEDED(Client *client, Channel *channel);
void ERR_USERNOTINCHANNEL(Client *client, std::string nick, Channel *channel);
void ERR_USERONCHANNEL(Client *client, std::string nick, Channel *channel);
void ERR_NOSUCHNICK(Client *client, std::string nick);
void ERR_NOTREGISTERED(Client *client);
void ERR_PASSWDMISMATCH(Client *client);
void ERR_UMODEUNKNOWNFLAG(Client *client, char flag);

#endif