#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <ctime>

void	pong_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	(void)line;
	(void)serv;
	time(&g_aClient[client_idx].second.get_last_activity());
	g_aClient[client_idx].second.set_ping_status(false);
}
