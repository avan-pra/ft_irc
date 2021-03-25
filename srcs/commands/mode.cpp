/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:06:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/25 17:49:26 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

static void		check_nick(const std::string nick, const size_t &client_idx, const Server &serv)
{
	if (nick != g_aClient[client_idx].second.get_nickname())
	{
		g_aClient[client_idx].second.send_reply(create_msg(502, client_idx, serv));
		throw std::exception();
	}
}

static void		check_params(const std::string cmd, const size_t &client_idx, const Server &serv)
{
	//IF ONLY NICK GIVEN RPLY WITH CURRENT MODE
	
	//CHECK IF MODE IS VALID
	//1. +/-
	//2. valid mode
}

void			mode_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string> params;
	std::string nick, cmd;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "MODE"));
		throw std::exception();
	}
	try
	{
		nick = params[1];
		check_nick(nick, client_idx, serv);
		if (params.size() == 2)
			g_aClient[client_idx].second.send_reply(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
		else 
		{
			cmd = params[2];
			check_params(cmd, client_idx, serv);
		}
	}
	catch(const std::exception &e){return ;}
}
