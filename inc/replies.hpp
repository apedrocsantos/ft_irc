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
void Join(Client *client, Channel *channel);
void PONG(Command *cmd, Client *client);


// RPLs

void RPL_TOPIC(Client *client, Channel *channel);

void RPL_NAMREPLY(Client *client, Channel *channel);

void RPL_ENDOFNAMES(Client *client, Channel *channel);

void RPL_WELCOME(Client *client);

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

void ERR_BadChannelKey(Client *client, Channel *channel);
void ERR_ChannelIsFull(Client *client, Channel *channel);
void ERR_InviteOnlyChan(Client *client, Channel *channel);
void ERR_NoSuChChannel(Client *client, std::string name);
#endif