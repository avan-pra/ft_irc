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
