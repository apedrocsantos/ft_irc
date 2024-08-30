#include "../inc/main.hpp"

Command::Command(std::string str, Server *server)
{
    char *str2;

    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line, '\r'))
    {
        if (line == "\n")
            return;
        this->_prefix.clear();
        this->_command.clear();
        this->_params.clear();
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
            if (this->_params.size())
                this->_params.append(" ");
            str2 = &word[0];
            this->_params.append(str2);
        }
        if (!this->_prefix.empty())
            std::cout << "prefix: " << this->_prefix << ", ";
        std::cout << server->it_pollfd->fd << " says: ";
        std::cout << "command: " << this->_command << ", params: " << this->_params << std::endl;
        CmdHandler cl(this, server->client_list[server->it_pollfd->fd], server);
        delete[] server->buf[server->it_pollfd->fd];
        server->buf.erase(server->it_pollfd->fd);
    }
}
