#include "../inc/main.hpp"

Command::Command(std::string str, Server *server)
{
    char *str2;
    std::vector<struct pollfd>::iterator it_pollfd;

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
        std::map<int, class Client *> client_list = server->get_client_list();
        CmdHandler cl(this, client_list[server->it_pollfd->fd], server);
        std::map<int, char *> &buf = server->get_buf();
        delete[] buf[server->it_pollfd->fd];
        buf.erase(server->it_pollfd->fd);
    }
}
