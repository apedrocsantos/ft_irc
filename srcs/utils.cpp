#include "../inc/main.hpp"

bool substring(char *str, const char *subst)
{
    int i = 0;
    int j = 0;
    while (str[i])
    {
        while (subst[j] && str[i + j] == subst[j])
        {
            j++;
            if (!subst[j])
                return true;
        }
        j = 0;
        i++;
    }
    return false;
}

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