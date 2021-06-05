#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

void	ping_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() == 1)
	{
		client_it->push_to_buffer(create_msg(409, client_it, serv));
		return ;
	}
	else if (arg.size() == 2)
		client_it->push_to_buffer(":" + serv.get_hostname() + " PONG " + serv.get_hostname() + " :" + arg[1] + "\r\n");
	else //3 arg avec ducoup le server a qui il faut envoyer le ping
	{
		if (arg[2] == serv.get_hostname())
		{
			client_it->push_to_buffer(":" + serv.get_hostname() + " PONG " + serv.get_hostname() + " :" + arg[1] + "\r\n");
			return ;
		}

		std::list<Server>::iterator it = find_server_by_iterator(arg[2]);

		if (it == g_all.g_aServer.end())
		{
			client_it->push_to_buffer(create_msg(402, client_it, serv, arg[2]));
			return ;
		}
		if (it->get_hopcount() > 1)
			it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " PING " + arg[1] + " :" + arg[2] + "\r\n");
		else
			it->push_to_buffer(":" + client_it->get_nickname() + " PING " + arg[1] + " :" + arg[2] + "\r\n");
	}
}

void	ping_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() <= 2)
	{
		server_it->push_to_buffer(create_msg(409, server_it, serv));
		return ;
	}

	std::string nick = arg[0].substr(arg[0].find_first_of(':') + 1, arg[0].size());
	if (nick == "")
		nick = "*";

	else if (arg.size() == 3)
		server_it->push_to_buffer(":" + serv.get_hostname() + " PONG " + nick + " " + arg[2] + "\r\n");
	else //		:avan PING teest :irc.ircgod.com
	{
		std::string target = line.substr(line.find_first_of(':', 1) + 1, line.size());
		if (target == "")
			target = "*";

		if (target == serv.get_hostname())
		{
			server_it->push_to_buffer(":" + serv.get_hostname() + " PONG " + nick + " :" + arg[2] + "\r\n");
			return ;
		}

		std::list<Server>::iterator it = find_server_by_iterator(target);

		if (it == g_all.g_aServer.end())
		{
			server_it->push_to_buffer(create_msg(402, server_it, serv, target));
			return ;
		}
		if (it->get_hopcount() > 1)
			it->get_server_uplink()->push_to_buffer(arg[0] + " PING " + arg[2] + " :" + target + "\r\n");
		else
			it->push_to_buffer(arg[0] + " PING " + arg[2] + " :" + target + "\r\n");
	}
}
