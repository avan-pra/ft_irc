#include <iostream>
#include "../includes/IRCserv.hpp"
#include "../includes/commands.hpp"
#include <algorithm>
#include <cstring>

bool	can_execute(const std::string command, std::list<Service>::iterator service_it, const MyServ &serv)
{
	bool ret = false;

	//ptet opti le search
	if ((service_it->is_registered() == false && (command == "PASS" || command == "SERVICE" || command == "QUIT"))
		|| service_it->is_registered() == true)
		ret = true;
	try
	{
		if (serv.get_command_service().at(command) == NULL)
			return false;
	}
	catch (const std::exception &e) { throw std::exception(); }
	if (ret == false)
		service_it->push_to_buffer(create_msg(451, service_it, serv));
	return ret;
}

void	service_parser(char *line, std::list<Service>::iterator service_it, MyServ &serv)
{
	std::vector<std::string>	packet;
	std::string					true_line;

	//add old unfinished packet if any exist
	true_line = service_it->get_unended_packet() + std::string(line);

	packet = ft_split(true_line, std::string("\r\n"));

	if (packet.size() != 0)
	{
		build_unfinished_packet(true_line, *service_it, packet.back());
		clear_empty_packet(packet);
		for (size_t i = 0; i < packet.size(); ++i)
		{
			std::string command = true_command(packet[i]);

			//put to uppercase letter the command (irssi send in lower case for example)
			for (std::string::iterator it = command.begin(); it != command.end(); ++it)
				*it = std::toupper(*it);
			
			for (size_t j = 0; packet[i][j] != ' ' && packet[i][j] != '\0'; j++)
				packet[i][j] = std::toupper(packet[i][j]);

			try
			{
				service_it->inc_number_of_messages_received();	
				serv.get_use_per_command().at(command)._count++;
			}
			catch (const std::exception &e) { }

			try
			{
				/*
				** execute command only if: ((if not registered and command are either PASS NICK or USER)
				** or if register) AND if command exist
				*/
				if (can_execute(command, service_it, serv) == true)
					serv.get_command_service().at(command)(packet[i], service_it, serv);
			}
			catch (const IncorrectPassException &e) { throw IncorrectPassException(); }
			catch (const QuitCommandException &e) { throw QuitCommandException(); }
			catch (const std::exception &e) { service_it->push_to_buffer(create_msg(421, service_it, serv, command)); } //il faut envoyer ca au client
		}
	}
}
