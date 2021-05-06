/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 12:27:36 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/06 11:27:57 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

static bool	is_user_in_any_channel(const std::string nickname)
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

void		quit_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args;
	std::string					output;
	std::string					part_string;

	//Add nick to disconnected user deque
	add_disconnected_nick(client_it);
	args = ft_split(line, " ");
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
