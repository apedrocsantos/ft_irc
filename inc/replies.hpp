#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "main.hpp"
#include "Client.hpp"
#include "Command.hpp"

class Command;
class Client;

// COMMANDS
void RPL_Join(Command *cmd, Client *client);

/**
 * :<OLDNICK/CURRENTNICK>!~<user>@<IP> NICK :<NEWNICK>
 * @param cmd The command
 * @param client The user that ran the cmd
 */
void RPL_Nick(Command *cmd, Client *client);

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
 */
void ERR_ErroneusNickName(Client *client);

/**
 * Returned when a nickname parameter is expected for a command but isn’t given.
 */
void ERR_NoNicknameGiven(Client * client);

//void RPL_topic(Client client, Channel channel);

#endif