/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 12:27:36 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/01 15:49:28 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

void	quit_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> args;
	std::string	output;
	std::string	part_string;
	
	args = ft_split(line, " ");
	for (int chann_idx = 0; chann_idx < g_vChannel.size(); chann_idx++)
	{
		for (std::vector<Client>::iterator it = g_vChannel[chann_idx]._users.begin();
			it != g_vChannel[chann_idx]._users.end(); )
		{
			if (it->get_nickname() == g_aClient[client_idx].second.get_nickname())
			{
				part_string += g_vChannel[channel_idx].get_name() + ",";
			}
			else
				it++;
		}
	}
	std::cout << part_string << std::endl;
	if (args.size() == 1)
	{
		throw QuitCommandException();
	}
	else
	{
		for (size_t i = 1; i < args.size() ; i++)
			output += args[i] + " ";
		throw QuitCommandException();
	}
}
