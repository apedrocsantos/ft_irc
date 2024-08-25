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
	void setOldNick(std::string nick) {_oldNick = nick;};
	void setNick(std::string nick) {_nick = nick;};
	void setUser(std::string username) {_username = username;};
	void setReal(std::string real) { _realname = real; };

	// Getters
	int getFd() const {return _fd;};
	std::string getOldNick() {return _oldNick;};
	std::string getNick() {return _nick;};
	std::string* getNick_ptr() {return &this->_nick;};
	std::string getHostname() {return _hostname;};
	std::string getUsername() {return _username;};
	std::string getRealname() {return _realname;};

};

#endif