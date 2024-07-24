#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <mutex>

#define PORT 6667

std::vector<int> clients;
std::mutex clients_mutex;

void handle_client(int client_socket) {
	char buffer[1024];
	int bytes_read;

	while ((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytes_read] = '\0';

		std::lock_guard<std::mutex> guard(clients_mutex);
		for (int client : clients) {
			if (client != client_socket) {
				send(client, buffer, bytes_read, 0);
			}
		}
	}

	close(client_socket);

	std::lock_guard<std::mutex> guard(clients_mutex);
	clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
}

int main() {
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_size;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error binding socket" << std::endl;
		return -1;
	}

	if (listen(server_socket, 10) < 0) {
		std::cerr << "Error listening on socket" << std::endl;
		return -1;
	}

	std::cout << "Server listening on port " << PORT << std::endl;

	while (true) {
		client_addr_size = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);

		if (client_socket < 0) {
			std::cerr << "Error accepting client" << std::endl;
			continue;
		}

		std::lock_guard<std::mutex> guard(clients_mutex);
		clients.push_back(client_socket);

		std::thread(handle_client, client_socket).detach();
	}

	close(server_socket);
	return 0;
}
