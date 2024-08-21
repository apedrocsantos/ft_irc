#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "main.hpp"
#include "replies.hpp"

class Client
{
	private:
    int _fd;
	std::string _oldNick;
    std::string _nickname;
    std::string _username;
    std::string _ip;
    std::string _realname;

    public:
    Client(int new_fd, std::string ip);
    ~Client() {};

	// Setters
	void setNick(std::string _nick);
	void setOldNick(std::string _nick);

	// Getters
	int getFd() const {return _fd;};
	std::string getOldNick();
	std::string getNick();
	std::string getIP();
	std::string getUsername();
	std::string getRealname();

};

#endif