#pragma once

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "main.hpp"

class Command {
    std::string _prefix;
    std::string _command;
    std::string _params;

    public:
    Command(std::string str, Server *server);
    ~Command() {};
	
    std::string get_prefix() const {return this->_prefix;};
    std::string get_command() const {return this->_command;};
    std::string get_params() const {return this->_params;};
};

#endif