#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::pass(Command *cmd, Client *client, Server *server) {
	(void) client;
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string pass;

	ss >> pass;

	if (client->get_auth() == false)
	{
		if (server->get_pwd() != pass)
			server->add_to_out_buf(client->getFd(), ERR_PASSWDMISMATCH(client));
		else
			client->set_auth(true);
	}
	else
		server->add_to_out_buf(client->getFd(), ERR_AlreadyRegistered(client));
}