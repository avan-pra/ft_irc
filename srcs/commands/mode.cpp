/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:06:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/23 23:07:16 by jvaquer          ###   ########.fr       */
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

void			mode_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string> params;
	std::string nick, cmd;

	try
	{
		params = ft_split(line, " ");
		nick = params[0];
		check_nick(nick, client_idx, serv);
		//cmd = params[1];
	}
	catch(const std::exception &e){return ;}
}
