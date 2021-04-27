#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include <ctime>

static void	handle_wrong_command(std::string &command, const size_t connection_idx, const MyServ &serv)
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
			g_aUnregistered[connection_idx].second.send_reply(err);
		}
		catch (const std::exception &e) 
		{
			std::string err = ":" + serv.get_hostname() + " " + ft_to_string(421) + " * " + command + " :Unknown command\r\n";
			g_aUnregistered[connection_idx].second.send_reply(err);
		}
	}
}

void	connection_parser(char *line, const size_t &connection_idx, const MyServ &serv)
{
	std::vector<std::string>	packet;
	std::string					true_line;
	Connection					&co = g_aUnregistered[connection_idx].second;

	true_line = g_aUnregistered[connection_idx].second.get_unended_packet() + std::string(line);
	packet = ft_split(true_line, std::string("\r\n"));
	if (packet.size() != 0)
	{
		build_unfinished_packet(true_line, g_aUnregistered[connection_idx].second, packet.back());
		clear_empty_packet(packet);
		for (std::vector<std::string>::iterator str = packet.begin(); str != packet.end(); ++str)
		{
			std::string command = std::string(str->substr(0, str->find(" ", 0)));

			true_line.erase(0, str->size() + 2); //erase the current packet from the starting string

			for (std::string::iterator it = command.begin(); it != command.end(); ++it)
				*it = std::toupper(*it);
			if (command == "NICK" || command == "USER" || (command == "PASS" && ft_split(*str, " ").size() <= 2))
			{
				Client cli = co;

				cli.set_unended_packet(*str + "\r\n" + true_line + co.get_unended_packet());
				g_aClient.push_back(std::make_pair(cli._fd, cli));
				throw NewClientException();
			}
			if (command == "SERVER" || (command == "PASS" && ft_split(*str, " ").size() > 2))
			{
				Server srv = co;

				srv.set_unended_packet(*str + "\r\n" + true_line + co.get_unended_packet());
				g_aServer.push_back(std::make_pair(srv._fd, srv));
				throw NewServerException();
			}
			try
			{
				handle_wrong_command(command, connection_idx, serv);
			}
			catch (QuitCommandException) { throw QuitCommandException(); }
		}
	}
}

static bool	check_register_timeout(Connection &co)
{
	time_t time_compare; //may be optimized better

	time(&time_compare);
	if (time_compare - co.get_last_activity() > TIMEOUT_REGISTER)
		return true;
	return false;
}

void	iterate_connection(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (size_t i = 0; i != g_aUnregistered.size(); ++i)
	{
		if (check_register_timeout(g_aUnregistered[i].second) == true)
			disconnect_connection(i);
		else if (is_readable(serv, g_aUnregistered[i].second))
		{
			get_message(c, g_aUnregistered[i].second, ret);
			if (ret <= 0)
				disconnect_connection(i);
			else if (ret > 0)
			{
				try
				{
					connection_parser(c, i, serv);
				}
				catch (NewServerException) { g_aUnregistered.erase(g_aUnregistered.begin() + i); i--; }
				catch (NewClientException) { g_aUnregistered.erase(g_aUnregistered.begin() + i); i--; }
				catch (QuitCommandException) { disconnect_connection(i); }
			}
		}
	}
}
