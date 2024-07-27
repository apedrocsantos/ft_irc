#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 6667
#define BUFFER_SIZE 1024

class IRCServer {
public:
    IRCServer(int port);
    void start();

private:
    int server_fd;
    std::vector<int> clients;
    
    void handle_new_connection();
    void handle_client_message(int client_fd);
    void broadcast_message(const std::string& message, int exclude_fd = -1);
};

IRCServer::IRCServer(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

void IRCServer::start() {
    fd_set read_fds;
    int max_sd;

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        max_sd = server_fd;

        for (size_t i = 0; i < clients.size(); ++i) {
            int sd = clients[i];
            if (sd > 0) FD_SET(sd, &read_fds);
            if (sd > max_sd) max_sd = sd;
        }

        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
        }

        if (FD_ISSET(server_fd, &read_fds)) {
            handle_new_connection();
        }

        for (size_t i = 0; i < clients.size(); ++i) {
            int sd = clients[i];
            if (FD_ISSET(sd, &read_fds)) {
                handle_client_message(sd);
            }
        }
    }
}

void IRCServer::handle_new_connection() {
    int new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::cout << "New connection from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    clients.push_back(new_socket);
}

void IRCServer::handle_client_message(int client_fd) {
    char buffer[BUFFER_SIZE];
    int valread = read(client_fd, buffer, BUFFER_SIZE);
    
    if (valread == 0) {
        close(client_fd);
        clients.erase(std::remove(clients.begin(), clients.end(), client_fd), clients.end());
        std::cout << "Client disconnected" << std::endl;
    } else {
        buffer[valread] = '\0';
        std::string message(buffer);
        std::cout << "Received: " << message;
        broadcast_message(message, client_fd);
    }
}

void IRCServer::broadcast_message(const std::string& message, int exclude_fd) {
    for (size_t i = 0; i < clients.size(); ++i) {
        int sd = clients[i];
        if (sd != exclude_fd) {
            send(sd, message.c_str(), message.length(), 0);
        }
    }
}

int main() {
    IRCServer server(PORT);
    server.start();
    return 0;
}
