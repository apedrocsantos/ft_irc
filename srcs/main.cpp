#include "../inc/main.hpp"

int main(int ac, char **argv)
{
    if (ac != 3)
    {
        std::cout << RED << "ERROR" << RESET << ": " << ORANGE << "To run the program do: ./ircserv <PORT> <PASSWORD>" << RESET << std::endl;
        return 1;
    }
    if (check_valid_port(argv[1]))
    {
        std::cout << "ERROR: invalid port\n";
        return 1;
    }
    std::string pwd(argv[2]);
    Server server(argv[1], pwd);
    server.start();
    return 0;
}