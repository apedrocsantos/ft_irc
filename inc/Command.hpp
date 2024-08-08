#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "main.hpp"

class Command {
    std::string _prefix;
    std::string _command;
    std::string _params;

    public:
    Command(std::string str);
    ~Command();
    void exec(class Client *client);
};

#endif