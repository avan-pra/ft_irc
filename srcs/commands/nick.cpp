#include "../../includes/commands.hpp"

void   nick_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::string arg;

	//try to remove "NICK " from string
	try { arg = line.substr(5); }
	catch (const std::exception &e) { }

	//si la string est vide
	if (arg.find_first_not_of(' ') == arg.npos)
		g_aClient[client_idx].second.send_reply(create_error(431, client_idx, serv));

	// ERR_ERRONEUSNICKNAME
	// ERR_NICKNAMEINUSE
	// ERR_NICKCOLLISION
}
