/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 13:16:11 by lucas             #+#    #+#             */
/*   Updated: 2021/05/14 13:18:41 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	pass_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	(void)line;
	if (server_it->is_registered() == true)
	{
		server_it->push_to_buffer(create_msg(462, server_it, serv));
		return;
	}
}
