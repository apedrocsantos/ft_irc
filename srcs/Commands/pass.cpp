#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::pass(Command *cmd, Client *client, Server *server) {
	(void) client;
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string pass;

	if (params.empty())
		return server->add_to_out_buf(client->getFd(), ERR_NEEDMOREPARAMS(cmd, client));

	ss >> pass;
	if (client->get_auth() == false) // if user not authenticated
	{
		if (server->get_pwd() != pass) // if wrong password
			server->add_to_out_buf(client->getFd(), ERR_PASSWDMISMATCH());
		else
			client->set_auth(true);
	}
	else // if user authenticated
		server->add_to_out_buf(client->getFd(), ERR_AlreadyRegistered(client));
}