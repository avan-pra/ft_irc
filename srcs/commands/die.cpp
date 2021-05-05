/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 16:24:30 by lucas             #+#    #+#             */
/*   Updated: 2021/05/05 19:36:57 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Disconnect.hpp"

void		die_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string		full_msg("");

	(void)line;
	(void)client_it;
	(void)serv;
	throw DieException();
}
