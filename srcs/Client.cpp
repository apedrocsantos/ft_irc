#include "../inc/Client.hpp"

Client::Client(int new_fd, std::string ip) : _fd(new_fd), _ip(ip) {
	(void) _fd;
	std::cout << "Connection accepted from ip " << _ip << ".\n";
}

void Client::setNick(std::string _nick) {
	_nickname = _nick;
}

void Client::setOldNick(std::string _nick) {
	_oldNick = _nick;
}

std::string Client::getOldNick() { return _oldNick; }

std::string Client::getNick() { return _nickname; }

std::string Client::getIP() { return _ip; }

std::string Client::getUsername() { return _username; }

std::string Client::getRealname() { return _realname; }