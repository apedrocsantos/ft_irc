#include "../../inc/main.hpp"

class CmdHandler;

void CmdHandler::pass(Command *cmd, Client *client, Server *server) {
	(void) client;
	std::string params = cmd->get_params();
	std::stringstream ss(params);
	std::string pass;

	ss >> pass;

	if (server->get_pwd() != pass) std::cout << "some error idk, maybe ERR_PassWdMisMatch" << std::endl;

	// continue
	// send pass success
}