#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

# include "CmdHandler.hpp"

extern bool stop;

class Server
{
    public:
    Server(char *port, std::string pwd);
    void start();
    ~Server();

    private:
    char *port;
    std::string pwd; //password
    std::map<int, char *> buf; //buffer with incomplete messages, indexed by the client fd
    int socketId, new_fd; //initial socket fd and client fd
    struct addrinfo hints, *res; //initial socket settings
    std::vector<struct pollfd> pollfds; //list of pollfd structs
    std::vector<struct pollfd>::iterator it_pollfd; //pollfd iterator
    std::map<int, class Client *> client_list; //list of clients, indexed by the client fd
    std::map<int, class Client *>::iterator it_map; //client iterator
    class Command *command; //command parser
    // class CmdHandler *_CmdHandler;

    void add_client();
    void remove_client();
    void receive_msg();
    std::string get_pwd() const {return this->pwd;};
    int get_nb_connected_users() const {return this->pollfds.size() - 1;};
    // std::map<std::string, class CmdHandler> getCmdHandler();
};

#endif