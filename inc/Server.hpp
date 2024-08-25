#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

extern bool stop;

class Server
{
    public:
    Server(char *port, std::string pwd);
    void start();
    ~Server();

    private:
    std::string _name;
    char *port;
    std::string pwd; //password
    // std::map<int, char *> buf; //buffer with incomplete messages, indexed by the client fd
    int socketId, new_fd; //initial socket fd and client fd
    struct addrinfo hints, *res; //initial socket settings
    // std::vector<struct pollfd> pollfds; //list of pollfd structs
    // std::vector<struct pollfd>::iterator it_pollfd; //pollfd iterator
    //std::map<int, class Client *> client_list; //list of clients, indexed by the client fd
    std::map<int, class Client *>::iterator it_map; //client iterator
    class Command *command; //command parser
    std::map<std::string, class Channel *> channel_list;
    std::map<std::string, class Channel *>::iterator it_channel_list;

    void add_client();
    void remove_client();
    void receive_msg();

    public:
	std::set<std::string> usedNicknames;
    std::map<int, class Client *> client_list; //list of clients, indexed by the client fd
    // std::map<int, class Client *> * get_client_list() {return &this->client_list;}; //list of clients, indexed by the client fd
    std::map<int, char *> buf; //buffer with incomplete messages, indexed by the client fd
    std::vector<struct pollfd> pollfds; //list of pollfd structs
    std::vector<struct pollfd>::iterator it_pollfd; //pollfd iterator
    std::string get_name() const {return _name;}
    std::string get_pwd() const {return pwd;};
    std::map<std::string, class Channel *> get_channel_list() const {return this->channel_list;};
    int get_nb_connected_users() const {return this->pollfds.size() - 1;};
    void add_channel(std::string name, class Channel *channel) {channel_list.insert(std::make_pair(name, channel));};
	std::map<int, class Client *> getClientList() {return client_list;};
};

#endif