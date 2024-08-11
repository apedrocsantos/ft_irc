#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Server
{
    public:
    Server(char *port, std::string pwd);
    void start();
    ~Server();

    private:
    char *port;
    std::string pwd;
    int socketId, new_fd; //initial socket fd and client fd
    struct addrinfo hints, *res; //initial socket settings
    char buf[MESSAGE_BUFFER_SIZE];
    std::vector <struct pollfd> pollfds;
    std::vector<struct pollfd>::iterator it_pollfd;
    std::map<int, class Client *>::iterator it_map;
    std::map<int, class Client *> client_list;
    // class CmdList *_cmdList;

    void add_client();
    void remove_client();
    void receive_msg();
    std::string get_pwd();
    int get_nb_connected_users() {return this->pollfds.size() - 1;};
    // std::map<std::string, class CmdList> getCmdList();
    // void send_msg();
};

#endif