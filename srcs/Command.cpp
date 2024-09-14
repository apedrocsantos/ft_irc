#include "../inc/main.hpp"

Command::Command(std::string str, Server *server)
{
    std::vector<struct pollfd>::iterator it_pollfd;
    std::stringstream ss(str);
    std::string line;

	ss >> std::ws;
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
		ss2 >> std::ws;
		if (!std::getline(ss2, word))
			word.erase();
		if (word[word.size() - 1] == '\n')
			word.erase(word.size() - 1, 1);
		this->_params = word;
        if (!this->_prefix.empty())
            std::cout << "prefix: " << this->_prefix << ", ";
        std::cout << server->it_pollfd->fd << " says: ";
        std::cout << "command: {" << this->_command << "}, params: {" << this->_params << "}\n";
        std::map<int, class Client *> client_list = server->get_client_list();
		if (client_list.find(server->it_pollfd->fd) == client_list.end())
			return;
        CmdHandler cl(this, client_list[server->it_pollfd->fd], server);
        std::map<int, char *> &in_buf = server->get_buf();
        delete[] in_buf[server->it_pollfd->fd];
        in_buf.erase(server->it_pollfd->fd);
    }
}
