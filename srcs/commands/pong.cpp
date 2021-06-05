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
	if (arg[1].substr(1, arg[1].size()) == serv.get_hostname() || arg[1] == serv.get_hostname())
	{
		time(&client_it->get_last_activity());
		client_it->set_ping_status(false);
	}
}

void	pong_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	(void)serv;
	if (arg.size() <= 3)
	{
		server_it->push_to_buffer(create_msg(409, server_it, serv));
		return ;
	}
	else if (arg.size() == 4)
	{
		if (arg[2] == serv.get_hostname())
		{
			if (line.substr(line.find_first_of(':', 1) + 1, line.size()) == serv.get_hostname())
			{
				time(&server_it->get_last_activity());
				server_it->set_ping_status(false);
			}
			return ;
		}

		std::list<Client>::iterator it = find_client_by_iterator(arg[2]);
		if (it != g_all.g_aClient.end())
		{
			//pour le client //:irc.ircgod.com PONG irc.ircgod.com :test
			//pour le serv   //:irc.example.net PONG avan :test
			if (it->get_hopcount() > 0)
				it->get_server_uplink()->push_to_buffer(line + "\r\n");
			else
			{
				std::string home_serv = arg[0].substr(arg[0].find_first_of(':') + 1, arg[0].size());
				it->push_to_buffer(":" + home_serv + " PONG " + home_serv + " " + arg[3] + "\r\n");
			}
		}
		else
			;//on l'a pas find
	}
}
