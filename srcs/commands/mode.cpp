/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:06:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/29 15:11:28 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

static void		check_nickname(const std::string str, const size_t &client_idx, const MyServ &serv)
{
	if (str != g_aClient[client_idx].second.get_nickname())
	{
		g_aClient[client_idx].second.send_reply(create_msg(502, client_idx, serv));
		throw std::exception();
	}
}

static int		check_channel_name(const std::string str, const size_t &client_idx, const MyServ &serv)
{
	for (int i = 0; i < g_vChannel.size(); i++)
		if (str == g_vChannel[i].get_name())
			return i;
	g_aClient[client_idx].second.send_reply(create_msg(401, client_idx, serv, str));
	throw std::exception();
	return 0;
}

static void		check_usr_in_channel(const int channel_idx, const size_t &client_idx, const MyServ &serv)
{
	std::vector<Client> vect = g_vChannel[channel_idx].get_users();
	for (int i = 0; i < vect.size(); i++)
		if (g_aClient[client_idx].second .get_nickname() == vect[i].get_nickname())
			return ;
	g_aClient[client_idx].second.send_reply(create_msg(441, client_idx, serv, g_vChannel[channel_idx].get_name(), g_aClient[client_idx].second.get_nickname()));
}

static void		check_usr_mode(const std::string mode, const size_t &client_idx, const MyServ &serv)
{
	//CHECK IF MODE IS VALID
	if (mode[0] != '+' || mode[0] != '-')
		g_aClient[client_idx].second.send_reply(create_msg(501, client_idx,serv));
	for (int i = 1; i < mode.size(); i++)
		if (!std::strchr(USER_VALID_MODE, mode[i]))
			g_aClient[client_idx].second.send_reply(create_msg(501, client_idx, serv));
	//1. +/-
	
	//2. valid mode
}

static void		check_chann_mode(const std::string mode, const int channel_idx, const int &client_idx, const MyServ &serv)
{
	//CHECK IF MODE IS VALID
	if (mode[0] != '+' || mode[0] != '-')
		g_aClient[client_idx].second.send_reply(create_msg(501, client_idx, serv));
	for (int i = 1; i < mode.size(); i++)
		if (!std::strchr(CHANNEL_VALID_MODE, mode[i]))
			g_aClient[client_idx].second.send_reply(create_msg(501, client_idx,serv));
	//CHECK IF MODE IS ALREADY SET
	//1. +/-
	//2. valid mode
}

void			mode_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> params;
	std::string str, mode;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "MODE"));
		throw std::exception();
	}
	try
	{
		str = params[1];
		if (std::strchr(CHANNEL_VALID_CHAR, str[0]))
		{
			int	channel_idx = 0;

			channel_idx = check_channel_name(str, client_idx, serv);
			check_usr_in_channel(channel_idx, client_idx, serv);
			if (params.size() == 2)
				g_aClient[client_idx].second.send_reply(create_msg(324, client_idx, serv, g_vChannel[channel_idx].get_name(), g_vChannel[channel_idx].get_mode(), ""));
			else
			{
				mode = params[2];
				check_chann_mode(mode, channel_idx, client_idx, serv);
			}
		}
		else
		{
			check_nickname(str, client_idx, serv);
			if (params.size() == 2)
				g_aClient[client_idx].second.send_reply(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
			else 
			{
				mode = params[2];
				check_usr_mode(mode, client_idx, serv);
			}
		}
	}
	catch(const std::exception &e){return ;}
}
