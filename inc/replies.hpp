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
std::string Nick(Client *client);
std::string User(Client *client);
std::string JOIN(Client *client, Channel *channel);
std::string PRIVMSG(Client *client, std::string dest_str, std::string message);
std::string PONG(Command *cmd);
std::string PART(Client *client, Channel *channel, std::string msg);
std::string KICK(Client *client, Channel *channel, std::string user_to_kick, std::string msg);
std::string INVITE(Client *client, std::string channel, std::string user_to_invite);
std::string MODE(Client *client, Channel *channel, std::string params);
std::string QUIT(Client *client, std::string msg);
std::string ERROR(std::string msg);


// RPLs

std::string RPL_TOPIC(Client *client, Channel *channel);
std::string RPL_NOTOPIC(Client *client, Channel *channel);
std::string RPL_NAMREPLY(Client *client, Channel *channel);
std::string RPL_ENDOFNAMES(Client *client, Channel *channel);
std::string RPL_WELCOME(Client *client);
std::string RPL_INVITING(Client *client, std::string user, std::string channel);
std::string RPL_CHANNELMODEIS(Client *client, Channel *channel);
std::string RPL_UNAWAY(Client *client);
std::string RPL_NOAWAY(Client *client);
std::string RPL_AWAY(Client *client, Client *dest);

//! ERRORS!

/**
 * Returned by a server to a client when it detects a nickname collision (registered of a NICK that already exists by another server).
 * The text used in the last param of this message may vary.
 * @param cmd The command
 * @param client The user that ran the cmd
 */
std::string ERR_NickCollision(Client *client);

/**
 * Returned when a NICK command cannot be successfully completed as the desired nickname is already in use on the network.
 * The text used in the last param of this message may vary.
 * @param client The user that ran the cmd
 */
std::string ERR_NickNameInUse(Client *client);

/**
 * Returned when a NICK command cannot be successfully completed as the desired nickname contains characters that are disallowed by the server.
 * See the NICK command for more information on characters which are allowed in various IRC servers.
 * The text used in the last param of this message may vary.
 * @param client The user that ran the cmd
 */
std::string ERR_ErroneusNickName(Client *client);

/**
 * Returned when a nickname parameter is expected for a command but isn’t given.
 * @param client The user that ran the cmd
 */
std::string ERR_NoNicknameGiven(Client * client);

/**
 * Returned when a client command cannot be parsed because not enough parameters were supplied.
 * The text used in the last param of this message may vary.
 * @param client The user that ran the cmd
 */
std::string ERR_NeedMoreParams(Command *cmd, Client *client);

/**
 * Returned when a client tries to change a detail that can only be set during registration
 * (such as resending the PASS or USER after registration).
 * The text used in the last param of this message varies.
 * @param client The user that ran the cmd
 */
std::string ERR_AlreadyRegistered(Client *client);


/**
 * Returned when a client command cannot be parsed as they are not yet registered.
 * Servers offer only a limited subset of commands until clients are properly registered to the server.
 * The text used in the last param of this message may vary.
 */
//std::string ERR_NotRegistered(Client *client)

std::string ERR_NEEDMOREPARAMS(Command *cmd, Client *client);
std::string ERR_BADCHANNELKEY(Client *client, Channel *channel);
std::string ERR_CHANNELISFULL(Client *client, Channel *channel);
std::string ERR_INVITEONLYCHAN(Client *client, Channel *channel);
std::string ERR_NOSUCHCHANNEL(Client *client, std::string channel);
std::string ERR_NOTONCHANNEL(Client *client, std::string name);
std::string ERR_UNKNOWNCOMMAND(Client *client, Command *cmd);
std::string ERR_CHANOPRIVSNEEDED(Client *client, Channel *channel);
std::string ERR_USERNOTINCHANNEL(Client *client, std::string nick, Channel *channel);
std::string ERR_USERONCHANNEL(Client *client, std::string nick, Channel *channel);
std::string ERR_NOSUCHNICK(Client *client, std::string nick);
std::string ERR_NOTREGISTERED(Client *client);
std::string ERR_PASSWDMISMATCH();
std::string ERR_UMODEUNKNOWNFLAG(Client *client, char flag);
std::string ERR_NOTEXTTOSEND(Client *client);
std::string ERR_NORECIPIENT(Client *client, Command *cmd);

#endif