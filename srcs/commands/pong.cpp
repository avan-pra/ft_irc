#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <ctime>

void	pong_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	(void)line;
	(void)serv;
	time(&client_it->get_last_activity());
	client_it->set_ping_status(false);
}

void	pong_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	(void)line;
	(void)serv;
	time(&server_it->get_last_activity());
	server_it->set_ping_status(false);
}
