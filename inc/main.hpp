#ifndef MAIN_HPP
# define MAIN_HPP

# include "header.hpp"

# include <iostream>
# include <unistd.h>
# include <string>
# include <cstring>
# include <csignal>

# include <vector>
# include <poll.h>
# include <sstream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <cstdlib>
# include <fcntl.h>
# include <map>
# include <filesystem>
# include <cstdio>
# include <ctime>
# include <algorithm>

# include "Server.hpp"


# define DPORT 6667
# define IPDEFAULT "127.0.0.1"

int check_valid_port(char *port);

#endif