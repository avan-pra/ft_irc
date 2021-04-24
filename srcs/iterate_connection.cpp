#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include <ctime>

static void	get_connection_message(char *c, const size_t &i, int &ret)
{
	ft_bzero((char *)c, sizeof(c));
	if (!(g_aUnregistered[i].second.get_tls()) || (g_aUnregistered[i].second.get_tls() &&
				SSL_is_init_finished(g_aUnregistered[i].second._sslptr)))
			ret = receive_message(g_aUnregistered[i].second, c);
	else
			ret = DoHandshakeTLS(i);
	//int ret = recv(g_aClient[i].first, &c, BUFF_SIZE, 0);
	// std::cout << "recv" << std::endl;
}

void	connection_parser(char *line, const size_t &connection_idx, const MyServ &serv)
{
	std::vector<std::string>	packet;
	std::string					true_line;
	Connection					&co = g_aUnregistered[connection_idx].second;

	//add old unfinished packet if any exist
	true_line = g_aUnregistered[connection_idx].second.get_unended_packet() + std::string(line);
	packet = ft_split(true_line, std::string("\r\n"));
	if (packet.size() != 0)
	{
		build_unfinished_packet(true_line, g_aUnregistered[connection_idx].second, packet.back());
		clear_empty_packet(packet);
		for (std::vector<std::string>::iterator str = packet.begin(); str != packet.end(); ++str)
		{
			std::string command = std::string(str->substr(0, str->find(" ", 0)));

			// true_line.erase(0, str->size());
			//put to uppercase letter the command (irssi send in lower case for example)
			for (std::string::iterator it = command.begin(); it != command.end(); ++it)
				*it = std::toupper(*it);
			if (command == "NICK" || command == "USER" || (command == "PASS" && ft_split(*str, " ").size() <= 2))
			{
				char test[1] = { '\0' };
				co.set_unended_packet(true_line);
				// std::cout << co.get_unended_packet() << std::endl;
				Client cli = co;
				g_aClient.push_back(std::make_pair(cli._fd, cli));
				parser(test, g_aClient.size() - 1, serv);
				throw NewClientException();
			}
			if (command == "SERVER" || (command == "PASS" && ft_split(*str, " ").size() > 2))
			{
				co.set_unended_packet(*str + true_line + co.get_unended_packet());
				Server srv = co;
				g_aServer.push_back(std::make_pair(srv._fd, srv));
				throw NewServerException();
			}
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
					g_aUnregistered[connection_idx].second.send_reply("not found");
				}
			}
		}
	}
}

void	iterate_connection(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (size_t i = 0; i != g_aUnregistered.size(); ++i)
	{
		if (kick_if_away(g_aUnregistered[i].second) == true)
			disconnect_connection(i);
		else if (FD_ISSET(g_aUnregistered[i].first, &serv.get_readfs()))
		{
			get_connection_message(c, i, ret);
			if (ret <= 0)
				disconnect_connection(i);
			else if (ret > 0)
			{
				c[ret] = '\0';
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
