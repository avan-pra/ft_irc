/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specific_modes.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:11:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/26 17:55:37 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <string>
#include <algorithm>
#include <cstring>

void		mode_o(const size_t &client_idx, const size_t &chann_idx, const char &sign, const std::string &name)
{
	if (sign == '-')
	{
		g_vChannel[chann_idx].remove_user_operator(name);
		g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
											" MODE " + g_vChannel[chann_idx].get_name() + " -o " + name + "\r\n");
	}
	else
	{
		size_t cli_to_add_idx;

		cli_to_add_idx = find_user_by_nick(name);
		if ((int)cli_to_add_idx != -1)
		{
			g_vChannel[chann_idx]._operator.push_back(&g_aClient[cli_to_add_idx].second);
			g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() +
				" MODE " + g_vChannel[chann_idx].get_name() + " +o " + name + "\r\n");
		}
	}
}

void		mode_b(const size_t &client_idx, const size_t &chann_idx, const char &sign, const std::string &str, const MyServ &serv)
{
	t_ban_id		user;

	//check mask
	format_mask(str, user.nickname, user.username, user.hostname);
	if (sign == '-')
	{
		g_vChannel[chann_idx].remove_user_ban(user);
		g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
			" MODE " + g_vChannel[chann_idx].get_name() + " -b " + user.nickname + "!" + user.username + "@" + user.hostname + "\r\n");
	}
	else
	{
		time(&user.ban_date);
		if (g_vChannel[chann_idx].is_ban_struct(user) == false)
		{
			g_vChannel[chann_idx]._ban.push_back(user);
			g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
				" MODE " + g_vChannel[chann_idx].get_name() + " +b " + user.nickname + "!" + user.username + "@" + user.hostname + "\r\n");
		}
		else
			g_aClient[client_idx].second.send_reply(create_msg(691, client_idx, serv, g_vChannel[chann_idx].get_name(), str));
	}
}

void		mode_v(const size_t &client_idx, const size_t &chann_idx, const char &sign, const std::string &name)
{
	if (sign == '-')
	{
		g_vChannel[chann_idx].remove_user_voice(name);
		g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
											" MODE " + g_vChannel[chann_idx].get_name() + " -v " + name + "\r\n");
	}
	else
	{
		size_t cli_to_add_idx;

		cli_to_add_idx = find_user_by_nick(name);
		if ((int)cli_to_add_idx != -1)
		{
			g_vChannel[chann_idx]._operator.push_back(&g_aClient[cli_to_add_idx].second);
			g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
											" MODE " + g_vChannel[chann_idx].get_name() + " +v " + name + "\r\n");	
		}
	}
}
