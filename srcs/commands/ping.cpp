#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

void	ping_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");
	
	try
	{
		client_it->push_to_buffer(":" + serv.get_hostname() + " PONG " + serv.get_hostname() + " " + arg[1] + "\r\n");
	}
	catch(const std::exception& e) { }
}

void	ping_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	try
	{
		server_it->push_to_buffer(":" + serv.get_hostname() + " PONG " + serv.get_hostname() + "\r\n");
	}
	catch(const std::exception& e) { }
}
