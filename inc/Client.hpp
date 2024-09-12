#pragma once

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
	bool _is_away;
	bool _registered;
	bool _auth;
	bool _disconnect;
	std::string _away_msg;
	std::vector<std::string> _channels;

    public:
    Client(int new_fd, std::string ip);
    ~Client() {};

	// Setters
	void setOldNick(std::string nick) {_oldNick = nick;};
	void setNick(std::string nick) {_nick = nick;};
	void setUser(std::string username) {_username = username;};
	void setReal(std::string real) { _realname = real; };
	void set_away(bool val) {_is_away = val;}
	void set_away_msg(std::string msg) {_away_msg = msg;};
	void set_registered(bool reg) {_registered = reg;};
	void set_auth(bool reg) {_auth = reg;};
	void set_disconnect(bool what) {_disconnect = what;};

	// Getters
	int getFd() const {return _fd;};
	std::string getOldNick() const {return _oldNick;};
	std::string getNick() const {return _nick;};
	std::string* getNick_ptr() {return &this->_nick;};
	std::string getHostname() const {return this->_hostname;};
	std::string getUsername() const {return this->_username;};
	std::string getRealname() const {return this->_realname;};
	std::vector<std::string> get_channels() {return this->_channels;};
	std::vector<std::string>::iterator get_channels_begin() {return this->_channels.begin();};
	std::vector<std::string>::iterator get_channels_end() {return this->_channels.end();};
	bool get_away() const {return _is_away;};
	bool get_registered() const {return _registered;};
	bool get_auth() const {return _auth;};
	std::string get_away_msg() const {return _away_msg;};
	bool get_disconnect() {return _disconnect;};

	void add_channel(std::string name) {this->_channels.push_back(name);};
	void remove_channel(std::string &name);
};

#endif