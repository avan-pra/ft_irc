/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 19:05:01 by lucas             #+#    #+#             */
/*   Updated: 2021/05/25 21:57:14 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/Disconnect.hpp"
#include "../../includes/commands.hpp"

void	error_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	(void)serv;
	(void)line;
	squit_command(":" + server_it->get_servername() + " SQUIT :Server going down", server_it, serv);
}
