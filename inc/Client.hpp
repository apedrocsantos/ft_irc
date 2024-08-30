#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "main.hpp"
#include "replies.hpp"

class Client
{
	private:
    int _fd;
	std::string _oldNick;
    std::string _nick;
    std::string _username;
    std::string _hostname;
    std::string _realname;
	std::vector<std::string> _channels;

    public:
    Client(int new_fd, std::string ip);
    ~Client() {};

	// Setters
	void setOldNick(std::string nick) {_oldNick = nick;};
	void setNick(std::string nick) {_nick = nick;};
	void setUser(std::string username) {_username = username;};
	void setReal(std::string real) { _realname = real; };

	// Getters
	int getFd() const {return _fd;};
	std::string getOldNick() {return _oldNick;};
	std::string getNick() {return _nick;};
	std::string* getNick_ptr() {return &this->_nick;};
	std::string getHostname() {return this->_hostname;};
	std::string getUsername() {return this->_username;};
	std::string getRealname() {return this->_realname;};
	std::vector<std::string>::iterator get_channels_begin() {return this->_channels.begin();};
	std::vector<std::string>::iterator get_channels_end() {return this->_channels.end();};
	
	void add_channel(std::string name) {this->_channels.push_back(name);};
	void remove_channel(std::string &name);
};

#endif