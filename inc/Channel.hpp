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
    std::vector<std::string> _invited_members;
    std::vector<std::string> _operators;

    public:
    Channel(std::string name);
    ~Channel() {};
    std::string get_name() const {return this->_name;}
    std::string get_topic() const {return this->_topic;}
    std::string get_key() const {return this->_key;}
    bool get_inviteOnly() {return this->_inviteOnly;}
    int get_user_limit() const {return this->_user_limit;}
    int get_nb_users() const {return _members.size();}
    std::string get_members();
    std::list<std::pair<std::string*, class Client *> >::iterator get_members_begin() {return _members.begin();};
    std::list<std::pair<std::string*, class Client *> >::iterator get_members_end() {return _members.end();};
    bool member_exists(const std::string name);
    void set_key(std::string key) {this->_key = key;}
    void set_member(class Client* client) {this->_members.insert(this->_members.begin(), std::make_pair(client->getNick_ptr(), client));}
    void set_topic(std::string topic) {this->_topic = topic;}
    void remove_member(std::string name);
    bool is_operator(std::string nick) {return(std::find(_operators.begin(), _operators.end(), nick) == _operators.end()) ? false : true;}
};

#endif