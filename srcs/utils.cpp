#include "../inc/main.hpp"

	int check_valid_port(char *port)
{
	int i = 0;
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (1);
		i++;
	}
	if (atoi(port) <= 1024 || atoi(port) > 65535)
        return (1);
	return (0);
}
