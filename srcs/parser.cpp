#include <iostream>
#include "../includes/IRCserv.hpp"
#include "../includes/commands.hpp"

static void	build_unfinished_packet(const std::string &true_line, const size_t &client_idx, std::string &last)
{
	// si il y a un crlf en fin de string alors c'est good, on stock R
	// si il n'y a pas de crlf meme apres concatenation alors on stock et on enleve le dernier du ft_split (qui n'est donc pas un packet complet)
	if (true_line.size() > 2 && *(true_line.end() - 1) == '\n' && *(true_line.end() - 2) == '\r')
		g_aClient[client_idx].second.set_unended_packet("");
	else
	{
		g_aClient[client_idx].second.set_unended_packet(last);
		last = std::string("");
	}
}

void	parser(char *line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string>	packet;
	std::string					true_line;

	//add old unfinished packet if any exist
	true_line = g_aClient[client_idx].second.get_unended_packet() + std::string(line);

	packet = ft_split(true_line, std::string("\r\n"));

	if (packet.size() != 0)
	{
		build_unfinished_packet(true_line, client_idx, packet.back());
		for (std::vector<std::string>::iterator str = packet.begin(); str != packet.end(); ++str)
		{
			std::string command = std::string(str->substr(0, str->find(" ", 0)));

			try
			{
				if (serv.get_command().at(command) != NULL)
					serv.get_command().at(command)(*str, client_idx, serv);
			}
			catch (const std::exception &e) { g_aClient[client_idx].second.send_reply(create_error(421, client_idx, serv, command)); } //il faut envoyer ca au client
		}
	}
}
