#ifndef UTILS_HPP
#define UTILS_HPP

int check_valid_port(char *port);

// ERRORS
// void ERR_NEEDMOREPARAMS(Command *cmd, Client *client);
// void ERR_BADCHANNELKEY(Client *client, Channel *channel);
// void ERR_CHANNELISFULL(Client *client, Channel *channel);
// void ERR_INVITEONLYCHAN(Client *client, Channel *channel);

// RPLs
// void RPL_TOPIC(Client *client, Channel *channel);
// void RPL_NAMREPLY(Client *client, Channel *channel);
// void RPL_ENDOFNAMES(Client *client, Channel *channel);
// void ERR_BADCHANNELKEY(Client *client, Channel *channel);
// void ERR_NOSUCHCHANNEL(Client *client, std::string name);

// MSGs
void Join(Client *client, Channel *channel);


#endif