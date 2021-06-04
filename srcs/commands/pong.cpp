#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <ctime>

void	pong_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	(void)serv;
	if (arg.size() == 1)
	{
		client_it->push_to_buffer(create_msg(409, client_it, serv));
		return ;
	}
	time(&client_it->get_last_activity());
	client_it->set_ping_status(false);
}

void	pong_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	(void)serv;
	if (arg.size() == 1)
	{
		server_it->push_to_buffer(create_msg(409, server_it, serv));
		return ;
	}
	time(&server_it->get_last_activity());
	server_it->set_ping_status(false);
}
