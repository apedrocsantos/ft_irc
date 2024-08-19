#include "../inc/CmdHandler.hpp"

CmdHandler::CmdHandler(Command *cmd, Client *client, Server *server)
{
    (void) cmd;
    (void) client;
    (void) server;
    std::string password = "pwd";
    if (cmd->get_command() == "PASS")
        pass(cmd->get_params(), client);
    if (cmd->get_command() == "NICK")
        nick(cmd->get_params(), client);
    if (cmd->get_command() == "USER")
    if (cmd->get_command() == "QUIT")
}

// void CmdHandler::pass(std::string params, class Client *client)
// {
//     if (params.empty())
//         send_msg("461 [nickname] :Please provide a password\r\n", client);
//     else if ((int)params.find(32) != -1 || params.empty() || params.compare("pass"))
//         send_msg("464 [nickname] :Wrong password\r\n", client);
// }

// void CmdHandler::nick(std::string params, class Client *client)
// {
//     std::cout << params << std::endl;
//     (void) client;
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