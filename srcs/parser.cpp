#include <iostream>
#include "../includes/IRCserv.hpp"

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
	std::string	*packet;
	std::string true_line;

	//add old unfinished packet if any exist
	true_line = g_aClient[client_idx].second.get_unended_packet() + std::string(line);

	packet = ft_split(true_line, std::string("\r\n"));

	size_t i = 0;
	while (packet[i] != std::string(""))
		++i;
	build_unfinished_packet(true_line, client_idx, packet[i - 1]);

	for (size_t i = 0; packet[i] != std::string(""); ++i)
	{
		try
		{
			std::cout << serv.get_command().at(packet[i].substr(0, packet[i].find(" ", 0))) << std::endl;
		}
		catch (const std::exception &e) { std::cout <<" ERROR pas connu" << std::endl; }
	}
	delete [] packet;
}
