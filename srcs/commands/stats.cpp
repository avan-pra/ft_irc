#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void			stats_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	(void)line;
	(void)client_it;
	(void)serv;
}

void	stats_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	(void)line;
	(void)server_it;
	(void)serv;
}
