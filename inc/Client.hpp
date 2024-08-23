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

    public:
    Client(int new_fd, std::string ip);
    ~Client() {};

	// Setters
	void setOldNick(std::string nick) {this->_oldNick = nick;};
	void setNick(std::string nick) {this->_nick = nick;};
	void setUser(std::string username) {this->_username = username;};

	// Getters
	int getFd() const {return _fd;};
	std::string getOldNick() {return this->_oldNick;};
	std::string getNick() {return this->_nick;};
	std::string* getNick_ptr() {return &this->_nick;};
	std::string getHostname() {return this->_hostname;};
	std::string getUsername() {return this->_username;};
	std::string getRealname() {return this->_realname;};

};

#endif