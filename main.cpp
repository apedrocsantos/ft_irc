#include <iostream>
#include <sys/socket.h>

int main(int ac, char **av)
{
    (void) av;
    std::cout << AF_INET << std::endl;
    if (ac != 3)
        return 1;
    return 0;
}