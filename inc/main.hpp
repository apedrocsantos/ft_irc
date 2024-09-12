#pragma once

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
#include <set>
#include <list>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

#include "lists.hpp"
#include "colors.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "CmdHandler.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include "replies.hpp"

// #define BACKLOG 10
#define MESSAGE_BUFFER_SIZE 512
#define NICKNAME_SIZE 16
#define CHANNEL_NAME_SIZE 50

#endif