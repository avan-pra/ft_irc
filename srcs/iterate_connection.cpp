#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Disconnect.hpp"
#include <ctime>

static void	handle_wrong_command(std::string &command, std::list<Unregistered>::iterator unregistered_it, const MyServ &serv)
{
	if (command == "QUIT")
	{
		throw QuitCommandException();
	}
	if (command == "CAP")
		;
	else
	{
		try
		{
			serv.get_command().at(command);
			std::string err = ":" + serv.get_hostname() + " " + ft_to_string(451) + " * :You have not registered\r\n";
			unregistered_it->push_to_buffer(err);
		}
		catch (const std::exception &e) 
		{
			std::string err = ":" + serv.get_hostname() + " " + ft_to_string(421) + " * " + command + " :Unknown command\r\n";
			unregistered_it->push_to_buffer(err);
		}
	}
}

void	connection_parser(char *line, std::list<Unregistered>::iterator unregistered_it, const MyServ &serv)
{
	std::vector<std::string>	packet;
	std::string					true_line;
	Unregistered					&co = *unregistered_it;

	true_line = unregistered_it->get_unended_packet() + std::string(line);
	packet = ft_split(true_line, std::string("\r\n"));
	if (packet.size() != 0)
	{
		build_unfinished_packet(true_line, *unregistered_it, packet.back());
		clear_empty_packet(packet);
		for (std::vector<std::string>::iterator str = packet.begin(); str != packet.end(); ++str)
		{
			std::string command = std::string(str->substr(0, str->find(" ", 0)));

			true_line.erase(0, str->size() + 2); //erase the current packet from the starting string

			for (std::string::iterator it = command.begin(); it != command.end(); ++it)
				*it = std::toupper(*it);
			if (command == "NICK" || command == "USER")
			{
				Client cli = co;

				cli.set_unended_packet(co.get_unended_packet() + *str + "\r\n" + true_line);
				g_all.g_aClient.push_back(cli);
				throw NewClientException();
			}
			if (command == "SERVER")
			{
				Server srv = co;

				srv.set_unended_packet(co.get_unended_packet() + *str + "\r\n" + true_line);
				g_all.g_aServer.push_back(srv);
				throw NewServerException();
			}
			if (command == "PASS")
			{
				;
			}
			try
			{
				handle_wrong_command(command, unregistered_it, serv);
			}
			catch (QuitCommandException) { throw QuitCommandException(); }
		}
	}
}

bool	check_register_timeout(Connection &co, const MyServ &serv)
{
	time_t time_compare; //may be optimized better

	if (co.is_registered() == true)
		return false;
	time(&time_compare);
	if (time_compare - co.get_last_activity() > serv.get_timeout_register())
		return true;
	return false;
}

void	iterate_connection(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); ++it)
	{
		if (check_register_timeout(*it, serv) == true)
			disconnect(&(*it), it);
		else if (is_readable(serv, *it))
		{
			get_message(c, *it, ret);
			// FD_CLR(it->_fd, &serv.get_readfs());
			check_message_problem(c, *it, serv, ret);
			if (ret <= 0)
				disconnect(&(*it), it);
			else if (ret > 0)
			{
				try
				{
					connection_parser(c, it, serv);
				}
				catch (NewServerException)
				{
					it = g_all.g_aUnregistered.erase(it);
				}
				catch (NewClientException)
				{
					it = g_all.g_aUnregistered.erase(it);
				}
				catch (QuitCommandException) { disconnect(&(*it), it); }
			}
		}
	}
}
