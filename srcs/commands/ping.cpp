#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

void	ping_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");
	
	try
	{
		g_aClient[client_idx].second.send_reply(":" + serv.get_hostname() + " PONG " + serv.get_hostname() + " " + arg[1] + "\r\n");
	}
	catch(const std::exception& e) { }
}
