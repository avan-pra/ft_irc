#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

void	ping_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	g_aClient[client_idx].second.send_reply("PONG " + serv.get_hostname() + "\r\n");
}