/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 13:56:08 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/22 16:42:44 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <vector>

void	user_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string> params;
	std::vector<std::string> args;
	std::string realname, line_new;

	//On split first les : comme ca on a deja realname de stocke
	params = ft_split(line, ":");
	realname = params[1];
	line_new = params[0];
	//On resplit par ' ' la premiere string recuperee par le premier split
	//comme ca les parametres sont bien separes
	args = ft_split(line_new, " ");
	if (realname.size() < 1)
		g_aClient[client_idx].second.send_reply(create_error(461, client_idx, serv, "USER"));
	std::cout << "New Line = " << args.size() << std::endl;
	
	//ERR_ALREADYREGISTRED
	//ERR_NEEDMOREPARAMS
}
