#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

#define BUFFER_SIZE 4096

class Server {
private:
	const char* serverIp;
	std::string pw;
	sockaddr_in IP;
	char buffer[BUFFER_SIZE];
	int bytesRead;
	int clientSocket;
	sockaddr_in client;
	socklen_t clientSize;
public:
	Server(char *port, char *password);
	~Server();

	int socketId;
	std::string getPass() { return pw; }

};


#endif