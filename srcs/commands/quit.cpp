/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 12:27:36 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/21 18:23:46 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/Disconnect.hpp"
#include "../../includes/IRCserv.hpp"

static bool		is_user_in_any_channel(const std::string nickname)
{
	for (size_t i = 0; i < g_vChannel.size(); i++)
	{
		for (size_t j = 0; j < g_vChannel[i]._users.size(); j++)
		{
			if (g_vChannel[i]._users[j]->get_nickname() == nickname)
				return	true;
		}
	}
	return	false;
}

std::string		create_part_str(std::list<Client>::iterator client_it)
{
	std::string		part_string;

	for (size_t chann_idx = 0; chann_idx < g_vChannel.size(); chann_idx++)
	{
		for (std::deque<Client*>::iterator it = g_vChannel[chann_idx]._users.begin();
			it != g_vChannel[chann_idx]._users.end(); it++)
		{
			if ((*it)->get_nickname() == client_it->get_nickname())
			{
				part_string += g_vChannel[chann_idx].get_name() + ",";
			}
		}
	}
	return (part_string);
}

void			quit_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args;
	std::string					output;
	std::string					part_string;
	std::string					quit_msg_server;

	//Add nick to disconnected user deque
	add_disconnected_nick(client_it);
	args = ft_split(line, " ");
	part_string = create_part_str(client_it);
	quit_msg_server += ":" + client_it->get_nickname() + " QUIT " + ":leaving";
	if (quit_msg_server.find("\r\n") == std::string::npos)
		quit_msg_server += "\r\n";
	send_to_all_server(quit_msg_server, g_all.g_aServer.begin(), true);
	if (is_user_in_any_channel(client_it->get_nickname()) == true)
	{
		if (part_string.size() > 0)
			part_string.resize(part_string.size() - 1);
		if (args.size() == 1)
			part_command("PART " + part_string, client_it, serv);
		else
		{
			try
			{
				output = line.substr(line.find_first_of(':'), line.size());
			}
			catch (std::exception) { return; }
			part_command("PART " + part_string + " " + output, client_it, serv);
		}
	}
	throw QuitCommandException();
}

void		quit_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;
	std::string					part_string;

	(void)serv;
	(void)server_it;
	if (params.size() < 3)
		return ;
	if (params[1].size() < 2 || (client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	part_string = create_part_str(client_it);
	if (part_string != "" && params[3][0] == ':')
	{
		for (size_t i = 3; i < params.size(); i++)
			part_string += " " + params[i];
	}
	if (part_string != "")
		part_command("PART " + part_string, client_it, serv);
	add_disconnected_nick(client_it);
	send_to_all_server(line, server_it);
	client_it = g_all.g_aClient.erase(client_it);
}


void			quit_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	std::vector<std::string>	args;
	std::string					output;
	std::string					quit_msg_server;

	(void)serv;
	args = ft_split(line, " ");
	quit_msg_server += ":" + service_it->get_nickname() + " QUIT " + ":leaving";
	if (quit_msg_server.find("\r\n") == std::string::npos)
		quit_msg_server += "\r\n";
	send_to_all_server(quit_msg_server, g_all.g_aServer.begin(), true);
	throw QuitCommandException();
}
