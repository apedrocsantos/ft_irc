#include "../inc/Server.hpp"

Server::Server(char *port, char *password) {
	if(check_valid_port(port)) {
		std::cerr << "Invalid port\n";
		exit(EXIT_FAILURE);
	}
	int PORT = atoi(port);
	pw = std::string(password);
	serverIp = IPDEFAULT;
	socketId = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
	if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		std::cerr << "Error setting socket options\n";
		close(socketId);
		exit(EXIT_FAILURE);
	}
	if (socketId == -1) {
		std::cerr << "Can't create a socket!";
		exit(EXIT_FAILURE);
	}

	fcntl(socketId, F_SETFL, O_NONBLOCK);

	IP.sin_family = AF_INET;
	if (inet_pton(AF_INET, serverIp, &IP.sin_addr) <= 0) {
		std::cerr << "Invalid address/Address not supported\n";
		exit(EXIT_FAILURE);
	}
	IP.sin_port = htons(PORT);
	if (bind(socketId, (sockaddr * ) & IP, sizeof(IP)) == -1) {
		std::cerr << "Can't bind to IP/port";
		exit(EXIT_FAILURE);
	}
	if (listen(socketId, SOMAXCONN) == -1) {
		std::cerr << "Can't listen!";
		exit(EXIT_FAILURE);
	}
}

Server::~Server() {
	close(socketId);
}
