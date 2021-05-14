/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 15:53:28 by lucas             #+#    #+#             */
/*   Updated: 2021/05/14 13:48:38 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"
#include "../includes/commands.hpp"

bool	can_execute(const std::string command, std::list<Server>::iterator server_it, const MyServ &serv)
{
	bool ret = false;

	//ptet opti le search
	if ((server_it->is_registered() == false && (command == "PASS" || command == "CAP" || command == "SERVER" || command == "QUIT" || command == "PONG"))
		|| server_it->is_registered() == true)
		ret = true;
	try
	{
		if (serv.get_command_server().at(command) == NULL)
			return false;
	}
	catch (const std::exception &e) { throw std::exception(); }
	if (ret == false)
		server_it->push_to_buffer(create_msg(451, server_it, serv));
	return ret;
}

std::string		true_command(const std::string &line)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() == 0)
		return ("");
	if (params.size() == 1)
		return (params[0]);
	if (params[0].size() > 1 && params[0][0] == ':')
		return (params[1]);
	else
		return (params[0]);
}

void	server_parser(char *line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::string					true_line;
	std::vector<std::string>	packet;

	true_line = server_it->get_unended_packet() + std::string(line);
	packet = ft_split(true_line, std::string("\r\n"));

	if (packet.size() != 0)
	{

		build_unfinished_packet(true_line, *server_it, packet.back());
		clear_empty_packet(packet);

		for (std::vector<std::string>::iterator str = packet.begin(); str != packet.end(); ++str)
		{
			std::string command = true_command(*str);

			std::cout << command << std::endl;
			for (std::string::iterator it = command.begin(); it != command.end(); ++it)
				*it = std::toupper(*it);

			try
			{
				if (can_execute(command, server_it, serv) == true)
					serv.get_command_server().at(command)(*str, server_it, serv);
			}
			catch (const std::exception &e) { server_it->push_to_buffer(create_msg(421, server_it, serv, command)); }
		}
	}
}