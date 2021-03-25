#include <iostream>
#include "../includes/IRCserv.hpp"
#include "../includes/commands.hpp"
#include <algorithm>
#include <cstring>

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

void hold_if_cap(std::vector<std::string> &packet, const size_t &client_idx)
{
	for (int i = 0; i < packet.size(); ++i)
	{
		std::vector<std::string> line = ft_split(packet[i], " ");
		try
		{
			if (line[0] == "CAP" && line[1] != "END")
				g_aClient[client_idx].second.set_cap(true);
			else if (line[0] == "CAP" && line[1] == "END")
			{
				g_aClient[client_idx].second.set_cap(false);
				std::vector<std::string> holded = ft_split(g_aClient[client_idx].second.hold_packet, "\r\n");
				for (int j = 0; j < holded.size(); ++j)
					packet.push_back(holded[j]);
				g_aClient[client_idx].second.hold_packet = std::string("");
			}
			if (g_aClient[client_idx].second.is_cap() == true)
			{
				if (line[0] != "CAP")
				{
					g_aClient[client_idx].second.hold_packet = g_aClient[client_idx].second.hold_packet + packet[i] + "\r\n";
					packet[i] = "";
				}
			}
		}
		catch(const std::exception& e) { }
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
		// hold_if_cap(packet, client_idx);
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
				if (((g_aClient[client_idx].second.is_registered() == false && (command == "PASS" || command == "NICK" || command == "USER" || command == "CAP"))
					|| g_aClient[client_idx].second.is_registered() == true)
					&& serv.get_command().at(command) != NULL)
					serv.get_command().at(command)(*str, client_idx, serv);
			}
			catch (const IncorrectPassException &e) { throw IncorrectPassException(); }
			catch (const QuitCommandException &e) {throw QuitCommandException(); }
			catch (const std::exception &e) { g_aClient[client_idx].second.send_reply(create_msg(421, client_idx, serv, command)); } //il faut envoyer ca au client
		}
	}
}
