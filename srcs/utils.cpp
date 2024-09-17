#include "../inc/main.hpp"

	int check_valid_port(char *port)
{
	int i = -1;
	while (port[++i])
	{
		if (port[i] == '+')
			continue;
		if (!std::isdigit(port[i]))
			return (1);
	}
	if (std::atoi(port) <= 1024 || std::atoi(port) > 65535)
        return (1);
	return (0);
}

std::string ft_itoa(int nb)
{
	std::string output;
	while (nb)
	{
		output.insert(output.begin(), nb % 10 + '0');
		nb /= 10;
	}
	return output;
}