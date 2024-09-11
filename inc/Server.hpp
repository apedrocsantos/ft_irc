#pragma once

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
    int socketId, new_fd; //initial socket fd and client fd
    struct addrinfo hints, *res; //initial socket settings
    std::map<int, class Client *>::iterator it_map; //client iterator
    class Command command; //command parser
    std::map<std::string, class Channel *> channel_list;
    std::map<std::string, class Channel *>::iterator it_channel_list;
    std::map<int, class Client *> client_list; //list of clients, indexed by the client fd
    std::vector<struct pollfd> pollfds; //list of pollfd structs
    std::map<int, char *> buf; //buffer with incomplete messages, indexed by the client fd
    void receive_msg();


    public:
	std::set<std::string> usedNicknames;
    std::vector<struct pollfd>::iterator it_pollfd; //pollfd iterator

    //Getters

    std::string get_name() const {return this->_name;}
    std::map<int, class Client *> get_client_list() const {return client_list;};
    std::map<int, char *>& get_buf() {return buf;};
    std::vector<struct pollfd> get_pollfds() {return pollfds;};
    Client * get_client(std::string name) {for (std::map<int, class Client *>::iterator it = client_list.begin(); it != client_list.end(); it++)if (it->second->getNick() == name) return it->second; return NULL;}
    Channel * get_channel(std::string name) {for (std::map<std::string, class Channel *>::iterator it = channel_list.begin(); it != channel_list.end(); it++)if (it->first == name) return it->second; return NULL;}
    std::string get_pwd() const {return this->pwd;};
    std::map<std::string, class Channel *> get_channel_list() const {return this->channel_list;};
    std::map<std::string, class Channel *>::iterator get_channel_begin() {return this->channel_list.begin();};
    std::map<std::string, class Channel *>::iterator get_channel_end() {return this->channel_list.end();};
    int get_nb_connected_users() const {return this->pollfds.size() - 1;};

    void add_client();
    void remove_client(int fd, std::string mgs);
    void remove_channel(std::string name);
    void add_channel(std::string name, class Channel *channel) {channel_list.insert(std::make_pair(name, channel));};
    bool channel_exists(std::string name) {return (channel_list.find(name) != channel_list.end());}
    void remove_pollfd(int fd) {for (std::vector<struct pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); it++) {if (it->fd == fd) {pollfds.erase(it); return;}}}
};

#endif