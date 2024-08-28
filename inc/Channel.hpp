#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "main.hpp"

class Channel {
    std::string _name;
    std::string _topic;
    std::string _key;
    bool _inviteOnly;
    int _user_limit;
    std::list<std::pair<std::string*, class Client *> > _members;
    std::vector<int> _invited_members;
    std::vector<int> _operators;

    public:
    Channel(std::string name);
    ~Channel() {};

    //Getters
    std::string get_name() const {return this->_name;}
    std::string get_topic() const {return this->_topic;}
    std::string get_key() const {return this->_key;}
    bool get_inviteOnly() {return this->_inviteOnly;}
    int get_user_limit() const {return this->_user_limit;}
    int get_nb_users() const {return _members.size();}
    std::string get_members();
    std::list<std::pair<std::string*, class Client *> >::iterator get_members_begin() {return _members.begin();};
    std::list<std::pair<std::string*, class Client *> >::iterator get_members_end() {return _members.end();};

    //Setters
    void set_key(std::string key) {this->_key = key;}
    void set_member(class Client* client) {if (client != NULL) this->_members.insert(this->_members.begin(), std::make_pair(client->getNick_ptr(), client));}
    void set_operator(int fd) {_operators.push_back(fd);}
    void set_topic(std::string topic) {this->_topic = topic;}
    
    void remove_member(std::string name);
    bool is_member(std::string name) {for (std::list<std::pair<std::string*, class Client *> >::iterator it = _members.begin(); it != _members.end(); it++) if (*it->first == name) return true; return false;};
    bool is_operator(int fd) {return(std::find(_operators.begin(), _operators.end(), fd) == _operators.end()) ? false : true;}
    bool member_exists(const std::string name);
};

#endif