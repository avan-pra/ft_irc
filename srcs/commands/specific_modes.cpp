/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specific_modes.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:11:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/13 13:52:42 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <string>
#include <algorithm>
#include <cstring>

void			mode_o(const size_t &client_idx, const size_t &chann_idx, const bool minus, const std::string name, const std::string chan_name, const MyServ &serv)
{
	if (minus == true)
	{
		g_vChannel[chann_idx].remove_user_operator(name);
		send_to_channel("MODE " + chan_name + " -o " + name, client_idx, serv, chann_idx, false);
		g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
			+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " MODE " + chan_name + " -o " + name + "\r\n");
	}
	else
	{
		size_t cli_to_add_idx;

		cli_to_add_idx = find_user_by_nick(name);
		g_vChannel[chann_idx]._operator.push_back(&g_aClient[cli_to_add_idx].second);
		send_to_channel("MODE " + chan_name + " +o " + name, client_idx, serv, chann_idx, false);
		g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
			+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " MODE " + chan_name + " +o " + name + "\r\n");
	}
}

void		mode_b(const size_t &client_idx, const size_t &chann_idx, const bool minus, const std::string name, const std::string chan_name, const MyServ &serv)
{
	if (minus == true)
	{
		g_vChannel[chann_idx].remove_user_ban(name);
		send_to_channel("MODE " + chan_name + " -b" + name, client_idx, serv, chann_idx);
		g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
			+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " MODE " + chan_name + " -b " + name + "\r\n");
	}
	else
	{
		size_t cli_to_add_idx;

		cli_to_add_idx = find_user_by_nick(name);
		g_vChannel[chann_idx]._ban.push_back(&g_aClient[cli_to_add_idx].second);
		send_to_channel("MODE " + chan_name + " +b " + name, client_idx, serv, chann_idx, false);
		g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
			+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " MODE " + chan_name + " +b " + name + "\r\n");
	}
}
