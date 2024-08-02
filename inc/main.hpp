#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <ctype.h>
#include "lists.hpp"
#include "colors.hpp"
#include "Server.hpp"

#define BACKLOG 10
#define MESSAGE_BUFFER_SIZE 512
#define NICKNAME_SIZE 9
#define CHANNEL_NAME_SIZE 50
#define PASSWORD "password"

int check_valid_port(char *port);
void exit_routine(std::string error, int serverSocket);

#endif