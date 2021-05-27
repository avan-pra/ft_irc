/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 16:24:30 by lucas             #+#    #+#             */
/*   Updated: 2021/05/27 16:43:47 by jvaquer          ###   ########.fr       */
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
	if (client_it->get_is_oper() == false)
	{
		client_it->push_to_buffer(create_msg(481, client_it, serv));
		return ;
	}
	throw DieException();
}
