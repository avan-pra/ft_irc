/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specific_modes.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:11:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/16 12:24:27 by jvaquer          ###   ########.fr       */
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
		if (cli_to_add_idx != -1)
		{
			g_vChannel[chann_idx]._operator.push_back(&g_aClient[cli_to_add_idx].second);
			g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() +
				" MODE " + g_vChannel[chann_idx].get_name() + " +o " + name + "\r\n");
		}
	}
}

void		mode_b(const size_t &client_idx, const size_t &chann_idx, const char &sign, const std::string &str)
{
	t_ban_id user;

	if (!std::strchr(str.c_str(), '!'))
	{
		user.nickname = str;
		user.username = "*";
		user.hostname = "*";
	}
	else
	{
		user.nickname = str.substr(0, str.find('!'));
		user.username = str.substr(str.find('!') + 1, str.find('@'));
		user.hostname = str.substr(str.find('@') + 1);
	}
	if (sign == '-')
	{
		g_vChannel[chann_idx].remove_user_ban(user);
		g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
			" MODE " + g_vChannel[chann_idx].get_name() + " -b " + user.nickname + "!" + user.username + "@" + user.hostname + "\r\n");
	}
	else
	{	
		time(&user.ban_date);
		g_vChannel[chann_idx]._ban.push_back(user);
		g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
			" MODE " + g_vChannel[chann_idx].get_name() + " +b " + user.nickname + "!" + user.username + "@" + user.hostname + "\r\n");
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
		if (cli_to_add_idx != -1)
		{
			g_vChannel[chann_idx]._operator.push_back(&g_aClient[cli_to_add_idx].second);
			g_vChannel[chann_idx].send_to_all(":" + g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + 
											" MODE " + g_vChannel[chann_idx].get_name() + " +v " + name + "\r\n");	
		}
	}
}
