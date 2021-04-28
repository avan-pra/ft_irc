#include <iostream>
#include "../includes/IRCserv.hpp"
#include "../includes/commands.hpp"
#include <algorithm>
#include <cstring>

void	build_unfinished_packet(const std::string &true_line, Connection &co, std::string &last)
{
	// si il y a un crlf en fin de string alors c'est good, on stock R
	// si il n'y a pas de crlf meme apres concatenation alors on stock et on enleve le dernier du ft_split (qui n'est donc pas un packet complet)
	if (true_line.size() > 2 && *(true_line.end() - 1) == '\n' && *(true_line.end() - 2) == '\r')
		co.set_unended_packet("");
	else
	{
		co.set_unended_packet(last);
		last = std::string("");
	}
}

void	clear_empty_packet(std::vector<std::string> &packet)
{
	size_t						i = 0;
	std::vector<std::string>	clear;

	for (std::vector<std::string>::iterator it = packet.begin(); it != packet.end(); it++)
	{
		i = 0;
		while ((*it)[i] && (*it)[i] == ' ')
			i++;
		if (i != it->size())
			clear.push_back(*it);
	}
	packet.swap(clear);
}

// void hold_if_cap(std::vector<std::string> &packet, const size_t &client_idx)
// a degager ^^

bool	can_execute(const std::string command, const size_t &client_idx, const MyServ &serv)
{
	bool ret = false;

	//ptet opti le search
	if ((g_aClient[client_idx].second.is_registered() == false && (command == "PASS" || command == "NICK"
				|| command == "USER" || command == "CAP" || command == "SERVER" || command == "QUIT" || command == "PONG"))
		|| g_aClient[client_idx].second.is_registered() == true)
		ret = true;
	try
	{
		if (serv.get_command().at(command) == NULL)
			return false;
	}
	catch (const std::exception &e) { throw std::exception(); }
	if (ret == false)
		g_aClient[client_idx].second.push_to_buffer(create_msg(451, client_idx, serv));
	return ret;
}

void	parser(char *line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	packet;
	std::string					true_line;

	//add old unfinished packet if any exist
	true_line = g_aClient[client_idx].second.get_unended_packet() + std::string(line);

	packet = ft_split(true_line, std::string("\r\n"));

	if (packet.size() != 0)
	{
		build_unfinished_packet(true_line, g_aClient[client_idx].second, packet.back());
		clear_empty_packet(packet);
		for (std::vector<std::string>::iterator str = packet.begin(); str != packet.end(); ++str)
		{
			std::string command = std::string(str->substr(0, str->find(" ", 0)));

			//put to uppercase letter the command (irssi send in lower case for example)
			for (std::string::iterator it = command.begin(); it != command.end(); ++it)
				*it = std::toupper(*it);
			try
			{
				/*
				** execute command only if: ((if not registered and command are either PASS NICK or USER)
				** or if register) AND if command exist
				*/
				if (can_execute(command, client_idx, serv) == true)
					serv.get_command().at(command)(*str, client_idx, serv);
			}
			catch (const IncorrectPassException &e) { throw IncorrectPassException(); }
			catch (const QuitCommandException &e) { throw QuitCommandException(); }
			catch (const std::exception &e) { g_aClient[client_idx].second.push_to_buffer(create_msg(421, client_idx, serv, command)); } //il faut envoyer ca au client
		}
	}
}
