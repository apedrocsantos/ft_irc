#pragma once

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "main.hpp"

class Channel {
    std::string _name;
    std::string _topic;
    std::string _key;
    int _user_limit;
    std::list<std::pair<std::string*, class Client *> > _members;
    std::vector<int> _invited_members;
    std::vector<int> _operators;
    std::map<char, bool> _flags;

    public:
    Channel(std::string name);
    ~Channel() {};

    //Getters
    std::string get_name() const {return this->_name;}
    std::string get_topic() const {return this->_topic;}
    std::string get_key() const {return this->_key;}
    int get_user_limit() const {return this->_user_limit;}
    int get_nb_users() const {return _members.size();}
    std::string get_members();
    std::list<std::pair<std::string*, class Client *> >::iterator get_members_begin() {return _members.begin();};
    std::list<std::pair<std::string*, class Client *> >::iterator get_members_end() {return _members.end();};
    bool get_flag(char flag) {if (_flags.find(flag) == _flags.end()) return false; return (_flags[flag]);};
    std::string get_topic() {return _topic;};
    std::string get_modes() {std::string output = "+"; for (std::map<char, bool>::iterator it = _flags.begin(); it != _flags.end(); it++){if (it->second) output += it->first;} return output;};
    int get_limit() {return _user_limit;};

    //Setters
    void set_key(std::string key) {this->_key = key;}
    void set_member(class Client* client) {if (client != NULL) this->_members.insert(this->_members.begin(), std::make_pair(client->getNick_ptr(), client));}
    void set_operator(int fd) {_operators.push_back(fd);}
    void set_topic(std::string topic) {this->_topic = topic;}
    void set_mode(char key, bool value) {if (_flags.find(key) == _flags.end()) return ; _flags.at(key) = value;}
    void set_limit(std::string str) {std::cout << "setting limit to " << str << std::endl; int nb; std::stringstream ss; ss << str; ss >> nb; if (nb <= 0) {_flags['l'] = false; return;} _user_limit = nb;}
    
    void remove_member(std::string name);
    void remove_operator(int fd);
    bool is_member(std::string name) {for (std::list<std::pair<std::string*, class Client *> >::iterator it = _members.begin(); it != _members.end(); it++) if (*it->first == name) return true; return false;};
    bool is_operator(int fd) {return(std::find(_operators.begin(), _operators.end(), fd) == _operators.end()) ? false : true;}
    bool is_invited(int fd) {return(std::find(_invited_members.begin(), _invited_members.end(), fd) == _invited_members.end()) ? false : true;}
    bool member_exists(const std::string name);
    void add_to_invited(int fd) {if (std::find(_invited_members.begin(), _invited_members.end(), fd) == _invited_members.end()) _invited_members.push_back(fd);}
    void remove_from_invited(int fd) {std::vector<int>::iterator it = std::find(_invited_members.begin(), _invited_members.end(), fd); if (it != _invited_members.end()) _invited_members.erase(it);}
};

#endif