#include "../../includes/IRCserv.hpp"
#include "../../includes/Disconnect.hpp"
#include "../../includes/commands.hpp"

void	error_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	(void)serv;
	(void)line;
	squit_command(":" + server_it->get_servername() + " SQUIT :Server going down", server_it, serv);
}
