#include "../../includes/IRCserv.hpp"

void	server_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::cout << "|" << line << "|" << std::endl;
	(void)line;
	(void)server_it;
	(void)serv;
}
