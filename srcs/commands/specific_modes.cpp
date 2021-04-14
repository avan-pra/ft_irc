/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specific_modes.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:11:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/14 16:13:01 by jvaquer          ###   ########.fr       */
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
		if (cli_to_add_idx != -1)
		{
			g_vChannel[chann_idx]._operator.push_back(&g_aClient[cli_to_add_idx].second);
			send_to_channel("MODE " + chan_name + " +o " + name, client_idx, serv, chann_idx, false);
			g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
				+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " MODE " + chan_name + " +o " + name + "\r\n");
		}
	}
}

void		mode_b(const size_t &client_idx, const size_t &chann_idx, const bool minus, const std::string str, const std::string chan_name, const MyServ &serv)
{
	if (minus == true)
	{
		g_vChannel[chann_idx].remove_user_ban(g_aClient[client_idx].second);
	}
	else
	{
		t_ban_id new_ban;
		
		if (!std::strchr(str.c_str(), '!'))
		{
			new_ban.nickname = str;
			new_ban.username = "*";
			new_ban.hostname = "*";
			time(&new_ban.ban_date);
		}
		else
		{
			new_ban.nickname = str.substr(0, str.find('!'));
			new_ban.username = str.substr(str.find('!') + 1, str.find('@'));
			new_ban.hostname = str.substr(str.find('@') + 1);
			time(&new_ban.ban_date);
		}
		g_vChannel[chann_idx]._ban.push_back(new_ban);
	}
}
