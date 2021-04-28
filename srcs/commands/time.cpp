/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 14:54:27 by lucas             #+#    #+#             */
/*   Updated: 2021/04/22 16:11:29 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <ctime>

void	time_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	time_t			get_time;
	std::string		now;

	(void)line;
	time(&get_time);
	now = ctime(&get_time);
	if (now[now.size() - 1] == '\n')
		now.resize(now.size() - 1);
	g_aClient[client_idx].second.push_to_buffer(RPL_TIME(serv.get_hostname(), now.c_str()));
}
