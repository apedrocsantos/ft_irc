#include "../inc/Command.hpp"

Command::Command(std::string str)
{
    char *str2;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line, '\r'))
    {
        if (line == "\n")
            return;
        std::stringstream ss2(line);
        std::string word;
        ss2 >> word;
        if (word[0] == ':')
        {
            this->_prefix = word.erase(0,1);
            ss2 >> word;
        }
        this->_command = word;
        while (ss2 >> word)
        {
            str2 = &word[0];
            this->_params.append(str2);
            this->_params.append(" ");
        }
        if (!this->_prefix.empty())
            std::cout << "prefix: " << this->_prefix << ", ";
        std::cout << "command: " << this->_command << ", params: " << this->_params << std::endl;
    }
}

void Command::exec(class Client *client)
{
    (void) client;
    // std::cout << "EXCUTE on " << client->getFd() << "\n";
}