#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <cstdlib>
#include <vector>
#include <map>
#include <utility>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <poll.h>
#include "lists.hpp"

#include "colors.hpp"
#include "Client.hpp"
#include "CmdList.hpp"
#include "Command.hpp"

// #define BACKLOG 10
#define MESSAGE_BUFFER_SIZE 512
#define NICKNAME_SIZE 16
#define CHANNEL_NAME_SIZE 50
#define PASSWORD "password"

int check_valid_port(char *port);

#endif