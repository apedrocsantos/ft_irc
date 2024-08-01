#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

class Server {
private:
	const char* serverIp;
	std::string pw;
	sockaddr_in IP;
public:
	Server(char *port, char *password);
	~Server();

	int socketId;
	std::string getPass() { return pw; }

};


#endif