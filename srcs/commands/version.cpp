/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 22:39:14 by lucas             #+#    #+#             */
/*   Updated: 2021/03/22 22:44:44 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	version_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	(void)line;
	(void)serv;
	g_aClient[client_idx].second.push_to_buffer(RPL_VERSION(std::string("beta 1.0"), "0", serv.get_hostname(), std::string("Actually in beta 1.0")));
}
