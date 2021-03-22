/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 13:56:08 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/22 18:28:04 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <vector>

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
		++it;
    return !s.empty() && it == s.end();
}

void	user_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string> params;
	std::vector<std::string> args;
	std::string realname, line_new;

	//On split first les : comme ca on a deja realname de stocke
	try
	{
		params = ft_split(line, ":");
		realname = params[1];
		line_new = params[0];
		//On resplit par ' ' la premiere string recuperee par le premier split
		//comme ca les parametres sont bien separes
		args = ft_split(line_new, " ");
		if (realname.size() < 1 || args.size() < 4)
		{
			g_aClient[client_idx].second.send_reply(create_error(461, client_idx, serv, " USER"));
			throw std::exception();
		}
		else if (!is_number(args[2]))
			throw std::exception();
		g_aClient[client_idx].second.set_username(args[1]);
		g_aClient[client_idx].second.set_realname(realname);
		g_aClient[client_idx].second.set_mode(args[2]);
	}
	catch(const std::exception &e) {return ; }
	
	//ERR_ALREADYREGISTRED
	//ERR_NEEDMOREPARAMS
}
